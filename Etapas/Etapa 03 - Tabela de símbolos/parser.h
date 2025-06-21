#ifndef __Parser_h
#define __Parser_h

#include "Class.h"
#include "GlobalScope.h"
#include "scanner.h"
#include <cstdarg>

using namespace mjc::symbol;

class Parser
{
public:
  Parser(string);
  void run();

private:
  ScannerPtr scanner;
  TokenPtr lToken;
  ScopePtr currentScope;
  int errorCount;

  void program();
  void mainClass();
  void classDeclaration();
  void varDeclaration();
  void methodDeclaration(Class*);
  void params(Method*);
  const Type* type();
  void statement(const Class*);
  const Type* expression(const Class*);
  const Type* equalityExpression(const Class*);
  const Type* logicalExpression(const Class*);
  const Type* arithmeticExpression(const Class*);
  const Type* unaryExpression(const Class*);
  const Type* prefixExpression(const Class*);
  const Type* primaryExpression(const Class*);
  void expressionList(const Class*);

  Token* lookahead(Token*, int = 1);

  void advance()
  {
    lToken = lToken->_next ? lToken->_next : scanner->nextToken();
  }

  void match(int);
  string matchIdentifier();
  Class* matchClass();

  void report(const char*, va_list);
  void report(const char*, ...);
  void error(const char*, ...);
  bool tryCast(const Type*, const Type*);

};

#endif // __Parser_h
