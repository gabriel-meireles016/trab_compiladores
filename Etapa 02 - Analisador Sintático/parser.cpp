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
    mainClass();
	while(lToken->name == CLASS) 
	{
		classDeclaration();
	}
	if (lToken->name == EOF) 
	{
		advance();
	} else {
		error("Sem end of file");
	}
}

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
												} else {
													error("Sem chaves");
												}
											} else {
												error("Sem segundo id");
											}
										} else {
											error("Sem colchete esquerdo");
										}
									} else {
										error("Sem string");
									}
								} else {
									error("Sem parenteses esquerdo");
								}
							} else {
								error("Sem main");
							}
						} else {
							error("Sem void");
						}
					} else {
						error("Sem static");
					}
				} else {
					error("Sem public");
				}
			} else {
				error("Sem parênteses esquerdo");
			}
		} else {
			error("Sem ID");
		}
	} else {
		error("mainClass errado");
	}
}

void
Parser::classDeclaration()
{
	if (lToken->name == CLASS)
	{
		advance();
		if (lToken->name == ID)
		{
			advance();
			if (lToken->name != EXTENDS && lToken->name != SEP_LBRACE)
			{
				error("Sem extends e sem chave");
			}
			// CASO DÊ PROBLEMA TALVEZ SEJA BOM COLOCAR UM ELSE
			if (lToken->name == EXTENDS)
			{
				advance();
				if (lToken->name == ID)
				{
					advance();
				} else {
					error("Tem extends mas não tem ID");
				}
			}
			if (lToken->name == SEP_LBRACE)
			{
				advance();
				while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
				{
					varDeclaration();	
				}
				while (lToken->name == PUBLIC)
				{
					methodDeclaration();
				}
				match(SEP_RBRACE);
			}		
		} else {
			error("Sem ID");
		}
	} else {
		error("class declaration errado");
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
		} else {
			error("Sem ponto virgula");
		}
	} else {
		error("Sem ID");
	}
}

void
Parser::methodDeclaration()
{
	if (lToken->name == PUBLIC)
	{
		advance();
		type();
		if (lToken->name == ID)
		{
			advance();
			if (lToken->name == SEP_LPAREN)
			{
				advance();
				if (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
				{
					params();
				}
				match(SEP_RPAREN);
				if (lToken->name == SEP_LBRACE)
				{
					advance();
					while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
					{
						varDeclaration();
					}
					while (lToken->name == SEP_LBRACE || lToken->name == IF || lToken->name == WHILE || lToken->name == SYSTEM_OUT_PRINTLN || lToken->name == ID)
					{
						statement();
					}
					if (lToken->name == RETURN)
					{
						advance();
						expression();
						if (lToken->name == SEP_SEMICOLON)
						{
							advance();
							match(SEP_RBRACE);
						} else {
							error("Sem ponto virgula");
						}
					} else {
						error("Sem return");
					}
				} else {
					error("Sem colchetes");
				}
			} else {
				error("Sem parenteses esquerdo");
			}
		} else {
			error("Sem ID");
		}
	} else {
		error("method declaration errado");
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
