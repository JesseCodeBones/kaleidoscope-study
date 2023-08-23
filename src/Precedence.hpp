#ifndef __jesse_precedence__
#define __jesse_precedence__


#include <cctype>
#include <unordered_map>

class PrecedenceParser {

public:
  PrecedenceParser() {
    binOpPrecedence['<'] = 10;
    binOpPrecedence['+'] = 20;
    binOpPrecedence['-'] = 20;
    binOpPrecedence['*'] = 40;
    binOpPrecedence['/'] = 40;
  }

  int getOpPrecedence(char c) {
    if (!isascii(c)){
      return -1;
    }
    int prec = binOpPrecedence[c];
    if (prec <= 0) return -1;
    return prec;
  }

private:
  std::unordered_map<char, int> binOpPrecedence;
};

#endif