#ifndef __jesse_token__
#define __jesse_token__
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <sstream>
#include <string>
#include <iostream>
enum Token : int32_t {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,
};

static std::string identifier;
static double numbValue;
static char lastChar;

static int getToken(std::function<char()> getchar) {
  lastChar = ' ';
  while (isspace(lastChar)) {
    lastChar = getchar();
  }

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

  // handle number
  if (isdigit(lastChar) || lastChar == '.') { // number
    std::stringstream numStr;
    do {
      numStr << (char)lastChar;
      lastChar = getchar();
    } while (isdigit(lastChar) || lastChar == '.');
    numbValue = strtod(numStr.str().c_str(), 0);
    return Token::tok_number;
  }

  // handle comments
  if (lastChar == '#') {
    // Comment until end of line.
    do {
      lastChar = getchar();
    } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
  }

  if (lastChar == EOF) {
    return Token::tok_eof;
  }

  // cannot identifier any token, return its value
  // step last char to next one, and return this char
  int ThisChar = lastChar;
  lastChar = getchar();
  return ThisChar;
}

#endif