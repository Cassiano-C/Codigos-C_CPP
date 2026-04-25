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
    Resposta
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
    Resposta
    jacobi_parallel_N(double A[N][N], double b[N], double x[N])
    {
        double x_new[N];
        double error = TOL + 1; // Inicializar o erro para entrar no loop
        Resposta resposta; // Criar uma instância de Resposta para armazenar o resultado

        while (error > TOL && resposta.iter < MAX_ITER)
        {
            error = 0.0; // Inicializar o erro para a iteração atual
            #pragma omp parallel for schedule(static) // o schule(static) vai dividir o loop pela quantidade de threads disponiveis, e cada thread vai executar uma parte do loop
            /*
                O uso de #pragma omp parallel for é necessário para paralelizar o loop que calcula os novos valores de x (x_new). 
                Isso permite que as iterações do loop sejam distribuídas entre as threads disponíveis, acelerando o processo de cálculo dos novos valores de x. 
                Sem essa diretiva, o loop seria executado sequencialmente, o que não aproveitaria os benefícios da paralelização.

                E usar o schedule(static) é uma escolha comum para loops que têm uma carga de trabalho uniforme, como neste caso, onde cada iteração do loop tem aproximadamente o mesmo tempo de execução.
                onde ele divide o loop em blocos de iterações e atribui cada bloco a uma thread. Isso pode ajudar a reduzir a sobrecarga de gerenciamento de threads e melhorar o desempenho, especialmente quando o número de iterações é grande.
            */
            for (int i = 0; i < N; i++)
            {
                double sum = 0.0;
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
            /*
                O uso de reduction(+:error) é necessário para garantir que a variável error seja atualizada corretamente quando várias threads estão somando seus valores locais de error. 
                Sem a redução, haveria uma condição de corrida, onde múltiplas threads tentariam atualizar a mesma variável error simultaneamente, resultando em um valor incorreto. 
                A redução garante que cada thread tenha sua própria cópia local de error, e no final do loop, os valores locais são somados para obter o valor total de error.
            */ 
            for (int i = 0; i < N; i++)
            {
                error += fabs(x_new[i] - x[i]);
                x[i] = x_new[i]; // Atualizar x para a próxima iteração
            }
            resposta.iter++; // Incrementar o contador de iterações
            /*
                #pragma omp barrier Nao é necessario, pois quando se usa o #pragma omp parallel for, 
                o programa já espera que todas as threads terminem antes de continuar para a próxima linha de código.
            */ 
        }
        resposta.error = error;
        return resposta;
    }

    // Foi feito por uma IA, para gerar a matriz A, o vetor b e os vetores x_n e x_p. A matriz A é gerada de forma a ser diagonalmente dominante, o que garante a convergência do método de Jacobi. O vetor b é preenchido com valores aleatórios, e os vetores x_n e x_p são inicializados com zeros. O uso de static para a semente do rand() garante que a semente seja definida apenas uma vez, evitando que a matriz seja gerada da mesma forma em cada chamada da função.
    template <size_t N>
    void 
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

    void
    print(Resposta resposta_normal, Resposta resposta_parallel, int N)
    {
        double diferenca = resposta_normal.tempo - resposta_parallel.tempo;
        double aceleracao = resposta_normal.tempo / resposta_parallel.tempo;
        printf("\n╔═════════════════════════════════════════════════════════╗\n");
        printf("║          RESULTADOS DO MÉTODO DE JACOBI (N=%4d)        ║\n", N);
        printf("╠═══════════════════════════╤═════════════════════════════╣\n");
        printf("║ %-26s │ %-27s ║\n", "MÉTRICA", "VALOR");
        printf("╠═══════════════════════════╪═════════════════════════════╣\n");
        printf("║ %-25s │ %-27d ║\n", "Quantidade de Threads", omp_get_max_threads());
        printf("║ %-25s │ %-27.6f ║\n", "Tempo Sequencial (s)", resposta_normal.tempo);
        printf("║ %-25s │ %-27.6f ║\n", "Tempo Paralelo (s)", resposta_parallel.tempo);
        printf("╟───────────────────────────┼─────────────────────────────╢\n");
        printf("║ %-25s │ %-27.6f ║\n", "Erro Sequencial", resposta_normal.error);
        printf("║ %-25s │ %-27.6f ║\n", "Erro Paralelo", resposta_parallel.error);
        printf("╟───────────────────────────┼─────────────────────────────╢\n");
        printf("║ %-27s │ %-27d ║\n", "Iterações Sequencial", resposta_normal.iter);
        printf("║ %-27s │ %-27d ║\n", "Iterações Paralelo", resposta_parallel.iter);
        printf("╟───────────────────────────┼─────────────────────────────╢\n");
        printf("║ %-25s │ %-27.6f ║\n", "Ganho de Tempo (s)", diferenca);
        printf("║ %-25s │ %-27.2f ║\n", "Speedup (x)", aceleracao);
        printf("║ %-25s │ %-27.2f ║\n", "Eficiência por Núcleo (%)", (aceleracao / omp_get_max_threads()) * 100);
        printf("╚═══════════════════════════╧═════════════════════════════╝\n\n");
    }

};

#endif /* JACOBI_H */