### 确定expression AST
基类
```C++
class ExpressAST {
public:
  virtual ~ExpressAST() {}
};
```

### Token的获取技巧
首先是要有两个数据在追踪token 和 char， `lastChar`和`currentToken`.  
currentToken和lastChar是步进式关系。  
例如 `(3+4)`  
`lastChar = (, currentToken = null`  
=>`lastChar = 3, currentToken = (`  
=> `lastChar = +, currentToken = Number`  
=> `lastChar = 4, currentToken = +`  
=> `lastChar = ), currentToken = Number`  
=> `lastChar = EOF, currentToken = )`  
=> `lastChar = null, currentToken = EOF`  

这样做的好处是，当我们步进的时候，我们可以有一步缓存，比如  
```C++
// Okay, we know this is a binop.
int BinOp = CurTok;
getNextToken();  // eat binop
```
我们可以拿到数值，然后在eat掉，进入下一个。

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

### 递归下降算法

比如表达式 (add(sub(1+2, 3), ^10))
这样的解析就存在递归下降算法

```C++

```
