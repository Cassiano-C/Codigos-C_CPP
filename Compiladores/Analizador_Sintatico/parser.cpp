#include "parser.h"

Parser::Parser(string input)
{
    scanner = new Scanner(input);
    advance();
}

void Parser::run()
{
    gool();
    match(END_OF_FILE);
    cout << "Análise sintática concluída com sucesso!" << endl;
}

void Parser::advance()
{
    lTocken = scanner->nextToken();
}

void Parser::match(int expected)
{
    if (lTocken->name == expected)
        advance();
    else
        error("Token inesperado: " + lTocken->lexeme);
}

void Parser::error(string message)
{
    cout << "Erro sintático: " << message << endl;
    exit(EXIT_FAILURE);
}


void Parser::gool()
{
    E();
}

void Parser::E()
{
    T();
    E1();
}


void Parser::E1()
{
    if (lTocken->name == PLUS)
    {
        advance();
        T();
        E1();
    }
    else if (lTocken->name == MINUS)
    {
        advance();
        T();
        E1();
    }
}

void Parser::T()
{
    F();
    T1();
}

void Parser::T1()
{
    if (lTocken->name == MULT)
    {
        advance();
        F();
        T1();
    }
    else if (lTocken->name == DIV)
    {
        advance();
        F();
        T1();
    }
}

void Parser::F()
{
    if (lTocken->name == NUMBER)
    {
        advance();
    }
    else if (lTocken->name == ID)
    {
        advance();
    }
    else if (lTocken->name == LPAREN)
    {
        advance();
        E();
        match(RPAREN);
    }
    else
        error("Token inesperado: " + lTocken->lexeme);
}