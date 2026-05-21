#include "Jacobi.h"

#define N_theads_per_block 256

template <size_t N>
inline void 
rodar_testes(Jacobi& Jacobi,int threadsPerBlock,std::string nome_arquivo)
{
    Resposta resposta_normal, resposta_parallel;
    auto A = new double[N][N];
    auto b = new double[N];
    auto x_n = new double[N];
    auto x_p = new double[N];

    Jacobi.gerar_matriz_N<N>(A, b, x_n, x_p, N);
    resposta_normal = Jacobi.jacobi_normal_N<N>(A, b, x_n);
    resposta_parallel = Jacobi.Controle_Jacobi_CUDA<N, threadsPerBlock>(A, b, x_p);

    Jacobi.cria_arquivo(resposta_normal, resposta_parallel, N, nome_arquivo);

    delete[] A;
    delete[] b;
    delete[] x_n;
    delete[] x_p;
}

int
main()
{
    Jacobi jacobi;
    rodar_testes<250>(jacobi, N_threads_per_block, "resultados_jacobi_250.txt"); // Teste para N=250
    rodar_testes<500>(jacobi, N_threads_per_block, "resultados_jacobi_500.txt"); // Teste para N=500
    rodar_testes<1000>(jacobi, N_threads_per_block, "resultados_jacobi_1000.txt"); // Teste para N=1000
    rodar_testes<1500>(jacobi, N_threads_per_block, "resultados_jacobi_1500.txt"); // Teste para N=1500
    return 0;
}