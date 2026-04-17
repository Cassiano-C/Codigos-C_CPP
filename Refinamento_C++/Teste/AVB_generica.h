#ifdef __AVB_generica_H
#define __AVB_generica_H

#include <cstdio>

using CompFunc = int (*)(void*, void*); // Tipo de função de comparação
using InterFunc = void (*)(void*); // Tipo de função de iteração

int compareInt(void *a, void *b) {
    return ((int)a) < ((int)b);
}

void printInt(void *a) {
    printf("%d ", (int)a);
}

// Nós da árvore AVL genérica
struct AVBNode
{
    void *chave;
    AVBNode *esq;
    AVBNode *dir;

    AVBNode(void *chave){
        this->chave = chave;
        esq = nullptr;
        dir = nullptr;
    }

    ~AVBNode(){
        delete esq;
        delete dir;
    }
};

// Estrutura da árvore AVL genérica
struct AVBTree
{
    Compfunc comp; // Função de comparação
    AVBNode *raiz;
    int nodeCount;

    AVBTree(){
        raiz = nullptr;
        nodeCount = 0;
    }

    ~AVBTree(){
        delete raiz;
    }

    void add(void *chave);
    bool contains(void *chave);
    void iterate(interFunc func);

};

inline void AVBTree::add(void *chave){

}


#endif // __AVB_generica_H