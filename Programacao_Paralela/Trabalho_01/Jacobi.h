#ifndef JACOBI_H
#define JACOBI_H

#include <iostream>
#include <cmath>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include "Resposta.h"

class Jacobi
{
private:
    // Valor da tolerancia do erro = 0,000001
    static constexpr double TOL = 1e-6; // Tolerância para convergência
    static constexpr int MAX_ITER = 20000; // Número máximo de iterações
public:

template <size_t N>
inline Resposta
jacobi_normal_N(double A[N][N], double b[N], double x[N])
{
    double x_new[N];
    double error = TOL + 1; // Inicializar o error para entrar no loop
    Resposta resposta; // Criar uma instância de Resposta para armazenar o resultado

    while (error > TOL && resposta.iter < MAX_ITER)
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
        resposta.iter++; // Incrementar o contador de iterações
    }
    resposta.error = error;
    return resposta;
}


template <size_t N>
inline Resposta
jacobi_parallel_N(double A[N][N], double b[N], double x[N])
{
    double x_new[N];
    double error = TOL + 1; // Inicializar o erro para entrar no loop
    Resposta resposta; // Criar uma instância de Resposta para armazenar o resultado

    while (error > TOL && resposta.iter < MAX_ITER)
    {
        error = 0.0; // Inicializar o erro para a iteração atual
        #pragma omp parallel for schedule(static) // Paralelizar o loop externo para calcular x_new
        for (int i = 0; i < N; i++)
        {
            double sum = 0.0;
            //#pragma omp parallel for reduction(+:sum) schedule(static) // sum é uma variável compartilhada/global, então precisamos de redução
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
        resposta.iter++; // Incrementar o contador de iterações
        // #pragma omp barrier Nao é necessario, pois quando se usa o #pragma omp parallel for, o programa já espera que todas as threads terminem antes de continuar para a próxima linha de código.
    }
    resposta.error = error;
    return resposta;
}

template <size_t N>
inline void 
gerar_matriz_N(double A[N][N], double b[N], double x_n[N], double x_p[N], int n)
{
    // O uso de static aqui garante que a semente seja definida uma única vez
    static bool seed_definida = false;
    if (!seed_definida) {
        srand(time(NULL));
        seed_definida = true;
    }

    for (int i = 0; i < N; i++) 
    {
        x_n[i] = 0.0;
        x_p[i] = 0.0;
        b[i] = static_cast<double>(rand() % n);
        
        double soma_linha = 0.0;
        // Geramos a linha
        for (int j = 0; j < N; j++) 
        {
            if (i != j) {
                A[i][j] = static_cast<double>(rand() % n);
                soma_linha += std::abs(A[i][j]);
            }
        }
        // Garantimos a dominância diagonal: A[i][i] > soma dos outros elementos
        A[i][i] = soma_linha + (rand() % 10 + 1);
    }
}

inline void
print(Resposta resposta_normal, Resposta resposta_parallel, int N)
{
    double diferenca = resposta_normal.tempo - resposta_parallel.tempo;
    double aceleracao = resposta_normal.tempo / resposta_parallel.tempo;
    printf("\n==========================================================\n");
    printf("           RESULTADOS DO MÉTODO DE JACOBI (N=%d)          \n", N);
    printf("==========================================================\n");
    printf(" %-26s | %-20s \n", "MÉTRICA", "VALOR");
    printf("---------------------------|------------------------------\n");
    printf(" %-25s | %d\n", "Quantidade de Threads", omp_get_max_threads());
    printf(" %-25s | %.6f seg\n", "Tempo Sequencial", resposta_normal.tempo);
    printf(" %-25s | %.6f seg\n", "Tempo Paralelo", resposta_parallel.tempo);
    printf("---------------------------|------------------------------\n");
    printf(" %-25s | %.6f \n", "Erro Sequencial", resposta_normal.error);
    printf(" %-25s | %.6f \n", "Erro Paralelo", resposta_parallel.error);
    printf("---------------------------|------------------------------\n");
    printf(" %-27s | %d \n", "Iterações Sequencial", resposta_normal.iter);
    printf(" %-27s | %d \n", "Iterações Paralelo", resposta_parallel.iter);
    printf("---------------------------|------------------------------\n");
    printf(" %-26s | %.6f seg\n", "Ganho de Tempo (Líquido)", diferenca);
    printf(" %-27s | %.2fx\n", "Speedup (Aceleração)", aceleracao);
    printf(" %-27s | %.2f%%\n", "Eficiência por Núcleo", (aceleracao / omp_get_max_threads()) * 100);
    printf("==========================================================\n\n");
}

};

#endif /* JACOBI_H */