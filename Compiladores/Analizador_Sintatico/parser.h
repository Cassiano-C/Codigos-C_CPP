#include "scanner.h"

class Parser
{
    private:
        Scanner* scanner;
        Token* lTocken;

        void advance();
        void match(int);

        void gool(); //1
        void E(); //2
        void E1(); //3
        void T(); //4
        void T1(); //5
        void F(); //6

        void error(string);

    public:
        Parser(string); 
        void run();

};
