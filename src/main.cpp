#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include "Token.hpp"
#include "AST.hpp"


static int currentToken;
extern double numbValue;

static std::unique_ptr<NumberExpressionAST> parseNumberExpression() {
    auto result = std::make_unique<NumberExpressionAST>(numbValue);
    return result;
}




int main(int, char **) {
  std::ifstream input(
      "/home/jesse/Documents/workspace/Kaleidoscope/testscript/test.kl");
  if (!input) {
    std::cerr << "cannot open file\n";
    return 1;
  }
  input.seekg(0, std::ios::end);
  uint32_t size{static_cast<uint32_t>(input.tellg()) + 1U};
  input.seekg(0, std::ios::beg);
  std::unique_ptr<char[]> content(new char[size]{});
  uint32_t pos = 0;

  input.read(content.get(), size);
  input.close();

  auto getchar = [&pos, &content, &size]() {
    if (pos < size) {
      char chr = (char)(*(content.get() + pos));
      pos++;
      return chr;
    } else {
      return static_cast<char>(EOF);
    }
  };

  currentToken = getToken(getchar);
  while (true) {
    switch (currentToken) {
        case tok_eof:
            return 0;
        case ';':
            currentToken = getToken(getchar);
        case tok_extern:
            printf("extern\n");
            currentToken = getToken(getchar);
            break;
        case tok_def:
            printf("def\n");
            currentToken = getToken(getchar);
            break;
        default:
            printf("expression: %c\n", (char)currentToken);
            currentToken = getToken(getchar);
            break;
    }
  }

  
}
