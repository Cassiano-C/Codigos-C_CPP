#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

//#define N 500 // tamanho da matriz
#define N 1000 // tamanho da matriz
#define MAX_ITER 1000 // número máximo de iterações
#define TOL 1e-6 // tolerância para convergência

//                  matriz A,       vetor b,    vetor x (solução inicial)
void 
jacobi_normal(double A[N][N], double b[N], double x[N])
{
    double x_new[N];
    double error = TOL + 1; // Inicializar o error para entrar no loop

    while (error > TOL) 
    {
        error = 0.0; // Inicializar o error para a iteração atual
        for (int i = 0; i < N; i++) 
        {
            double sum = 0.0;
            for (int j = 0; j < N; j++) 
            {
                if (j != i) {
                    sum += A[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / A[i][i];
        }
        // Verificar convergência
        for (int i = 0; i < N; i++) 
        {
            error += fabs(x_new[i] - x[i]);
            x[i] = x_new[i]; // Atualizar x para a próxima iteração
        }
        if (error < TOL) 
        {
            break; // Convergência alcançada
        }
    }
}

void
jacobi_parallel(double A[N][N], double b[N], double x[N])
{
    double x_new[N];
    double error = TOL + 1; // Inicializar o erro para entrar no loop

    while (error > TOL)
    {
        error = 0.0; // Inicializar o erro para a iteração atual
        #pragma omp parallel for schedule(static) // Garantir que as iterações sejam feitas em paralelo
        for (int i = 0; i < N; i++)
        {
            double sum = 0.0; // soma é uma variável local, então não precisa de redução
            for (int j = 0; j < N; j++) 
            {
                if (j != i) 
                {
                    sum += A[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / A[i][i];
        }
        // Verificar convergência
        #pragma omp parallel for reduction(+:error) schedule(static) // error é uma variável compartilhada/global, então precisamos de redução
        for (int i = 0; i < N; i++)
        {
            error += fabs(x_new[i] - x[i]);
            x[i] = x_new[i]; // Atualizar x para a próxima iteração
        }
    }
}

void
gerar_matriz(double A[N][N], double b[N], double x_n[N], double x_p[N], int n) 
{
    srand(time(NULL));
    for (int i = 0; i < N; i++) 
    {
        x_n[i] = 0;
        x_p[i] = 0;
        b[i] = (double)(rand() % n); // Gerar valores aleatórios para b
        
        double soma_linha = 0.0;
        #pragma omp parallel for reduction(+:soma_linha)
        for (int j = 0; j < N; j++) 
        {
            A[i][j] = (double)(rand() % n); // Gerar valores aleatórios para A
            soma_linha += A[i][j];
        }
        A[i][i] = soma_linha + (double)(rand() % 10 + 1); // Garantir que a diagonal seja dominante
    }
}

void
imprimir_matriz(double A[N][N], double b[N]) 
{
    printf("Matriz A:\n");
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("Vetor b:\n");
    for (int i = 0; i < N; i++) 
    {
        printf("%f ", b[i]);
    }
    printf("\n");
}

int main() {
    /*
    ===================================================
    Quantidade de threads: 8

    Solução: 
    Tempo de execução: 53.257108 segundos

    Solução paralela: 
    Tempo de execução paralelo: 15.693042 segundos

    Diferença de tempo: 37.564066 segundos

    Aceleração: 3.393676
    ==================================================
    Quantidade de threads: 6

    Solução: 
    Tempo de execução: 52.737585 segundos

    Solução paralela: 
    Tempo de execução paralelo: 15.103762 segundos

    Diferença de tempo: 37.633823 segundos

    Aceleração: 3.491685
    ==================================================
    Quantidade de threads: 4

    Solução: 
    Tempo de execução: 52.327178 segundos

    Solução paralela: 
    Tempo de execução paralelo: 13.696685 segundos

    Diferença de tempo: 38.630492 segundos

    Aceleração: 3.820426
    =================================================
    Quantidade de threads: 2

    Solução: 
    Tempo de execução: 54.252791 segundos

    Solução paralela: 
    Tempo de execução paralelo: 27.634950 segundos

    Diferença de tempo: 26.617841 segundos

    Aceleração: 1.963195
    =================================================
    */
    omp_set_num_threads(4); // Ajuste o número de threads conforme necessário


    // variáveis para medir o tempo de execução
    double inicio_n, fim_n, inicio_p, fim_p;
    double A[N][N];
    double b[N], x_n[N], x_p[N];

    gerar_matriz(A, b, x_n, x_p, 1000);

    inicio_n = omp_get_wtime();
    jacobi_normal(A, b, x_n);
    fim_n = omp_get_wtime();

    inicio_p = omp_get_wtime();
    jacobi_parallel(A, b, x_p);
    fim_p = omp_get_wtime();

    printf("Quantidade de threads: %d\n\n", omp_get_max_threads());

    printf("Solução: \n");
    printf("Tempo de execução: %f segundos\n\n", fim_n - inicio_n);
    
    printf("Solução paralela: \n");
    printf("Tempo de execução paralelo: %f segundos\n\n", fim_p - inicio_p);

    // se der diferença de tempo negativa, o paralelo foi mais rápido
    printf("Diferença de tempo: %f segundos\n\n", (fim_n - inicio_n) - (fim_p - inicio_p));

    printf("Aceleração: %f\n\n", (fim_n - inicio_n) / (fim_p - inicio_p));

    //imprimir_matriz(A, b);
    return 0;
}