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
    Token* tok;
    string lexeme;

    while (isspace(input[pos]))
        pos++;
    
    if (input[pos] == '\0')
    {
        tok = new Token(END_OF_FILE);
        
    }
    else if(input[pos] == '"')
    {
        pos++; // pula a aspa inicial
        while (input[pos] != '"')
        {
            if (input[pos] == '\n')
                line++;
            lexeme += input[pos];
            pos++;
        }
        pos++; // pula a aspa final
        tok = new Token(STRINGCONST, lexeme);
        
    }
    else if(input[pos] == '\'')
    {
        pos++; // pula a aspa inicial
        if (input[pos] == '\\') // caractere de escape
        {
            lexeme += input[pos];
            pos++;
            if(input[pos] == '\'') lexicalError("Caractere constante mal formado");
            lexeme += input[pos];
            pos++;
        }
        else
        {
            lexeme += input[pos]; // adiciona o caractere normal
            pos++;
        }
        if (input[pos] != '\'')
        {
            lexicalError("Caractere constante mal formado");
        }
        pos++; // pula a aspa final
        tok = new Token(CHARCONST, lexeme);
        
    }
    else if(input[pos] == '/')
    {
        if(input[pos + 1] == '/')
        {
            while (input[pos] != '\n')
                pos++;
            line++;
        }
        else if(input[pos + 1] == '*')
        {
            pos += 2; // vai pra posição do primeiro caractere após /*
            while (input[pos] != '*' || input[pos + 1] != '/')
            {
                if (input[pos] == '\n')
                    line++;
                pos++;
            }
            pos += 2; // vai pra posição do primeiro caractere após */
        }
        else
        {
            tok = new Token(OPERATOR, DIVIDE);
            pos++;
            
        }
    }
    else if(isdigit(input[pos]))
    {
        while (isdigit(input[pos]))
        {
            lexeme += input[pos];
            pos++;
        }
        tok = new Token(INTCONST, lexeme);
        
    }
    else if(isalpha(input[pos]))
    {
        while (isalpha(input[pos]) || isalnum(input[pos]) || input[pos] == '_')
        {
            lexeme += input[pos];
            pos++;
        }
        tok = new Token(IDENTIFIER, lexeme);
        
    }
    else if(input[pos] == '+')
    {
        tok = new Token(OPERATOR, PLUS);
        pos++;
        
    }
    else if(input[pos] == '-')
    {
        tok = new Token(OPERATOR, MINUS);
        pos++;
        
    }
    else if(input[pos] == '*')
    {
        tok = new Token(OPERATOR, MULTIPLY);
        pos++;
        
    }
    else if(input[pos] == '=')
    {
        if(input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, EQUALS_EQUALS);
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, EQUALS);
            pos++;
        }
        
    }
    else if(input[pos] == '!')
    {
        if(input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, NOT_EQUALS);
            pos += 2;
            
        }
    }
    else if(input[pos] == '<')
    {
        if(input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, LESS_EQUALS);
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, LESS);
            pos++;
        }
        
    }
    else if(input[pos] == '>')
    {
        if(input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, GREATER_EQUALS);
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, GREATER);
            pos++;
        }
        
    }
    else if(input[pos] == '&' && input[pos + 1] == '&')
    {
        tok = new Token(OPERATOR, AND);
        pos += 2;
        
    }
    else if(input[pos] == '|' && input[pos + 1] == '|')
    {
        tok = new Token(OPERATOR, OR);
        pos += 2;
        
    }
    else if(input[pos] == '!')
    {
        tok = new Token(OPERATOR, NOT);
        pos++;
        
    }
    else if(input[pos] == '(')
    {
        tok = new Token(LPAREN);
        pos++;
        
    }
    else if(input[pos] == ')')
    {
        tok = new Token(RPAREN);
        pos++;
        
    }
    else if(input[pos] == '{')
    {
        tok = new Token(LBRACE);
        pos++;
        
    }
    else if(input[pos] == '}')
    {
        tok = new Token(RBRACE);
        pos++;
        
    }
    else if(input[pos] == '[')
    {
        tok = new Token(LBRACKET);
        pos++;
        
    }
    else if(input[pos] == ']')
    {
        tok = new Token(RBRACKET);
        pos++;
        
    }
    else if(input[pos] == ',')
    {
        tok = new Token(COMMA);
        pos++;
        
    }
    else if(input[pos] == ';')
    {
        tok = new Token(SEMICOLON);
        pos++;
        
    }
    else
    {
        lexicalError("Caractere inválido: " + string(1, input[pos]));
    }

    return tok;
 
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
