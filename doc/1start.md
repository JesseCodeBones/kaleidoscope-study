### 第一步
处理一个text文件并且理解它说的是什么  
就是将一个输入且分成不同的tokens  
每一个token包含一个token Code和潜在的元数据信息，比如number或者字符串
### Token定义技巧
通用token：  
eof  
命令token：  
def  
extern  
基础类型：  
identifier  
number  
因为该语言不支持string literal，所以只有数字的支持。  

### token解析技巧
`lastChar`是读取到的最后的一个字符。通过该字符进行判断。
1, 空字符放过
```C++
while (isspace(lastChar)) {
    lastChar = getchar();
  }
```

2, 合法字符串，先提取到默认标识符中，再判断标识符是不是命令Token，然后返回
```C++
if (isalpha(lastChar)) { // [a-zA-Z][a-zA-Z0-9]*
    identifier = lastChar;
    while (isalnum((lastChar = getchar()))) {
      identifier += lastChar;
    }

    if (identifier == "def") {
      return Token::tok_def;
    }

    if (identifier == "extern") {
      return Token::tok_extern;
    }

    return Token::tok_identifier;
  }
```

3, 数字的处理，数字只有一种可能就是numbToken
```C++
  // handle number
  if (isdigit(lastChar) || lastChar == '.') { // number
    std::stringstream numStr;
    do {
      numStr << lastChar;
    } while (isdigit(lastChar) || lastChar == '.');
    numbValue = strtod(numStr.str().c_str(), 0);
    return Token::tok_number;
  }
```

4, 对于注释的处理，注释以#开头，行尾结束，直接放过就可以
```C++
// handle comments
  if (lastChar == '#') {
    // Comment until end of line.
    do {
      lastChar = getchar();
    } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
  }
```

5, 对EOF的处理
```C++
  if (lastChar == EOF) {
    return Token::tok_eof;
  }
```

6, 对待不认识的字符，可以直接返回
```C++
  int ThisChar = lastChar;
  lastChar = getchar();
  return ThisChar;
```
