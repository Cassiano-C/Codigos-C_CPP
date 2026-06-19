#ifndef JACOBI_H
#define JACOBI_H

#include "Resposta.h"
#include <mpi.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

template <size_t N>
class Jacobi
{
private:
    const double TOL = 1e-6;    // Tolerância para convergência
    const int MAX_ITER = 10000; // Número máximo de iterações
    Resposta resposta_normal;
    Resposta resposta_parallel;

public:
    int rank = 0, size = 1; // Valores padrão seguros

    // CORRIGIDO: alterado de Resposta para void
    void jacobi_normal_N(double A[N][N], double b[N], double x[N])
    {
        double x_new[N];
        double error = TOL + 1;

        resposta_normal.iter = 0; // CORRIGIDO: Garante reinicialização

        clock_t start, end;
        start = clock();

        while (error > TOL && resposta_normal.iter < MAX_ITER)
        {
            error = 0.0;
            for (size_t i = 0; i < N; i++)
            {
                double sum = 0.0;
                for (size_t j = 0; j < N; j++)
                {
                    if (j != i)
                    {
                        sum += A[i][j] * x[j];
                    }
                }
                x_new[i] = (b[i] - sum) / A[i][i];
            }

            // Verificar convergência
            for (size_t i = 0; i < N; i++)
            {
                error += std::abs(x_new[i] - x[i]);
                x[i] = x_new[i];
            }
            resposta_normal.iter++;
        }
        end = clock();

        resposta_normal.tempo = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        resposta_normal.error = error;
    }

    void jacobi_mpi_N(double A[N][N], double b[N], double x[N])
    {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        resposta_parallel.iter = 0; // CORRIGIDO: Garante reinicialização

        // 1. Descobrir quantas linhas ESSE processo específico vai tratar
        int resto = N % size; // Para pegar os dados uniformemente para poder distribuir para os processor
        int linhas_locais = (rank < resto) ? (N / size) + 1 : (N / size);

        // 2. Alocação dinâmica para evitar estouro de memória
        std::vector<double> b_local(linhas_locais);
        std::vector<double> x_new_local(linhas_locais);
        std::vector<double> A_local_flat(linhas_locais * N);

        // 3. Criar os contadores específicos para a Matriz e para o Vetor
        std::vector<int> sendcounts_A(size), displs_A(size);
        std::vector<int> sendcounts_v(size), displs_v(size);

        int offset_A = 0;
        int offset_v = 0;
        for (int i = 0; i < size; i++)
        {
            int r_linhas = (i < resto) ? (N / size) + 1 : (N / size);

            // Definindo quais dados do vetor B cada processo vai enxergar
            sendcounts_v[i] = r_linhas;
            displs_v[i] = offset_v;

            // Definindo quais dados do vetor A cada processo vai enxergar
            sendcounts_A[i] = r_linhas * N;
            displs_A[i] = offset_A;

            // Atualizado os valores
            offset_v += r_linhas;
            offset_A += r_linhas * N;
        }

        int start_global = displs_v[rank];

        // 4. Distribuição dos dados (Scatters)
        MPI_Scatterv(&A[0][0], sendcounts_A.data(), displs_A.data(), MPI_DOUBLE, A_local_flat.data(), linhas_locais * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(b, sendcounts_v.data(), displs_v.data(), MPI_DOUBLE, b_local.data(), linhas_locais, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double start_time = MPI_Wtime();
        double error = TOL + 1;

        while (error > TOL && resposta_parallel.iter < MAX_ITER)
        {
            double local_error = 0.0;

            for (int i = 0; i < linhas_locais; i++)
            {
                int global_i = start_global + i;
                double sum = 0.0;

                for (size_t j = 0; j < N; j++)
                {
                    if (j != global_i)
                    {
                        sum += A_local_flat[i * N + j] * x[j];
                    }
                }
                double diag = A_local_flat[i * N + global_i];
                x_new_local[i] = (b_local[i] - sum) / diag;
            }

            for (int i = 0; i < linhas_locais; i++)
            {
                local_error += std::abs(x_new_local[i] - x[start_global + i]);
            }

            MPI_Allreduce(&local_error, &error, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            MPI_Allgatherv(x_new_local.data(), linhas_locais, MPI_DOUBLE, x, sendcounts_v.data(), displs_v.data(), MPI_DOUBLE, MPI_COMM_WORLD);

            resposta_parallel.iter++;
        }

        double end_time = MPI_Wtime();
        resposta_parallel.tempo = end_time - start_time;
        resposta_parallel.error = error;
    }

    void gerar_matriz_N(double A[N][N], double b[N], double x_n[N], double x_p[N], int n)
    {
        static bool seed_definida = false;
        if (!seed_definida)
        {
            srand(time(NULL) + rank); // Adicionado + rank para evitar sementes idênticas caso usem em paralelo
            seed_definida = true;
        }

        for (size_t i = 0; i < N; i++)
        {
            x_n[i] = 0.0;
            x_p[i] = 0.0;
            b[i] = static_cast<double>(rand() % n);

            double soma_linha = 0.0;
            for (size_t j = 0; j < N; j++)
            {
                if (i != j)
                {
                    A[i][j] = static_cast<double>(rand() % n);
                    soma_linha += std::abs(A[i][j]);
                }
            }
            A[i][i] = soma_linha + (rand() % 10 + 1);
        }
    }

    void salvar_para_csv(int tamanho_N, int quantidade_processos, const std::string &nome_arquivo_csv)
    {
        // CORRIGIDO: Garante que a variável 'rank' esteja atualizada antes de checar
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        // CORRIGIDO: APENAS o processo principal (Rank 0) escreve no arquivo!
        if (rank != 0)
        {
            return;
        }

        bool arquivo_existe = false;
        if (FILE *f = fopen(nome_arquivo_csv.c_str(), "r"))
        {
            arquivo_existe = true;
            fclose(f);
        }

        FILE *arquivo = fopen(nome_arquivo_csv.c_str(), "a");
        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo CSV.\n");
            return;
        }

        if (!arquivo_existe)
        {
            fprintf(arquivo, "N,QuantidadeProcessos,TempoSequencial,TempoMPI,ErroSequencial,ErroMPI,IteracoesSequencial,IteracoesMPI,Speedup\n");
        }

        double speedup = (resposta_parallel.tempo > 0) ? (resposta_normal.tempo / resposta_parallel.tempo) : 0.0;

        fprintf(arquivo, "%d,%d,%.6f,%.6f,%.6f,%.6f,%d,%d,%.2f\n",
                tamanho_N,
                quantidade_processos,
                resposta_normal.tempo,
                resposta_parallel.tempo,
                resposta_normal.error,
                resposta_parallel.error,
                resposta_normal.iter,
                resposta_parallel.iter,
                speedup);

        fclose(arquivo);
    }
};

#endif /* JACOBI_H */