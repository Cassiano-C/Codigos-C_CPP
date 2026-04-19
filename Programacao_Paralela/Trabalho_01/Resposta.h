#ifndef resposta_h
#define resposta_h

#include <stdio.h>

class Resposta
{
public:
    double error;
    int iter;

    Resposta() : error(0.0), iter(0) {}
};

#endif /* resposta_h */