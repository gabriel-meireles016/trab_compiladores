#ifndef __Scanner_h
#define __Scanner_h

#include "token.h"
#include <fstream>

class Scanner: public mjc::SharedObject
{
private:
  string input;
  int pos{};
  int line{1};

public:
  Scanner(const string&);
  int getLine();
  Token* nextToken();
  void lexicalError(const string&);
};

using ScannerPtr = mjc::ObjectPtr<Scanner>;

#endif // __Scanner_h
