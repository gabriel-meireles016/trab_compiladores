# Trabalho de Compiladores
O trabalho final de Compiladores consiste em implementar um compilador para a linguagem de programação MiniJava com algumas alterações. O desenvolvimento do compilador é dividido em três etapas: implementar um analisador léxico, um analisador sintático e algumas funcionalidades para verificação semântica da linguagem.

## Primeira etapa
O analisador léxico reconhece e retorna os seguintes tokens, que posteriormente serão símbolos terminais da gramática:
1. Identificadores (ID): Uma sequência de letras, dígitos e underscores, começando sempre
com uma letra. A linguagem distingue letras maiúsculas e minúsculas;
2. Números inteiros (INTEGER_LITERAL): uma sequência de dígitos decimais, que denotam
o número inteiro correspondente;
3. Operadores (OP): &&, <, >, +, −, ∗, /, =, ==, ! = e !;
4. Separadores (SEP): (, ), [, ], {, }, ;, . e ,;
5. Palavras reservadas: boolean, class, else, extends, false, if, int, length,
main, new, public, return, static, String, System.out.println, this, true,
void e while;

## Segunda etapa
O analisador sintático é descendente preditivo e percorre o programa fonte, detecta e reporta erros, porém não implementa estratégias de recuperação de erros. 

## Terceira etapa
Algumas funcionalidades são implementadas para verificação semântica da linguagem MiniJava modificada, simultaneamente à análise sintática e com auxílio da tabela de símbolos. As funcionalidades implementadas são:
1. Ao declarar uma classe, uma variável ou um método, suas informações são
inseridas na tabela de símbolos do escopo adequado;
2. Ao acessar uma variável ou um método, verifica-se se sua declaração existe e
se pode ser acessada a partir do escopo corrente;
3. Ao realizar uma atribuição, verifica-se se o tipo da expressão à direita
é igual ao da esquerda.

Erros apenas semânticos não param a execução do processo de compilação. 

## Compilação e execução
Para compilar o programa:
```
g++ *.cpp -std=c++20 -o mj_compiler
```
Para executar o programa:
```
./mj_compiler teste.mj
```
