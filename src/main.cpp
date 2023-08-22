#include "AST.hpp"
#include "Precedence.hpp"
#include "Token.hpp"
#include <array>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

extern double numbValue;

static int currentToken;
static int getNextToken(std::function<char()> getchar) {
  return currentToken = getToken(getchar);
}

static std::unique_ptr<ExpressAST> parseNumberExpression() {
  auto result = std::make_unique<NumberExpressionAST>(numbValue);
  return std::move(result);
}

static std::unique_ptr<ExpressAST>
parseExpression(std::function<char()> getchar);
static std::unique_ptr<ExpressAST>
parseIdentifierExpression(std::function<char()> getchar);
static std::unique_ptr<ExpressAST>
parseParenExpression(std::function<char()> getchar);

static std::unique_ptr<ExpressAST> parsePrimary(std::function<char()> getchar) {
  switch (currentToken) {
  case Token::tok_identifier:
    return parseIdentifierExpression(getchar);
  case Token::tok_number:
    return parseNumberExpression();
  case '(':
    return parseParenExpression(getchar);
  default:
    return nullptr;
  }
}

static std::unique_ptr<ExpressAST>
parseIdentifierExpression(std::function<char()> getchar) {
  std::string identifierName = identifier;
  getNextToken(getchar);
  if (currentToken != '(') { // not call, variable expression
    return std::make_unique<VariableExprAST>(identifierName);
  } else {
    // call expression
    getNextToken(getchar);
    std::vector<std::unique_ptr<ExpressAST>> args;
    if (currentToken != ')') {
      while (true) {
        if (auto arg = parseExpression(getchar)) {
          args.push_back(std::move(arg));
        } else {
          return nullptr;
        }

        if (lastChar == ')') {
          break;
        }

        if (lastChar != ',') {
          throw std::runtime_error("arguments expression error");
        }
        getToken(getchar);
      }
    }
    return std::make_unique<CallExprAST>(identifierName, std::move(args));
  }
}

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExpressAST>
parseParenExpression(std::function<char()> getchar) {
  getNextToken(getchar); // eat (
  auto v = parseExpression(getchar);
  if (!v) {
    return nullptr;
  } else {
    if (static_cast<char>(getToken(getchar)) != ')') {
      throw std::runtime_error("incorrect () expression");
    }
  }
  return v;
}

int parseRealScript() {
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

static std::unique_ptr<ExpressAST>
parseBinaryRHS(int expressionPrecedence, std::unique_ptr<ExpressAST> LHS,
               std::function<char()> getchar) {
  return nullptr;
}

static std::unique_ptr<ExpressAST>
parseExpression(std::function<char()> getchar) {
  auto LHS = parsePrimary(getchar);
  if (!LHS) {
    return nullptr;
  }
  return parseBinaryRHS(0, std::move(LHS), getchar); // TODO fix it
}

int precedenceParse() {

  std::stringstream contentStream;
  contentStream << "(3+5*7);\n";
  contentStream << EOF;
  std::string str = contentStream.str();
  int pos = 0;
  int size = 12;

  auto getchar = [&pos, &str, &size]() {
    if (pos < size) {
      char chr = str.at(pos);
      pos++;
      return chr;
    } else {
      return static_cast<char>(EOF);
    }
  };
  getNextToken(getchar);
  parseExpression(getchar);
  return 0;
}

int main(int, char **) { return precedenceParse(); }
