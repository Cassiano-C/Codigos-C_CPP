# Programação Paralela

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenMP](https://img.shields.io/badge/OpenMP-blue?style=for-the-badge)

Nessa pasta ira conter alguns codigos em C/C++ focados em programação paralela e no uso do OpenMp.

# Tabela de Arquivos

| Pastas | Descrição | Técnica |
| :--- | :---: | ---: |
| **teste.c** | Primeiro arquivo de paralelismo | `#pragma omp parallel for`|
| **Trabalhos_01** | Resolução de sistemas lineares | `#pragma omp parallel for` |

## Trabalho 01

Focado na resolução do **Método Iterativo de Jacobi**, usando metodo sem paralelismo e com paralelismo.

### Ideia Geral
O método de Jacobi resolve o sistema linear de forma iterativa. O procedimento consiste em:
1. escolher um vetor solução inicial (por exemplo, todos os valores iguais a zero);
2. calcular sucessivas aproximações da solução;
3. interromper o processo quando a solução convergir.

### Como executar

Para compilar o código com suporte a multiprocessamento, utilize a flag `-fopenmp` e para definir a quantidade de threads é no codigo bem no inicio da `main()`:

```bash
# Compilação
gcc -fopenmp Jacobi.c -o main

# Execução
./main
```
