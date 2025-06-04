#include "parser.h"

int main(int argc, char *argv[])
{
    // Verifica se foi executado corretamente
    // Essa main espera receber o nome do arquivo a ser
    // executado na linha de comando.
    if (argc != 2)
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }

    //string input;

    // getline(cin, input);
    // parte antiga da etapa 01 que chamava o scanner e retornava todos os tokens
    /* Scanner *scanner = new Scanner(argv[1]);

    Token *t;

    
    do
    {
        t = scanner->nextToken();

        cout << t->name << " ";
    } while (t->name != END_OF_FILE);

    delete scanner; */

    Parser* parser = new Parser(argv[1]);

    parser->run();

    return 0;
}