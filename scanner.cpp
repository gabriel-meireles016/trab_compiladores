#include "scanner.h"

Scanner::Scanner(const string& filename)
{
  ifstream inputFile(filename, ios::in);

  if (!inputFile.is_open())
  {
    cout << "Unable to open file " << filename << '\n';
    throw runtime_error{""};
  }

  string line;

  while (getline(inputFile, line))
    input.append(line + '\n');
  inputFile.close();
}

int
Scanner::getLine()
{
  return line;
}

Token*
Scanner::nextToken()
{
  Token* tok = nullptr;
  string lexeme;

  // Ignores whitespaces and comments
  while (pos < input.length())
  {
    if (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n')
    {
      if (input[pos] == '\n')
      {
        line++;
        pos++;
        continue;
      }
      else
      {
        pos++;
        continue;
      }
    }

    // line comments
    if (input[pos] == '/' && (pos + 1) < input.length() && input[pos + 1] == '/')
    {
      while (pos < input.length() && input[pos] != '\n')
      {
        pos++;
      }

      if (pos < input.length())
      {
        line++;
        continue;
      }
    }

    // block comments
    if (input[pos] == '/' && (pos + 1) < input.length() && input[pos + 1] == '*')
    {
      pos += 2;
 
      while (pos < input.length() && !(input[pos] == '*' && (pos + 1) < input.length() && input[pos + 1] == '/'))
      {
        if (input[pos] == '\n')
        {
          line++;
        }
        pos++;
      }

      if (pos >= input.length())
      {
        lexicalError("Block comment without closing");
      }

      pos += 2;
      continue;
    }
    break;
  }

  // end of file
  if (input[pos] == '\0')
  {
    return new Token(END_OF_FILE);
  }

  char current = input[pos];

  if (isalpha(current))
  {
    lexeme += current;
    pos++;

    while (pos < input.length() && (isalpha(input[pos]) || input[pos] == '_'))
    {
      lexeme += input[pos];
      pos++;
    }

    // verifies if variable 'current' is a reserved word
    if (lexeme == "boolean") tok = new Token(BOOLEAN, lexeme);
    else if (lexeme == "class") tok = new Token(CLASS, lexeme);
    else if (lexeme == "else") tok = new Token(ELSE, lexeme);
    else if (lexeme == "extends") tok = new Token(EXTENDS, lexeme);
    else if (lexeme == "false") tok = new Token(FALSE, lexeme);
    else if (lexeme == "if") tok = new Token(IF, lexeme);
    else if (lexeme == "int") tok = new Token(INT, lexeme);
    else if (lexeme == "length") tok = new Token(LENGTH, lexeme);
    else if (lexeme == "main") tok = new Token(MAIN, lexeme);
    else if (lexeme == "new") tok = new Token(NEW, lexeme);
    else if (lexeme == "public") tok = new Token(PUBLIC, lexeme);
    else if (lexeme == "static") tok = new Token(STATIC, lexeme);
    else if (lexeme == "String") tok = new Token(STRING, lexeme);
    else if (lexeme == "return") tok = new Token(RETURN, lexeme);
    else if (lexeme == "System")

    // verifies if System.out.println is written correctly
    {
      bool isSOP = false;
      string sop = lexeme;


      if (pos < input.length() && input[pos] == '.')
      {
        sop += '.';
        pos++;

        if (pos + 2 < input.length() && input.substr(pos, 3) == "out")
        { 
          sop += "out";
          pos += 3;

          if (pos < input.length() && input[pos] == '.')
          {
            sop += '.';
            pos++;

            
            if (pos + 6 < input.length() && input.substr(pos, 7) == "println")
            {
              sop += "println";
              pos += 7;
              isSOP = true;
              tok = new Token(SYSTEM_OUT_PRINTLN, sop);
            }
          }
        }
      }

      if (!isSOP)
      {
        lexicalError("'" + lexeme + "' missing '.out.println'");
      }

    }
    else if (lexeme == "this") tok = new Token(THIS, lexeme);
    else if (lexeme == "true") tok = new Token(TRUE, lexeme);
    else if (lexeme == "void") tok = new Token(VOID, lexeme);
    else if (lexeme == "while") tok = new Token(WHILE, lexeme);
    
    // if none of the above, the token is an ID
    else tok = new Token(ID, lexeme);
  }

  // verifies if 'current' is an integer
  else if (isdigit(current))
  {
    lexeme += current;
    pos++;

    while (pos < input.length() && isdigit(input[pos]))
    {
      lexeme += input[pos];
      pos++;
    }

    tok = new Token(INTEGER_LITERAL, lexeme);
  }

  // verifies if 'current' is an operator
  else
  {
    switch (current)
    {
      case '&':
        if (pos + 1 < input.length() && input[pos + 1] == '&')
        {
          tok = new Token(OP_AND, "&&");
          pos += 2;
        }
        else
        {
          lexicalError("Invalid '&'");
        }
        break;
      case '=':
        if (pos + 1 < input.length() && input[pos + 1] == '=')
        {
          tok = new Token(OP_EQ, "==");
          pos += 2;
        }
        else
        {
          tok = new Token(OP_ASSIGN, "=");
          pos++;
        }
        break;
      case '!':
        if (pos + 1 < input.length() && input[pos + 1] == '=')
        {
          tok = new Token(OP_NE, "!=");
          pos += 2;
        }
        else
        {
          tok = new Token(OP_NOT, "!");
          pos++;
        }
        break;
      case '<':
        if (pos < input.length())
        {
          tok = new Token(OP_LT, "<");
          pos++;
        }
        break;
      case '>':
        if (pos < input.length())
        {
          tok = new Token(OP_GT, ">");
          pos++;
        }
        break;
      case '+':
        if (pos < input.length())
        {
          tok = new Token(OP_PLUS, "+");
          pos++;
        }
        break;
      case '-':
        if (pos < input.length())
        {
          tok = new Token(OP_MINUS, "-");
          pos++;
        }
        break;
      case '*':
        if (pos < input.length())
        {
          tok = new Token(OP_MULT, "*");
          pos++;
        }
        break;
      case '/':
        if (pos < input.length())
        {
          tok = new Token(OP_DIV, "/");
          pos++;
        }
        break;
      case '(':
        if (pos < input.length())
        {
          tok = new Token(SEP_LPAREN, "(");
          pos++;
        }
        break;
      case ')':
        if (pos < input.length())
        {
          tok = new Token(SEP_RPAREN, ")");
          pos++;
        }
        break;
      case '[':
        if (pos < input.length())
        {
          tok = new Token(SEP_LBRACKET, "[");
          pos++;
        }
        break;
      case ']':
        if (pos < input.length())
        {
          tok = new Token(SEP_RBRACKET, "]");
          pos++;
        }
        break;
      case '{':
        if (pos < input.length())
        {
          tok = new Token(SEP_LBRACE, "{");
          pos++;
        }
        break;
      case '}':
        if (pos < input.length())
        {
          tok = new Token(SEP_RBRACE, "}");
          pos++;
        }
        break;
      case ';':
        if (pos < input.length())
        {
          tok = new Token(SEP_SEMICOLON, ";");
          pos++;
        }
        break;
      case '.':
        if (pos < input.length())
        {
          tok = new Token(SEP_DOT, ".");
          pos++;
        }
        break;
      case ',':
        if (pos < input.length())
        {
          tok = new Token(SEP_COMMA, ",");
          pos++;
        }
        break;
      default:
        lexicalError("Unknown character '" + string(1, current) + "'");
    }
  }

  return tok;
}

void
Scanner::lexicalError(const string& msg)
{
  cout << "Line " << line << ": " << msg << endl;
  throw runtime_error{""};
}
