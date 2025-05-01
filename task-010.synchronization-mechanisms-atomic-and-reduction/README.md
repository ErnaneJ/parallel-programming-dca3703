# 🧮 Monte Carlo π Estimation with OpenMP

Este repositório contém um experimento de paralelização usando OpenMP para estimar o valor de π via o método de Monte Carlo, comparando diferentes mecanismos de sincronização: `critical`, `atomic` e `reduction`. O objetivo foi avaliar o desempenho e a aplicabilidade de cada abordagem em um ambiente com múltiplos núcleos.

## 🚀 Descrição

O código implementa cinco variações do algoritmo de Monte Carlo para estimar π:

| Versão | Descrição |
|--------|-----------|
| 🟢 **v1** | `#pragma omp critical` com contador local |
| 🔵 **v2** | `#pragma omp atomic` com contador local |
| 🟣 **v3** | Cláusula `reduction(+:count)` |
| 🔻 **v4** | `#pragma omp atomic` a cada ponto aceito (alta contenção) |
| 🔻 **v5** | `#pragma omp critical` a cada ponto aceito (altíssima contenção) |

Todas as versões utilizam geradores de números aleatórios independentes por thread via `rand_r()`.

## ⚙️ Ambiente de Execução

- **Máquina**: MacBook Air M2
- **Núcleos disponíveis**: 8 (`nproc = 8`)
- **Compilador**: `gcc-14` com suporte a OpenMP
- **Sistema Operacional**: macOS (via `brew install gcc`)
- **Tamanho da simulação**: `N = 999999999` pontos

## 🧪 Execução

Compile e execute com:

```bash
gcc-14 -fopenmp ./task-10.synchronization-mechanisms-atomic-and-reduction/main.c -o ./task-10.synchronization-mechanisms-atomic-and-reduction/out/main.o && ./task-10.synchronization-mechanisms-atomic-and-reduction/out/main.o
```

## 📈 Resultados da Última Execução

```text
== Monte Carlo π Estimation with OpenMP ==

🟢 [v1] critical + local count + rand_r():        π ≈ 3.141576695141577 | Time: 3.098s  
🔵 [v2] atomic + local count + rand_r():          π ≈ 3.141576695141577 | Time: 3.095s  
🟣 [v3] reduction(+:count) + rand_r():            π ≈ 3.141576695141577 | Time: 3.048s  
🔻 [v4] worst: atomic per hit + rand_r():         π ≈ 3.141631403141631 | Time: 34.350s  
🔻 [v5] worst: critical per hit + rand_r():       π ≈ 3.141676511141676 | Time: 60.818s  
```

## 📌 Conclusões

- `reduction` se mostrou a abordagem mais simples e eficiente.
- `atomic` com contadores locais tem desempenho muito próximo ao `reduction`, sendo uma boa alternativa quando a operação não é suportada por `reduction`.
- `critical` só deve ser usado quando necessário, e nunca dentro de laços com alta frequência de acesso.
- Sincronizações por acesso (`v4` e `v5`) geram gargalos severos de desempenho.
