#ifndef __Token_h
#define __Token_h

#include "SharedObject.h"
#include <ctype.h>
#include <iostream>
#include <string>

using namespace std;

enum TokenType
{
  UNDEF,
  // reserved words
  BOOLEAN,
  CLASS,
  ELSE,
  EXTENDS,
  FALSE,
  IF,
  INT,
  LENGTH,
  MAIN,
  NEW,
  PUBLIC,
  RETURN,
  STATIC,
  STRING,
  SYSTEM_OUT_PRINTLN,
  THIS,
  TRUE,
  VOID,
  WHILE,
  // sequence of letters or symbols
  ID,
  // sequence of numbers
  INTEGER_LITERAL,
  // &&, <, >, +, −, ∗, /, =, ==, != e !;
  OP_AND,
  OP_LT,
  OP_GT,
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_ASSIGN,
  OP_EQ,
  OP_NE,
  OP_NOT,
  //  (, ), [, ], {, }, ;, . e ,;
  SEP_LPAREN,
  SEP_RPAREN,
  SEP_LBRACKET,
  SEP_RBRACKET,
  SEP_LBRACE,
  SEP_RBRACE,
  SEP_SEMICOLON,
  SEP_DOT,
  SEP_COMMA,
  // end of file
  END_OF_FILE
};

class Token: public mjc::SharedObject
{
public:
  TokenType name;
  string lexeme;

  Token(TokenType name):
    name{name}
  {
    // do nothing
  }

  Token(TokenType name, const string& lexeme):
    name{name},
    lexeme{lexeme}
  {
    // do nothing
  }

private:
  Token* _next{};

  friend class Parser;

};

using TokenPtr = mjc::ObjectPtr<Token>;

#endif //  __Token_h
