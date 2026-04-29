#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,// 0
    IDENTIFIER, // 1
    INTCONST, // 2
    CHARCONST, // 3
    STRINGCONST, // 4
    OPERATOR, // 5
    PLUS, // 6
    MINUS, // 7
    MULTIPLY, // 8
    DIVIDE, // 9
    EQUALS, // 10
    EQUALS_EQUALS, // 11
    NOT_EQUALS, // 12
    LESS, // 13
    LESS_EQUALS, // 14
    GREATER, // 15
    GREATER_EQUALS, // 16
    AND, // 17
    OR, // 18
    NOT, // 19
    LPAREN, // 20
    RPAREN, // 21
    LBRACE, // 22
    RBRACE, // 23
    LBRACKET, // 24
    RBRACKET, // 25
    COMMA, // 26
    SEMICOLON, // 27
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
        
        string nameToString() {
            static const string names[] = {
                "UNDEF", "IDENTIFIER", "INTCONST", "CHARCONST", "STRINGCONST",
                "OPERATOR", "PLUS", "MINUS", "MULTIPLY", "DIVIDE",
                "EQUALS", "EQUALS_EQUALS", "NOT_EQUALS", "LESS", "LESS_EQUALS",
                "GREATER", "GREATER_EQUALS", "AND", "OR", "NOT",
                "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET",
                "COMMA", "SEMICOLON", "END_OF_FILE"
            };
            if (name >= 0 && name <= END_OF_FILE)
                return names[name];
            return "UNKNOWN";
        }
};
