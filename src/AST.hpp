#ifndef __jesse_ast__
#define __jesse_ast__
#include <memory>
#include <string>
#include <vector>

class ExpressAST {
public:
  virtual ~ExpressAST() {}
  virtual std::string getText() = 0;
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
};

class VariableExprAST : public ExpressAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
  std::string getText() override {
    return "{\"type\":\"variable expression\", \"name\": \"" + Name + "\"}";
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
};

// function AST part
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args)
      : Name(name), Args(std::move(Args)) {}
  std::string getText() {
    return "{\"type\":\"Prototype\", \"Name\": \"" + Name +
           "\", \"argsSize\":" + std::to_string(Args.size()) + "}";
  }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExpressAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExpressAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}
  std::string getText() {
    return "{\"type\":\"Function\", \"proto\": " + Proto->getText() +
           ", \"body\":" + Body->getText() + "}";
  }
};

#endif