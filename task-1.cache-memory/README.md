# Task 1 - Cache Memory

> Implemente duas versões da multiplicação de matriz por vetor (MxV) em C: uma com acesso à matriz por linhas (linha externa, coluna interna) e outra por colunas (coluna externa, linha interna). Meça o tempo de execução de cada versão com uma função apropriada e execute testes com diferentes tamanhos de matriz. Identifique a partir de que tamanho os tempos passam a divergir significativamente e explique por que isso ocorre, relacionando suas observações com o uso da memória cache e o padrão de acesso à memória.

## Sistema

![Macbook Air M2, 2022](image.png)

```bash
$ nproc
> 8

$ sysctl -a | grep cache
# ...
> hw.perflevel1.l1icachesize: 131072
> hw.perflevel1.l1dcachesize: 65536
> hw.perflevel1.l2cachesize: 4194304
> hw.perflevel0.l1icachesize: 196608
> hw.perflevel0.l1dcachesize: 131072
> hw.perflevel0.l2cachesize: 16777216
# ...

# Para linux
$ lscpu | grep -i cache
# ...
> L1d cache: ...
> L1i cache: ...
> L2 cache:  ...
> L3 cache:  ...
# ...
```

- **Cache L1 (nível 1):**  
  - **Instruções (perflevel0 - alto desempenho):** 192 KB (196608 bytes)  
  - **Dados (perflevel0 - alto desempenho):** 128 KB (131072 bytes)  
  - **Instruções (perflevel1 - eficiência):** 128 KB (131072 bytes)  
  - **Dados (perflevel1 - eficiência):** 64 KB (65536 bytes)  

- **Cache L2 (nível 2, compartilhado):**  
  - **Perflevel0 (alto desempenho):** 16 MB (16777216 bytes)  
  - **Perflevel1 (eficiência):** 4 MB (4194304 bytes)  

Ou seja, o **M2 Air** tem um cache L1 padrão de **192 KB + 128 KB por núcleo (x8)** e um cache L2 compartilhado de **16 MB para os núcleos de alto desempenho e 4 MB para os núcleos de eficiência**.

## Build

```bash
gcc-14 ./task-1.cache-memory/dinamic_matrix_vector_mult.c -o ./task-1.cache-memory/out/dinamic_matrix_vector_mult.o
```

## Execution

```bash
./task-1.cache-memory/out/dinamic_matrix_vector_mult.o
./task-1.cache-memory/out/static_matrix_vector_mult.o

```

## Output Example - Dinamic

```bash
💡 Matrix-vector multiplication with SIZE=20000.

> ✅ Memory allocation success!

- 🎯 Execution time (row-major): 1.326828 seconds
- 🎯 Execution time (column-major): 4.125764 seconds

> Results match! ✅
```

