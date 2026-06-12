#include "Jacobi.h"
#include <string>
#include <vector>

template <size_t N>
inline void rodar_testes(const std::string& nome_arquivo)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Alocação das matrizes globais como static para não estourar a Stack
    static double A[N][N];
    static double b[N];
    static double x_seq[N];
    static double x_mpi[N];

    Jacobi<N> solver;

    // 1. Apenas o rank 0 gera a matriz e roda o sequencial
    if (rank == 0) {
        solver.gerar_matriz_N(A, b, x_seq, x_mpi, 100);
        solver.jacobi_normal_N(A, b, x_seq);
    }

    // 2. TODOS os processos entram no MPI juntos (para fazer os Scatters e Reduções)
    solver.jacobi_mpi_N(A, b, x_mpi);

    // 3. Salvar os resultados (CORRIGIDO: usando a variável nome_arquivo passada por parâmetro)
    solver.salvar_para_csv(N, size, nome_arquivo);
}

int main(int argc, char *argv[])
{
    // Inicializa o MPI apenas UMA vez no início de tudo
    MPI_Init(&argc, &argv);

    const std::string arquivo_unico = "resultados_jacobi.csv";

    // Executa cada tamanho de forma sequencial e limpa.
    // O compilador C++ agora consegue gerar o código correto para cada tamanho.
    rodar_testes<250>(arquivo_unico);
    rodar_testes<500>(arquivo_unico);
    rodar_testes<1000>(arquivo_unico);
    rodar_testes<1500>(arquivo_unico);
    rodar_testes<2000>(arquivo_unico);

    // Finaliza o MPI apenas UMA vez no encerramento do programa
    MPI_Finalize();
    return 0;
}