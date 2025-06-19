#include "Method.h"
#include "parser.h"
#include <cassert>
#include <cstdio>
#include <iostream>

using namespace mjc::symbol;

Parser::Parser(string input):
  scanner{new Scanner(input)},
  currentScope{Scope::global()}
{
  // do nothing
}

Token*
Parser::lookahead(Token* token, int n)
{
  auto t = token;

  for (int i = 0; i < n; ++i)
    t = t->_next ? t->_next : (t->_next = scanner->nextToken());
  return t;
}

void
Parser::match(int t)
{
  if (lToken->name != t)
    error("Unexpected '%s'", lToken->lexeme.c_str());
  advance();
}

string
Parser::matchIdentifier()
{
  if (lToken->name != ID)
    error("Identifier expected");

  auto id = lToken->lexeme;

  advance();
  return id;
}

void
Parser::run()
{
  errorCount = 0;
  if ((lToken = scanner->nextToken())->name == END_OF_FILE)
    return;
  program();
  if (errorCount)
    printf("%d error(s)\n", errorCount);
  else
    puts("Compilation completed successfully!");
}

void
Parser::program()
{
  mainClass();
  while (lToken->name == CLASS)
    classDeclaration();
  if (lToken->name != END_OF_FILE)
    error("Syntax error");
}

void
Parser::mainClass()
{
  if (lToken->name != CLASS)
    error("'class' expected");
  advance(); // CLASS

  auto className = matchIdentifier();
  auto mainClass = Scope::global()->addClass(className, nullptr);

  if (lToken->name != SEP_LBRACE)
    error("'{' expected");
  advance();
  if (lToken->name == PUBLIC)
  {
    advance();
    if (lToken->name == STATIC)
    {
      advance();
      if (lToken->name == VOID)
      {
        advance();
        if (lToken->name != MAIN)
          error("'main' expected");
        advance();
        if (lToken->name != SEP_LPAREN)
          error("'(' expected");
        advance();
        if (lToken->name != STRING)
          error("Parameter in 'main' must be 'String[]'");
        advance();
        if (lToken->name != SEP_LBRACKET)
          error("'[' expected");
        advance();
        match(SEP_RBRACKET);
        matchIdentifier();
        match(SEP_RPAREN);
        if (lToken->name != SEP_LBRACE)
          error("'{' expected");
        advance();
        statement(mainClass);
        match(SEP_RBRACE);
        match(SEP_RBRACE);
        currentScope = mainClass->parent();
        return;
      }
    }
  }
  error("'main' must be public static void");
}

Class*
Parser::matchClass()
{
  auto className = matchIdentifier();
  auto clazz = Scope::global()->lookupClass(className);

  if (clazz == nullptr)
    report("Undefined class '%s'", className.c_str());
  return clazz;
}

void
Parser::classDeclaration()
{
  advance(); // CLASS

  auto className = matchIdentifier();
  Class* superClass = nullptr;

  if (lToken->name == EXTENDS)
  {
    advance();
    superClass = matchClass();
  }

  Class* clazz = Scope::global()->addClass(className, superClass);

  if (!clazz)
    report("Class '%s' already defined", className.c_str());

  auto scope = currentScope;

  currentScope = clazz;
  if (lToken->name != SEP_LBRACE)
    error("'{' expected");
  advance();
  while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
    varDeclaration();
  while (lToken->name == PUBLIC)
    methodDeclaration(clazz);
  match(SEP_RBRACE);
  currentScope = scope;
}

void
Parser::varDeclaration()
{
  auto varType = type();
  auto varName = matchIdentifier();

  if (lToken->name != SEP_SEMICOLON)
    error("';' expected");
  advance();
  if (currentScope && !currentScope->addVariable(varName, varType))
    report("Variable '%s' already defined", varName.c_str());
}

void
Parser::methodDeclaration(Class* clazz)
{
  advance(); // PUBLIC

  auto returnType = type();
  auto methodName = matchIdentifier();
  Method* method{};

  if (clazz)
  {
    if ((method = clazz->addMethod(methodName, returnType, clazz)))
      currentScope = method->scope();
    else
      report("Method '%s' already defined", methodName.c_str());
  }
  if (lToken->name != SEP_LPAREN)
    error("'(' expected");
  advance();
  if (lToken->name != SEP_RPAREN)
    params(method);
  match(SEP_RPAREN);
  if (lToken->name != SEP_LBRACE)
    error("'{' expected");
  advance();
  for (auto isStmt = true; isStmt;)
    switch (lToken->name)
    {
      case INT:
      case BOOLEAN:
        varDeclaration();
        break;
      case SEP_LBRACE:
      case IF:
      case WHILE:
      case SYSTEM_OUT_PRINTLN:
        statement(clazz);
        break;
      case ID:
      {
        auto la = lookahead(lToken);

        if (la->name == SEP_LBRACKET || la->name == OP_ASSIGN)
          statement(clazz);
        else if (la->name == ID)
          varDeclaration();
        else
          error("Syntax error");
        break;
      }
      default:
        isStmt = false;
    }
  if (lToken->name != RETURN)
    error("'return' expected");
  advance();
  tryCast(expression(clazz), returnType);
  if (lToken->name != SEP_SEMICOLON)
    error("';' expected");
  advance();
  match(SEP_RBRACE);
  currentScope = clazz;
}

void
Parser::params(Method* method)
{
  for (;;)
  {
    auto paramType = type();
    auto paramName = matchIdentifier();

    if (method && !method->addParameter(paramName, paramType))
      report("Parameter '%s' already defined", paramName.c_str());
    if (lToken->name != SEP_COMMA)
      return;
    advance();
  }
}

const Type*
Parser::type()
{
  if (lToken->name == INT)
  {
    advance();
    if (lToken->name != SEP_LBRACKET)
      return Type::Int();
    advance();
    match(SEP_RBRACKET);
    return Type::IntArray();
  }
  if (lToken->name == BOOLEAN)
  {
    advance();
    return Type::Boolean();
  }
  if (auto clazz = matchClass())
    return clazz;
  return Type::error();
}

bool
Parser::tryCast(const Type* from, const Type* to)
{
  if (from->canCastTo(to))
    return true;
  report("Cannot convert from %s to %s", from->name(), to->name());
  return false;
}

void
Parser::statement(const Class* clazz)
{
  if (lToken->name == SEP_LBRACE)
  {
    auto scope = currentScope;

    advance();
    if (scope)
      currentScope = new BlockScope{scope, clazz};
    while (lToken->name != SEP_RBRACE)
      statement(clazz);
    advance();
    currentScope = scope;
    return;
  }
  if (lToken->name == IF)
  {
    advance();
    if (lToken->name != SEP_LPAREN)
      error("'(' expected");
    advance();
    expression(clazz);
    match(SEP_RPAREN);
    statement(clazz);
    if (lToken->name != ELSE)
      error("'else' expected");
    advance();
    return statement(clazz);
  }
  if (lToken->name == WHILE)
  {
    advance();
    if (lToken->name != SEP_LPAREN)
      error("'(' expected");
    advance();
    expression(clazz);
    match(SEP_RPAREN);
    return statement(clazz);
  }
  if (lToken->name == SYSTEM_OUT_PRINTLN)
  {
    advance();
    if (lToken->name != SEP_LPAREN)
      error("'(' expected");
    advance();
    expression(clazz);
    match(SEP_RPAREN);
    if (lToken->name != SEP_SEMICOLON)
      error("';' expected");
    return advance();
  }
  if (lToken->name == ID)
  {
    auto varName = lToken->lexeme;
    Variable* var{};
    
    if (!currentScope || !(var = currentScope->lookupVariable(varName)))
      report("Variable '%s' undefined", varName.c_str());
    advance();

    auto varType = var ? var->type() : Type::error();

    if (lToken->name == SEP_LBRACKET)
    {
      tryCast(varType, Type::IntArray());
      advance();
      tryCast(expression(clazz), Type::Int());
      match(SEP_RBRACKET);
      varType = Type::Int();
    }
    if (lToken->name != OP_ASSIGN)
      error("'=' expected");
    advance();
    tryCast(expression(clazz), varType);
    if (lToken->name != SEP_SEMICOLON)
      error("';' expected");
    return advance();
  }
  error("Syntax error");
}

const Type*
Parser::expression(const Class* clazz)
{
  auto e1 = equalityExpression(clazz);

  while (lToken->name == OP_AND)
  {
    advance();

    auto e2 = equalityExpression(clazz);
    auto b1 = tryCast(e1, Type::Boolean());
    auto b2 = tryCast(e2, Type::Boolean());

    e1 = b1 && b2 ? Type::Boolean() : Type::error();
  }
  return e1;
}

const Type*
Parser::equalityExpression(const Class* clazz)
{
  auto e1 = logicalExpression(clazz);

  for (;;)
  {
    switch (lToken->name)
    {
      case OP_EQ:
      case OP_NE:
        break;
      default:
        return e1;
    }
    advance();

    auto e2 = logicalExpression(clazz);

    e1 = tryCast(e2, e1) && e2 != Type::error() ? Type::Boolean() :
      Type::error();
  }
}

const Type*
Parser::logicalExpression(const Class* clazz)
{
  auto e1 = arithmeticExpression(clazz);

  for (;;)
  {
    switch (lToken->name)
    {
      case OP_LT:
      case OP_GT:
        break;
      default:
        return e1;
    }
    advance();

    auto e2 = arithmeticExpression(clazz);
    auto b1 = tryCast(e1, Type::Int());
    auto b2 = tryCast(e2, Type::Int());

    e1 = b1 && b2 ? Type::Boolean() : Type::error();
  }
}

const Type*
Parser::arithmeticExpression(const Class* clazz)
{
  auto e1 = unaryExpression(clazz);

  for (;;)
  {
    switch (lToken->name)
    {
      case OP_PLUS:
      case OP_MINUS:
      case OP_MULT:
      case OP_DIV:
        break;
      default:
        return e1;
    }
    advance();

    auto e2 = unaryExpression(clazz);
    auto b1 = tryCast(e1, Type::Int());
    auto b2 = tryCast(e2, Type::Int());

    e1 = b1 && b2 ? Type::Int() : Type::error();
  }
}

const Type*
Parser::unaryExpression(const Class* clazz)
{
  auto opNot = false;

  while (lToken->name == OP_NOT)
    advance(), opNot = true;

  auto e = prefixExpression(clazz);

  if (opNot)
    tryCast(e, Type::Boolean());
  return e;
}

const Type*
Parser::prefixExpression(const Class* clazz)
{
  auto e = primaryExpression(clazz);

  for (;;)
  {
    if (lToken->name == SEP_LBRACKET)
    {
      auto b = tryCast(e, Type::IntArray());

      advance();
      tryCast(expression(clazz), Type::Int());
      match(SEP_RBRACKET);
      e = b ? Type::Int() : Type::error();
    }
    else if (lToken->name == SEP_DOT)
    {
      advance();
      if (lToken->name == LENGTH)
      {
        auto b = tryCast(e, Type::IntArray());

        advance();
        e = b ? Type::Int() : Type::error();
        continue;
      }

      auto name = matchIdentifier();
      Method* method{};
      auto objType = asClass(e);

      e = Type::error();
      if (!objType)
        report("Expression is not an object");
      else if (!(method = objType->lookupMethod(name)))
        report("'%s' is not a method of '%s'", name.c_str(), objType->name());
      else
        e = method->returnType();
      if (lToken->name != SEP_LPAREN)
        error("'(' expected");
      advance();
      if (lToken->name != SEP_RPAREN)
        expressionList(clazz);
      match(SEP_RPAREN);
    }
    else
      break;
  }
  return e;
}

const Type*
Parser::primaryExpression(const Class* clazz)
{
  if (lToken->name == INTEGER_LITERAL)
  {
    advance();
    return Type::Int();
  }
  if (lToken->name == TRUE || lToken->name == FALSE)
  {
    advance();
    return Type::Boolean();
  }
  if (lToken->name == ID)
  {
    auto varName = lToken->lexeme;
    Variable* var{};

    if (!currentScope || !(var = currentScope->lookupVariable(varName)))
      report("Variable '%s' undefined", varName.c_str());
    advance();
    return var ? var->type() : Type::error();
  }
  if (lToken->name == THIS)
  {
    advance();
    return clazz ? clazz : Type::error();
  }
  if (lToken->name == NEW)
  {
    advance();
    if (lToken->name == INT)
    {
      advance();
      if (lToken->name != SEP_LBRACKET)
        error("'[' expected");
      advance();
      if (expression(clazz) != Type::Int())
        error("Array index must be an integer");
      match(SEP_RBRACKET);
      return Type::IntArray();
    }
    clazz = matchClass();
    if (lToken->name != SEP_LPAREN)
      error("'(' expected");
    advance();
    match(SEP_RPAREN);
    return clazz ? clazz : Type::error();
  }
  if (lToken->name == SEP_LPAREN)
  {
    advance();

    auto e = expression(clazz);

    match(SEP_RPAREN);
    return e;
  }
  error("Syntax error");
  return Type::error();
}

void
Parser::expressionList(const Class* clazz)
{
  expression(clazz);
  while (lToken->name == SEP_COMMA)
  {
    advance();
    expression(clazz);
  }
}

void
Parser::report(const char* fmt, va_list args)
{
  constexpr auto maxLen = 1024;
  char msg[maxLen];

  vsnprintf(msg, maxLen, fmt, args);
  ++errorCount;
  printf("Line %d: %s\n", scanner->getLine(), msg);
}

void
Parser::report(const char* fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  report(fmt, args);
}

void
Parser::error(const char* fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  report(fmt, args);
  throw std::runtime_error{""};
}
