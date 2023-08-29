## 表达式codegen
### expression AST增加codegen需函数
```C++
class ExpressAST {
public:
  virtual ~ExpressAST() {}
  virtual std::string getText() = 0;
  virtual llvm::Value *codegen() = 0;
};
```
### Number AST codegen

```C++
virtual llvm::Value *codegen() override {
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(value));
  }
```

### Variable AST codegen
这里就是变量和存量之间一个匹配，将变量替换成存量  
```C++
  virtual llvm::Value *codegen() override {
    llvm::Value *v = NamedValues[Name];
    if (!v) {
      throw std::runtime_error("no value for name=" + Name);
    }
    return v;
  }
```
### binary codegen
```C++
virtual llvm::Value *codegen() override {
    llvm::Value *L = LHS->codegen();
    llvm::Value *R = RHS->codegen();
    if (!L || !R) {
      throw std::runtime_error("illegal L or R");
    }
    switch (Op) {
    case '+': {
      return Builder.CreateFAdd(L, R);
    }
    case '-': {
      return Builder.CreateFSub(L, R);
    }
    case '*': {
      return Builder.CreateFMul(L, R);
    }
    case '/': {
      return Builder.CreateFDiv(L, R);
    }
    case '<': {
      L = Builder.CreateFCmpULT(L, R);
      return Builder.CreateUIToFP(L, llvm::Type::getFloatTy(TheContext),
                                  "boolTemp");
    }
    default: {
      throw std::runtime_error("illegal op");
    }
    }
  }
```
### call code gen
```C++
 virtual llvm::Value *codegen() override {
    llvm::Function *calleeFunction = TheModule.getFunction(Callee);
    if (!calleeFunction) {
      throw std::runtime_error("cannot find callee");
    }
    if (calleeFunction->arg_size() != Args.size()) {
      throw std::runtime_error("signature error");
    }
    std::vector<llvm::Value *> argsV;
    for (auto &arg : Args) {
      argsV.push_back(arg->codegen());
    }
    return Builder.CreateCall(calleeFunction, argsV, "callTemp");
  }
```

## 对于方法的codegen
### prototype
通过名字和参数列表，生成函数模型  
```C++
 virtual llvm::Function *codegen() {
    std::vector<llvm::Type *> doubles(Args.size(),
                                      llvm::Type::getDoubleTy(TheContext));
    llvm::FunctionType *functionType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(TheContext), doubles, false);
    llvm::Function *func = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, Name, TheModule);
    int index = 0;
    for (auto &arg : func->args()) {
      arg.setName(Args[index++]);
    }
    return func;
  }
```

### function codegen
往函数模型中填充表达式并且进行优化  
```C++
virtual llvm::Function *codegen() {
    llvm::Function *func = TheModule.getFunction(Proto->getName());
    if (!func) {
      func = Proto->codegen();
    }
    if (!func) {
      return nullptr;
    }

    if (!func->empty()) {
      throw std::runtime_error("redefine function");
    }
    llvm::BasicBlock *basicBlock =
        llvm::BasicBlock::Create(TheContext, "entry", func);
    Builder.SetInsertPoint(basicBlock);
    NamedValues.clear();
    for (auto &arg : func->args()) {
      NamedValues[std::string(arg.getName())] = &arg;
    }
    if (llvm::Value *ret = Body->codegen()) {
      Builder.CreateRet(ret);
      bool result = llvm::verifyFunction(*func);
      std::cout << "verify result: " << result << std::endl;
      manager.run(*func);
      return func;
    }
    func->eraseFromParent();
    return nullptr;
  }
```

## 结果
`def fib(x) 2 * x + 10 + 5 ; fib(42);\n`的执行结果：  

```ll
; ModuleID = 'my module'
source_filename = "my module"

define double @fib(double %x) {
entry:
  %0 = fmul double %x, 2.000000e+00
  %1 = fadd double %0, 1.000000e+01
  %2 = fadd double %1, 5.000000e+00
  ret double %2
}

define i32 @main() {
entry:
  %callTemp = call double @fib(double 4.200000e+01)
  %0 = fptosi double %callTemp to i32
  ret i32 %0
}
```
