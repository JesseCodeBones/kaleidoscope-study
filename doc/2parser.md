### 确定expression AST
基类
```C++
class ExpressAST {
public:
  virtual ~ExpressAST() {}
};
```

### 其他AST子类

比如binary expression  
`a+b()`, 需要有一个操作符， 左expression 和 右expression  

```C++
class NumberExpressionAST {
public:
  NumberExpressionAST(double Val) : value(Val) {}
  virtual ~NumberExpressionAST() {}
  double value;
};

class VariableExprAST : public ExpressAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExpressAST {
  char Op;
  std::unique_ptr<ExpressAST> LHS, RHS;
public:
  BinaryExprAST(char op, std::unique_ptr<ExpressAST> LHS,
                std::unique_ptr<ExpressAST> RHS)
    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExpressAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExpressAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExpressAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}
};

```