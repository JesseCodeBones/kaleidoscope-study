# parse 剩余部分
### top level 文法

> top ::= function definition | extern declaration | top level expression

### 数据结构
prototype {
  name,
  variables
}

function {
  prototype,
  expressions
}

### parse过程

> parse prototype
```C++
static std::unique_ptr<PrototypeAST>
parsePrototype(std::function<char()> getchar) {
  if (currentToken != Token::tok_identifier) {
    throw new std::runtime_error("function define must have identifier");
  }
  std::string functionName = identifier;
  getNextToken(getchar);

  if (currentToken != '(') {
    throw new std::runtime_error("Expected '(' in prototype");
  }

  std::vector<std::string> argNames;
  while (getNextToken(getchar) == Token::tok_identifier) {
    argNames.push_back(identifier);
  }

  if (currentToken != ')') {
    throw new std::runtime_error("Expected ')' in prototype");
  }
  getNextToken(getchar);
  return std::make_unique<PrototypeAST>(std::move(functionName), std::move(argNames));
}
```

> parse function
```C++
static std::unique_ptr<FunctionAST>
parseFunction(std::function<char()> getchar) {
  getNextToken(getchar);
  auto prototype = parsePrototype(getchar);
  if (auto expression = parseExpression(getchar)) {
    return std::make_unique<FunctionAST>(std::move(prototype),
                                         std::move(expression));
  }
  return nullptr;
}
```

### 将top level expression放入一个特殊函数中
```C++
static std::unique_ptr<FunctionAST>
parseToplevelAST(std::function<char()> getchar) {

  if (auto expression = parseExpression(getchar)) {
    auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(Proto),
                                         std::move(expression));
  }
  return nullptr;
}
```

## 重点，文件编译driver

这里的重点是用EOF作为出口。

```C++
// top ::= function define | external function | expression | ; | EOF
static void driver(std::function<char()> getchar) {
  while (true) {
    switch (currentToken) {
    case Token::tok_eof:
      return;
    case ';': {
      getNextToken(getchar);
      break;
    }
    case Token::tok_def: {
      auto function = parseFunction(getchar);
      if (function) {
        std::cout << function->getText() << std::endl;
      }
      break;
    }
    case Token::tok_extern: {
      auto ext = parseExtern(getchar);
      ext->getText();
      break;
    }
    default: {
      auto function = parseToplevelAST(getchar);
      function->getText();
      break;
    }
    }
  }
}
```
### 例子
`def fib(x) x + 10 ;`  
编译结果：  
```json
{
  "type": "Function",
  "proto": {
    "type": "Prototype",
    "Name": "fib",
    "argsSize": 1
  },
  "body": {
    "type": "binary expression",
    "LHS": {
      "type": "variable expression",
      "name": "x"
    },
    "op": "+",
    "RHS": {
      "type": "Number expression",
      "value": 10.000000
    }
  }
}
```
