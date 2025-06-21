#ifndef __Scanner_h
#define __Scanner_h

#include "token.h"
#include <fstream>

class Scanner: public mjc::SharedObject
{
private:
  string input; // Armazena o texto de entrada
  int pos{}; // Posição atual
  int line{1};

public:
  // Construtor
  Scanner(const string&);

  int getLine();

  // Método que retorna o próximo token da entrada
  Token* nextToken();

  // Método para manipular erros
  void lexicalError(const string&);
};

using ScannerPtr = mjc::ObjectPtr<Scanner>;

#endif // __Scanner_h
