#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    // inicializa o token como ponteiro vazio/nulo
    Token* tok = nullptr;
    string lexeme;

    // ignorando espaços em branco e comentários
    while (pos < input.length()) {
        if (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n') {
            if (input[pos] == '\n') {
                line++;
                pos++;
                continue;
            }
            pos++;
            continue;
        }

        // comentário só da linha (//)
        if (input[pos] == '/' && (pos + 1) < input.length() && input[pos + 1] == '/') {
            while (pos < input.length() && input[pos] != '\n') {
                pos++;
            }

            if (pos < input.length()) {
                line++;
                continue;
            }
        }
        
        // comentário de bloco /**/
        if (input[pos] == '/' && (pos + 1) < input.length() && input[pos + 1] == '*') {
            pos += 2;
            
            // loop continua enquanto não encontrar o fechamento do bloco 
            while (pos < input.length() && !(input[pos] == '*' && (pos + 1) < input.length() && input[pos + 1] == '/')) {
                if (input[pos] == '\n') {
                    line++;
                    pos++;
                }
            }
            
            if (pos >= input.length()) {
                lexicalError("Comentário de bloco não foi fechado");
            }
            
            pos += 2;
            continue;
        }
        break;
    }
    
    // acaba o arquivo
    if (pos >= input.length()) {
        return new Token(END_OF_FILE);
    }
    
    // identifica token
    char current = input[pos];

    // IDs e palavras reservadas

    if (isalpha(current)) {
        lexeme += current;
        pos++;

        while (pos < input.length() && (isalpha(input[pos]) || input[pos] == '_')) {
            lexeme += input[pos];
            pos++;
        }
        
        // verifica se é uma palavra reservada
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
        else if (lexeme == "System"){
            bool isSOP = false;
            string sop = lexeme;

            if (pos < input.length() && input[pos] == '.') {
                sop += '.';
                pos++;

                // verificando se tem out
                if (pos + 2 < input.length() && input.substr(pos, 3) == "out") { //pega 3 caracteres de pos (pos + 2) e input.substr avança pos em 3 se for válido.
                    sop += 'out';
                    pos += 3;

                    // verificando .
                    if (pos < input.length() && input[pos] == '.') {
                        sop += '.';
                        pos++;

                        // verificando println
                        if (pos + 6 < input.length() && input.substr(pos, 7) == "println") {
                            sop += 'println';
                            pos += 7;
                            isSOP = true;
                            tok = new Token(SYSTEM_OUT_PRINTLN, sop);
                        }
                    }   
                }   
            }

            if (!isSOP) {
                lexicalError("'" + lexeme + "' deve ser escrito acompanhado de '.out.println'");
            }
            
        }
        else if (lexeme == "this") tok = new Token(THIS, lexeme);
        else if (lexeme == "true") tok = new Token(TRUE, lexeme);
        else if (lexeme == "void") tok = new Token(VOID, lexeme);
        else if (lexeme == "while") tok = new Token(WHILE, lexeme);
        // se não, é um id
        else tok = new Token(ID, lexeme);
    }
    
    // numeros inteiros
    else if (isdigit(current)) {
        lexeme += current;
        pos++;
        
        while (pos < input.length() && isdigit(input[pos])) {
            lexeme += input[pos];
            pos++;
        }

        tok = new Token(INTEGER_LITERAL, lexeme);
    }

    // operadores/separadores
    else {
        switch (current) {
            case '&':
                if (pos + 1 < input.length() && input[pos + 1] == '&') {
                    tok = new Token(OP_AND, "&&");
                    pos += 2;
                } else {
                    lexicalError("Operador '&' inválido.");
                }
                break;
            case '=':
                if (pos + 1 < input.length() && input[pos + 1] == '=') {
                    tok = new Token(OP_EQ, "==");
                    pos += 2;
                } else {
                    tok = new Token(OP_ASSIGN, "=");
                    pos++;
                }
                break;
            case '!':
                if (pos + 1 < input.length() && input[pos+1] == '=') {
                    tok = new Token(OP_NE, "!=");
                    pos += 2;
                } else {
                    tok = new Token(OP_NOT, "!");
                    pos++;
                }
                break;
            case '<':
                if (pos < input.length()) {
                    tok = new Token(OP_LT, "<");
                    pos++;
                }
            case '>':
                if (pos < input.length()) {
                    tok = new Token(OP_BT, ">");
                    pos++;
                }
            case '+':
                if (pos < input.length()) {
                    tok = new Token(OP_PLUS, "+");
                    pos++;
                }
            case '-':
                if (pos < input.length()) {
                    tok = new Token(OP_MINUS, "-");
                    pos++;
                }
            case '*':
                if (pos < input.length()) {
                    tok = new Token(OP_MULT, "*");
                    pos++;
                }
            case '/':
                if (pos < input.length()) {
                    tok = new Token(OP_DIV, "/");
                    pos++;
                }
            default:
                lexicalError("Caractere não reconhecido: " + string(1, current));    
        }
    }
    
    return tok;
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
