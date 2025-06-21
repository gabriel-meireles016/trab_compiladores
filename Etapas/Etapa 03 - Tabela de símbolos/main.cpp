#include "parser.h"

int main(int argc, char *argv[])
{
    // Verifica se foi executado corretamente
    // Essa main espera receber o nome do arquivo a ser
    // executado na linha de comando.
    if (argc != 2)
    {
        cout << "Usage: ./mj_compiler filename.mj\n";
        return 1;
    }

    Parser* parser = new Parser(argv[1]);

    try
    {
      parser->run();
    }
    catch (...)
    {
      // do nothing
    }
    delete parser;
#ifdef _MSC_VER
    puts("Press any key to exit...");
    (void)getchar();
#endif // _MSC_VER
    return 0;
}
