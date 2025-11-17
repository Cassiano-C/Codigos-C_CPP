#include <stdio.h>
#include <stdlib.h>

struct celula
{
    int chave;
    struct celula *prox;
};

//Funçao q vai inserir ordenadamente na lista
void insere_ordenado(int n, celula* &lst){
    celula *novo, *p, *ant = NULL;

    novo = (celula*) malloc(sizeof(celula));
    novo->chave = n;
    novo->prox = NULL;

    if(lst == NULL)
        lst = novo;
    else{
        p = lst;
        while(p != NULL && p->chave < n){
            ant = p;
            p = p->prox;
        }

        novo->prox = p;
        if (ant != NULL){
            ant->prox = novo;
        }else{
            lst = novo;
        }
    }
}

/*Funcao que apaga todos os nos da lista L*/
void desalocar(celula* &L)
{
    celula *aux;

    while( L != NULL)
    {
        aux = L;

        L = aux->prox;

        free(aux);
    }
}


/*Funcao imprime o campo chave armazenado em cada no da lista L*/
void imprimir(celula *lst)
{
    celula *p;
    for(p = lst; p != NULL; p = p->prox)
        printf("%d ", p->chave);
}


int main()
{
    celula *lista = NULL;

    int num, N;

    scanf(" %d",&N);
    for(int i=0;i<N;i++){
        scanf(" %d",&num);
        insere_ordenado(num,lista);
    }
    
    imprimir(lista);
    desalocar(lista);
    return 0;
}