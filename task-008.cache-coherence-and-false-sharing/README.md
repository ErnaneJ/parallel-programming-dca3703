# 🧮 Stochastic Pi Estimation (Task 8)

Este projeto implementa e compara diferentes versões paralelas para a estimação estocástica da constante matemática π, explorando o impacto da concorrência, da geração de números aleatórios e de técnicas de paralelização com OpenMP.

## 📄 Descrição

Quatro versões do algoritmo foram implementadas:

- **Version 1 - critical + rand()**  
  Uso de `rand()` com seção crítica (`#pragma omp critical`) para proteger a atualização da variável global.
- **Version 2 - array + rand()**  
  Uso de `rand()` com vetor de acumulação local por thread, evitando seções críticas.
- **Version 3 - critical + rand_r()**  
  Uso de `rand_r()`, gerador de números aleatórios com seed privada por thread, combinado com `#pragma omp critical`.
- **Version 4 - array + rand_r()**  
  Uso de `rand_r()` com vetores locais por thread para máxima eficiência, sem necessidade de seções críticas.

Todas as versões estimam π gerando pontos aleatórios no plano e verificando a razão dos pontos que caem dentro do círculo unitário.

## ⚙️ Como Compilar e Executar

Utilize o `gcc` com suporte a OpenMP.

```bash
gcc-14 -fopenmp ./task-8.cache-coherence-and-false-sharing/main.c -o ./task-8.cache-coherence-and-false-sharing/out/main.o
./task-8.cache-coherence-and-false-sharing/out/main.o
```

> **Observação:** O projeto assume o compilador `gcc-14`. Se necessário, ajuste para a versão disponível no seu sistema (`gcc` ou `gcc-13`, por exemplo).

## 📈 Resultados

A execução gera uma saída semelhante a esta:

```bash
==> Stochastic Pi Estimation (Task 8)

🟡 Version 1 - critical + rand():      π ≈ 3.126013512601351 | Time: 0.073s
🔴 Version 2 - array + rand():         π ≈ 3.133681113368112 | Time: 0.056s
🟢 Version 3 - critical + rand_r():    π ≈ 3.141924714192471 | Time: 0.033s
🌕 Version 4 - array + rand_r():       π ≈ 3.141924714192471 | Time: 0.048s
```

Esses resultados permitem comparar a precisão e o tempo de execução entre as abordagens, destacando o impacto das técnicas de paralelização e geração de números aleatórios.
