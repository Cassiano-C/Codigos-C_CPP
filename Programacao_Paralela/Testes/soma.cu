#include <iostream>
#include <cuda_runtime.h>

// __global__ indica que esta função corre na GPU e é chamada pela CPU
__global__ void somaVetores(float* A, float* B, float* C, int n) {
    // Calcula o índice global desta thread específica
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    
    // Verifica se o índice está dentro do limite do array
    if (i < n) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int n = 1 << 20; // 1 milhão de elementos
    size_t bytes = n * sizeof(float);

    // 1. Alocar memória no "Host" (CPU)
    float *h_A = (float*)malloc(bytes);
    float *h_B = (float*)malloc(bytes);
    float *h_C = (float*)malloc(bytes);

    // Inicializar os dados
    for (int i = 0; i < n; i++) {
        h_A[i] = 1.0f;
        h_B[i] = 2.0f;
    }

    // 2. Alocar memória no "Device" (GPU)
    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    // 3. Copiar dados da CPU para a GPU
    cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice);

    // 4. Configurar a execução (Threads e Blocos)
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Lançar o Kernel
    somaVetores<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);

    // 5. Copiar o resultado de volta para a CPU
    cudaMemcpy(h_C, h_C, bytes, cudaMemcpyDeviceToHost);

    // Verificar um resultado
    std::cout << "Resultado final: " << h_C[0] << " (Esperado: 3.0)" << std::endl;

    // Limpeza
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);

    return 0;
}