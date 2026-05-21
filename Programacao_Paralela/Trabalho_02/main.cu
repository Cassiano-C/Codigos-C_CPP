#include "Jacobi.h"

#define N_theads_64 64
#define N_theads_128 128
#define N_theads_256 256
#define N_theads_512 512
#define N_theads_1024 1024

template <size_t N, int threadsPerBlock>
inline void 
rodar_testes(Jacobi& Jacobi,std::string nome_arquivo)
{
    Resposta resposta_normal, resposta_parallel;
    auto A = new double[N][N];
    auto b = new double[N];
    auto x_n = new double[N];
    auto x_p = new double[N];

    Jacobi.gerar_matriz_N<N>(A, b, x_n, x_p, N);
    resposta_normal = Jacobi.jacobi_normal_N<N>(A, b, x_n);
    resposta_parallel = Jacobi.Controle_Jacobi_CUDA<N, threadsPerBlock>(A, b, x_p);

    Jacobi.cria_arquivo(resposta_normal, resposta_parallel, N, threadsPerBlock, nome_arquivo);

    delete[] A;
    delete[] b;
    delete[] x_n;
    delete[] x_p;
}

int
main()
{
    Jacobi jacobi;
    rodar_testes<250, N_theads_64>(jacobi, "resultados_jacobi_250.txt"); // Teste para N=250
    rodar_testes<500, N_theads_64>(jacobi, "resultados_jacobi_500.txt"); // Teste para N=500
    rodar_testes<1000, N_theads_64>(jacobi, "resultados_jacobi_1000.txt"); // Teste para N=1000
    rodar_testes<1500, N_theads_64>(jacobi, "resultados_jacobi_1500.txt"); // Teste para N=1500

    printf("Fim dos testes para threads por bloco = %d\n", N_theads_64);

    rodar_testes<250, N_theads_128>(jacobi, "resultados_jacobi_250.txt"); // Teste para N=250
    rodar_testes<500, N_theads_128>(jacobi, "resultados_jacobi_500.txt"); // Teste para N=500
    rodar_testes<1000, N_theads_128>(jacobi, "resultados_jacobi_1000.txt"); // Teste para N=1000
    rodar_testes<1500, N_theads_128>(jacobi, "resultados_jacobi_1500.txt"); // Teste para N=1500

    printf("Fim dos testes para threads por bloco = %d\n", N_theads_128);

    rodar_testes<250, N_theads_256>(jacobi, "resultados_jacobi_250.txt"); // Teste para N=250
    rodar_testes<500, N_theads_256>(jacobi, "resultados_jacobi_500.txt"); // Teste para N=500
    rodar_testes<1000, N_theads_256>(jacobi, "resultados_jacobi_1000.txt"); // Teste para N=1000
    rodar_testes<1500, N_theads_256>(jacobi, "resultados_jacobi_1500.txt"); // Teste para N=1500

    printf("Fim dos testes para threads por bloco = %d\n", N_theads_256);

    rodar_testes<250, N_theads_512>(jacobi, "resultados_jacobi_250.txt"); // Teste para N=250
    rodar_testes<500, N_theads_512>(jacobi, "resultados_jacobi_500.txt"); // Teste para N=500
    rodar_testes<1000, N_theads_512>(jacobi, "resultados_jacobi_1000.txt"); // Teste para N=1000
    rodar_testes<1500, N_theads_512>(jacobi, "resultados_jacobi_1500.txt"); // Teste para N=1500

    printf("Fim dos testes para threads por bloco = %d\n", N_theads_512);

    rodar_testes<250, N_theads_1024>(jacobi, "resultados_jacobi_250.txt"); // Teste para N=250
    rodar_testes<500, N_theads_1024>(jacobi, "resultados_jacobi_500.txt"); // Teste para N=500
    rodar_testes<1000, N_theads_1024>(jacobi, "resultados_jacobi_1000.txt"); // Teste para N=1000
    rodar_testes<1500, N_theads_1024>(jacobi, "resultados_jacobi_1500.txt"); // Teste para N=1500

    printf("Fim dos testes para threads por bloco = %d\n", N_theads_1024);

    return 0;
}