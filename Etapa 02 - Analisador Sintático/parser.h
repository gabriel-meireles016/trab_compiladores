#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;

		void advance();
		void match(int);
		
	public:
		Parser(string);
		void run();
        void program();
        void mainClass();
		void classDeclaration();
		void methodDeclaration();
		void params();
		void varDeclaration();
		void statement();
		void expression();
		void type();
		void op();
        //Continuar....
        
        void error(string);
};