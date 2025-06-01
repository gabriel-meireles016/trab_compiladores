#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    // Sequência de letras, dígitos e underscores
    ID,
    // Sequência de dígitos decimais
    INTEGER_LITERAL,
    // &&, <, >, +, −, ∗, /, =, ==, ! = e !;
    OP_AND, OP_LT, OP_BT, OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_ASSIGN, OP_EQ, OP_NE, OP_NOT,
    //  (, ), [, ], {, }, ;, . e ,;
    SEP_LPAREN, SEP_RPAREN, SEP_LBRACKET, SEP_RBRACKET, SEP_LBRACE, SEP_RBRACE, SEP_SEMICOLON, SEP_DOT, SEP_COMMA,
    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
