# RecursiveDescent 递归下降分析算法

### 解析目标
`1+2×3`，`1×2+3`，`1×2+3×4`  
### 数据结构
```
BinaryExpression{
  LHS: expression
  opt: token
  RHS: expression
}
```
### 语法定义
```
expression -> binary expression | call expression ...
binary expression -> expression + opcode + expression
```

### 期望结果
以 `1+2×3`为例，期望的解析结果是：
```
binary expression {
  Number expression
  +
  binary expression {
    Number expression
    *
    Number expression
  }
}
```

### 解析过程

1. 基本expression解析
>> 这里解析的是Number expression, Identifier expression, parenthesis Expression这种比较简单不需要复杂递归运算的expression。
```C++
static std::unique_ptr<ExpressAST> parsePrimary(std::function<char()> getchar) {
  switch (currentToken) {
  case Token::tok_identifier:
    return parseIdentifierExpression(getchar);
  case Token::tok_number:
    return parseNumberExpression(getchar);
  case '(':
    return parseParenExpression(getchar);
  default:
    return nullptr;
  }
}
```

2. 统一expression解析入口
>> 这里试着使用基本解析入口解析一个`LHS`，然后判断当前需要解析的opcode， 如果是个2元操作符，则进行`RHS`的解析

```C++
static std::unique_ptr<ExpressAST>
parseExpression(std::function<char()> getchar) {
  auto LHS = parsePrimary(getchar);
  if (!LHS) {
    return nullptr;
  }
  return parseBinaryRHS(0, std::move(LHS), getchar);
}
```
>> 这里传入parseBinaryRHS中的操作付优先级为0, 因为如果当前处理的token不是二元操作符的话优先级为-1, 所以将当前LHS直接传入下一级，如果是非二元操作符的话会直接返回LHS完成`parseExpression`

3. 重头戏，`parseBinaryRHS`
>> 先上代码
```C++
static std::unique_ptr<ExpressAST>
parseBinaryRHS(int expressionPrecedence, std::unique_ptr<ExpressAST> LHS,
               std::function<char()> getchar) {
  while (true) {
    int currentOperatorPrecedence = getTokenPrecedence();
    if (currentOperatorPrecedence < expressionPrecedence) {
      // 唯一出口，当当前处理的token不是二元操作符的时候
      return LHS;
    }

    int ope = currentToken;
    getNextToken(getchar);
    auto RHS = parsePrimary(getchar);
    if (!RHS) {
      return nullptr;
    }
    int nextOperatorPrecedence = getTokenPrecedence();
    if (currentOperatorPrecedence < nextOperatorPrecedence) {
      
      RHS = parseBinaryRHS(currentOperatorPrecedence + 1, std::move(RHS),
                           getchar);
    }
    LHS = std::make_unique<BinaryExprAST>(static_cast<char>(ope), std::move(LHS), std::move(RHS));
  }
  return nullptr;
}
```
>> 该方法有效出口只有一个
```C++
if (currentOperatorPrecedence < expressionPrecedence) {
  // 唯一出口，当当前处理的token不是二元操作符的时候
  return LHS;
}
```
>> 获取当前token和解析一个普通expression类型的RHS

```C++
    int ope = currentToken;
    getNextToken(getchar);
    auto RHS = parsePrimary(getchar);
    if (!RHS) {
      return nullptr;
    }
```
但是有个问题，当前LHS,RHS都并非是最终LHS和RHS
所以下面需要进行判断  

```C++
int nextOperatorPrecedence = getTokenPrecedence();
if (currentOperatorPrecedence < nextOperatorPrecedence) {
  // a+b*c
  // 如果当前是+下一个是*,
  // 则将当前的RHS当作下一个操作的LHS进行处理，得到新的RHS
  // +1的意思是下一级循环循环到和当前优先级一样的操作副，不要放在右expression中，而是直接返回LHS
  RHS = parseBinaryRHS(currentOperatorPrecedence + 1, std::move(RHS),
                        getchar);
}
LHS = std::make_unique<BinaryExprAST>(static_cast<char>(ope), std::move(LHS), std::move(RHS));
```
>> 首先，是否RHS是否是当前的基本expression是由下一个token的算数优先级来决定的，如果优先级高，则RHS需要进一步组装。 比如 a+b*c, 到了这里，会将b*c当作RHS

>> 其次，如果下一个算数优先级比当前处理的优先级低，则将当前的LHS和RHS组装成一个新的LHS，然后进行下一轮装配，直到currentToken不在是一个二元操作符，从出口弹出。

>> 这里的`parseBinaryRHS`中会进入到下一个while循环中，我们设置的弹出点应该比当前优先级高一点。比如当前操作符是`+`，当RHS中循环到`+`或`-`，这个时候应该终止循环返回RHS。而终止循环的办法就是待操作操作符的优先级比输入优先级低。也就是`+1`的作用所在。
>> 这里的 `LHS = std::make_unique<BinaryExprAST>(static_cast<char>(ope), std::move(LHS), std::move(RHS));` 整理操作并非要推出循环，而是要进入下一个循环，而当前LHS已经被解析好了，继续解析下一个RHS

### 解析结果实例

1. `a-b*c-d`
```json
{
  "type": "binary expression",
  "LHS": {
    "type": "binary expression",
    "LHS": {
      "type": "variable expression",
      "name": "a"
    },
    "op": "-",
    "RHS": {
      "type": "binary expression",
      "LHS": {
        "type": "variable expression",
        "name": "b"
      },
      "op": "*",
      "RHS": {
        "type": "variable expression",
        "name": "c"
      }
    }
  },
  "op": "-",
  "RHS": {
    "type": "variable expression",
    "name": "d"
  }
}
```

2. `a/b-c*d`

```json
{
  "type": "binary expression",
  "LHS": {
    "type": "binary expression",
    "LHS": {
      "type": "variable expression",
      "name": "a"
    },
    "op": "/",
    "RHS": {
      "type": "variable expression",
      "name": "b"
    }
  },
  "op": "-",
  "RHS": {
    "type": "binary expression",
    "LHS": {
      "type": "variable expression",
      "name": "c"
    },
    "op": "*",
    "RHS": {
      "type": "variable expression",
      "name": "d"
    }
  }
}
```
