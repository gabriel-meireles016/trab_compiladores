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
O analisador sintático é descendente preditivo e percorre o programa fonte, detecta e reporta erros, porém não implementa estratégias de recuperação de erros léxicos e sintáticos. 

## Terceira etapa
Algumas funcionalidades são implementadas para verificação semântica da linguagem MiniJava modificada, simultaneamente à análise sintática e com auxílio da tabela de símbolos. As funcionalidades implementadas são:
1. Ao declarar uma classe, uma variável ou um método, suas informações são
inseridas na tabela de símbolos do escopo adequado;
2. Ao acessar uma variável ou um método, verifica-se se sua declaração existe e
se pode ser acessada a partir do escopo corrente;
3. Ao realizar uma atribuição, verifica-se se o tipo da expressão à direita
é igual ao da esquerda.

Erros apenas semânticos não param a execução do processo de compilação.

A implementação da análise semântica conta com uma hierarquia de classes para símbolos e escopos. Um símbolo é um objeto de um tipo derivado da classe abstrata Symbol. Um símbolo é um objeto compartilhado, isto é, que tem um contador de referência usado para determinar se o objeto não é mais apontado por nenhum ponteiro "esperto" (quando o contador de referência for igual a 0), situação em que o objeto deve ser destruído. Todo símbolo tem um nome dado por uma string. Classes concretas derivadas de Symbol são:
- Type: representa o tipo de uma váriável, retorno ou parâmetro de função;
- Class: derivada de Type, representa uma classe definida no programa. Uma classe conhece a sua superclasse. Class também deriva de Scope, isto é, uma classe também é um escopo. Como tal, uma classe mantém duas tabelas de símbolos: uma para as variáveis e outra para os métodos definidos em seu corpo;
- Variable: representa uma variável ou parâmetro de função. Toda variável conhece seu tipo (objeto da classe Type);
- Method: representa um método de uma classe. Um método conhece seu tipo de retorno (objeto da classe Type), sua lista de paraâmetros (objetos do tipo Variable) e a classe em que foi definido (objeto do tipo Class). Um método mantém ainda um escopo de bloco (objeto da classe BlockScope, descrita abaixo). O escopo de bloco do método contém uma tabela de símbolos de todas as variáveis declaradas em seu corpo (obejtos da classe Variable), incluíndo seus parâmetros.

Um escopo é um objeto de um tipo derivado da classe abstrata Scope. Um escopo também é um objeto compartilhado. Todo escopo mantém um ponteiro para seu escopo pai, que pode ser nulo. Cada tipo específico de escopo pode manter uma ou mais tabelas de símbolos. Uma tabela de símbolos é um std::map de um tipo específico de símbolo, isto é, há tabelas de símbolos de classes, variáveis e métodos. As classes concretas derivadas de Scope são: 
- GlobalScope: representa o escopo global do programa. Este não tem escopo pai e mantém uma tabela de símbolos com as classes definidas no programa;
- Class: escopo que contém as tabelas de símbolos de variáveis e métodos definidos em uma classe, como descrito acima. O escopo pai de uma classe é sua superclasse ou o escopo global, se esta for nula;
- BlockScope: mantém a tabela de símbolos das variáveis declaradas em um bloco de um método. O escopo pai de um escopo de bloco é o escopo de bloco externo. Se o bloco for o corpo de um método, seu escopo pai é a classe na qual ele foi definido. 
  
## Compilação e execução
O programa utiliza recursos de C++20, por isso deve ser compilado com a seguinte linha de comando:
```
g++ *.cpp -std=c++20 -o mj_compiler
```
Para executar o programa:
```
./mj_compiler teste.mj
```
