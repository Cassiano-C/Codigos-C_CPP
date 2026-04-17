#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Aux 
{
    PLUS,//0
    MINUS,//1
    MULT,//2
    DIV,//3
    LPAREN,//4
    RPAREN,//5
    ID,//6
    NUMBER,//7,
    UNDEF,//8
    END_OF_FILE//9
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
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};