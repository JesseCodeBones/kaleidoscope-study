#ifndef __jesse_ast__
#define __jesse_ast__

#include <iostream>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static llvm::Module TheModule("my module", TheContext);
static std::map<std::string, llvm::Value *> NamedValues;

class ExpressAST {
public:
  virtual ~ExpressAST() {}
  virtual std::string getText() = 0;
  virtual llvm::Value *codegen() = 0;
};

class NumberExpressionAST : public ExpressAST {
public:
  NumberExpressionAST(double Val) : value(Val) {}
  virtual ~NumberExpressionAST() {}
  double value;
  std::string getText() override {
    return "{\"type\":\"Number expression\", \"value\": " +
           std::to_string(value) + "}";
  }
  virtual llvm::Value *codegen() override {
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(value));
  }
};

class VariableExprAST : public ExpressAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
  std::string getText() override {
    return "{\"type\":\"variable expression\", \"name\": \"" + Name + "\"}";
  }
  virtual llvm::Value *codegen() override {
    llvm::Value *v = NamedValues[Name];
    if (!v) {
      throw std::runtime_error("no value for name=" + Name);
    }
    return v;
  }
};

class BinaryExprAST : public ExpressAST {
  char Op;
  std::unique_ptr<ExpressAST> LHS, RHS;

public:
  BinaryExprAST(char op, std::unique_ptr<ExpressAST> LHS,
                std::unique_ptr<ExpressAST> RHS)
      : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  std::string getText() override {
    return "{\"type\":\"binary expression\", \"LHS\": " + LHS->getText() +
           ", \"op\": \"" + Op + "\", \"RHS\": " + RHS->getText() + "}";
  }

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
};

class CallExprAST : public ExpressAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExpressAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExpressAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}
  std::string getText() override {
    return "{\"type\":\"call expression\", \"callee\": " + Callee +
           ", \"argsSize\":" + std::to_string(Args.size()) + "}";
  }
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
};

// function AST part
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args)
      : Name(name), Args(std::move(Args)) {}
  std::string &getName() { return Name; }
  std::string getText() {
    return "{\"type\":\"Prototype\", \"Name\": \"" + Name +
           "\", \"argsSize\":" + std::to_string(Args.size()) + "}";
  }

  virtual llvm::Function *codegen() {
    std::vector<llvm::Type *> doubles(Args.size(),
                                      llvm::Type::getDoubleTy(TheContext));
    llvm::FunctionType *functionType = nullptr;
    if (Name == "main") {
      // change main function type from double to int
      functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext),
                                             doubles, false);
    } else {
      functionType = llvm::FunctionType::get(
          llvm::Type::getDoubleTy(TheContext), doubles, false);
    }
    llvm::Function *func = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, Name, TheModule);
    int index = 0;
    for (auto &arg : func->args()) {
      arg.setName(Args[index++]);
    }
    return func;
  }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExpressAST> Body;
  llvm::legacy::FunctionPassManager manager;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExpressAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)), manager(&TheModule) {
    manager.add(llvm::createInstructionCombiningPass());
    manager.add(llvm::createReassociatePass());
    manager.add(llvm::createGVNSinkPass());
    manager.add(llvm::createCFGSimplificationPass());
    manager.doInitialization();
  }
  std::string getText() {
    return "{\"type\":\"Function\", \"proto\": " + Proto->getText() +
           ", \"body\":" + Body->getText() + "}";
  }
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
      if (Proto->getName() == "main") {
        // main function will return int value
        ret = Builder.CreateFPToSI(ret, llvm::Type::getInt32Ty(TheContext));
      }
      Builder.CreateRet(ret);
      bool result = llvm::verifyFunction(*func);
      std::cout << "verify result: " << result << std::endl;
      manager.run(*func);
      return func;
    }
    func->eraseFromParent();
    return nullptr;
  }
};

#endif