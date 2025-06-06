#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
	//TODO
    mainClass();
}

//Continuar....

void
Parser::mainClass()
{
    if (lToken->name == CLASS)
	{
		advance();
		if (lToken->name = ID)
		{
			advance();
			if (lToken->name == SEP_LBRACE)
			{
				advance();
				if (lToken->name == PUBLIC)
				{
					advance();
					if (lToken->name == STATIC)
					{
						advance();
						if (lToken->name == VOID)
						{
							advance();
							if (lToken->name == MAIN)
							{
								advance();
								if (lToken->name == SEP_LPAREN)
								{
									advance();
									if (lToken->name == STRING)
									{
										advance();
										if (lToken->name == SEP_LBRACKET)
										{
											advance();
											match(SEP_RBRACKET);
											if (lToken->name == ID)
											{
												advance();
												match(SEP_RPAREN);
												if (lToken->name == SEP_LBRACE)
												{
													advance();
													statement();
													match(SEP_RBRACE);
													match(SEP_RBRACE);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		error("mainClass errado");
	}
}

void
Parser::varDeclaration()
{
	type();
	if (lToken->name == ID)
	{
		advance();
		if (lToken->name == SEP_SEMICOLON)
		{
			advance();
		}
	}
}

void
Parser::op()
{
	if (lToken->name == OP_AND)
	{
		advance();
	}
	else if (lToken->name == OP_LT)
	{
		advance();
	}
	else if(lToken->name == OP_BT)
	{
		advance();
	}
	else if(lToken->name == OP_EQ)
	{
		advance();
	}
	else if(lToken->name == OP_NE)
	{
		advance();
	}
	else if(lToken->name == OP_PLUS)
	{
		advance();
	}
	else if(lToken->name == OP_MINUS)
	{
		advance();
	}
	else if(lToken->name == OP_MULT)
	{
		advance();
	}
	else if (lToken->name == OP_DIV)
	{
		advance();
	}
	else
	{
		error("Operação não existente");
	}
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
