#include "Jacobi.h"

#define N_THREADS 4 // Número de threads para execução paralela

template <size_t N>
void rodar_testes(Jacobi& Jacobi)
{
    Resposta resposta_normal, resposta_parallel;
    double inicio_n, fim_n, inicio_p, fim_p;
    auto A = new double[N][N];
    auto b = new double[N];
    auto x_n = new double[N];
    auto x_p = new double[N];

    Jacobi.gerar_matriz_N<N>(A, b, x_n, x_p, N);

    inicio_n = omp_get_wtime();
    resposta_normal = Jacobi.jacobi_normal_N<N>(A, b, x_n);
    fim_n = omp_get_wtime();

    inicio_p = omp_get_wtime();
    resposta_parallel = Jacobi.jacobi_parallel_N<N>(A, b, x_p);
    fim_p = omp_get_wtime();

    // Cálculos de apoio
    double tempo_n = fim_n - inicio_n;
    double tempo_p = fim_p - inicio_p;
    
    Jacobi.print(tempo_n, tempo_p, resposta_normal, resposta_parallel,N);
}

int
main()
{
    omp_set_num_threads(N_THREADS); // Ajuste o número de threads conforme necessário
    Jacobi jacobi;
    rodar_testes<100>(jacobi); // Teste para N=100
    rodar_testes<500>(jacobi); // Teste para N=500
    rodar_testes<1000>(jacobi); // Teste para N=1000
    return 0;
}