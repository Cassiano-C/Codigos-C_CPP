#include "scanner.h"    

//Construtor
Scanner::Scanner(string input)
{
    this->buffer = input;
    cout << "Entrada: " << buffer << endl << "Tamanho: " 
         << buffer.length() << endl;
    pos = 0;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    while (isspace(buffer[pos]))
        pos++;
    
    if (buffer[pos] == '\0')
    {
        tok = new Token(END_OF_FILE);
        return tok;
    }
    else if (buffer[pos] == '+')
    {
        lexeme = buffer[pos];
        pos++;
        tok = new Token(PLUS);
    }
    else if (buffer[pos] == '-')
    {
        lexeme = buffer[pos];
        pos++;
        tok = new Token(MINUS);
    }
    else if (buffer[pos] == '*')
    {
        lexeme = buffer[pos];
        pos++;
        tok = new Token(MULT);
    }
    else if (buffer[pos] == '/')
    {
        lexeme = buffer[pos];
        pos++;
        tok = new Token(DIV);
    }
    else if (buffer[pos] == '(')
    {
        lexeme = buffer[pos];
        pos++;
        tok = new Token(LPAREN);
    }
    else if (buffer[pos] == ')')
    {
        lexeme = buffer[pos];
        pos++;
        tok = new Token(RPAREN);
    }
    else if (isdigit(buffer[pos]))
    {
        lexeme.push_back(buffer[pos]);
        pos++;

        while (isdigit(buffer[pos]))
        {
            lexeme.push_back(buffer[pos]);
            pos++;
        }

        tok = new Token(NUMBER);
        tok->lexeme = lexeme;
    }
    else if (isalpha(buffer[pos]) || buffer[pos] == '_')
    {
        lexeme.push_back(buffer[pos]);
        pos++;

        while (isalnum(buffer[pos]) || buffer[pos] == '_')
        {
            lexeme.push_back(buffer[pos]);
            pos++;
        }

        tok = new Token(ID);
        tok->lexeme = lexeme;
    }
    else
    {
        lexicalError();
    }

    return tok;
}

void 
Scanner::lexicalError()
{
    cout << "Token mal formado\n";
    
    exit(EXIT_FAILURE);
}
