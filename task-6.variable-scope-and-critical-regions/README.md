# 🔢 Estimativa Estocástica de π com OpenMP

Este projeto apresenta a implementação de um estimador estocástico para o número π utilizando o método de Monte Carlo. A versão sequencial foi paralelizada com OpenMP, abordando problemas clássicos como condições de corrida e controle de escopo de variáveis com diferentes cláusulas (`private`, `firstprivate`, `lastprivate`, `shared`, `default(none)`).

## 📌 Objetivo

Explorar o paralelismo com OpenMP, identificando erros causados por condições de corrida, aplicando boas práticas com controle de escopo e mensurando o impacto de diferentes abordagens na performance e na correção da estimativa de π.

## 🧪 Execução

O projeto foi desenvolvido e testado em um **Macbook Air M2 (8GB RAM, 8 núcleos)**.

A compilação e execução são feitas com o seguinte comando:

```bash
gcc-14 -fopenmp ./task-6.variable-scope-and-critical-regions/main.c -o ./task-6.variable-scope-and-critical-regions/out/main.o && ./task-6.variable-scope-and-critical-regions/out/main.o
```

> ⚙️ Como não foi especificado o número de threads, o OpenMP utiliza automaticamente todos os núcleos disponíveis.

## 🧮 Casos testados

1. ✅ Versão sequencial  
2. ❌ `#pragma omp parallel for` — resultado incorreto devido a condição de corrida  
3. ✅ Correção com `#pragma omp critical`  
4. ✅ Uso de `private` para contagem local  
5. ✅ Uso de `firstprivate` para inicialização local da semente de aleatoriedade  
6. ✅ Uso de `lastprivate` para manter o valor da última iteração  
7. ✅ Aplicação de `default(none)` para controle explícito de escopo

## 📈 Resultados

```text
==> Reference π ≈ 3.141592653589793
==== OpenMP π Estimation Tests ====

✅ Case 1 - Sequential:       π ≈ 3.141877914187791 | Time: 0.193s  
❌ Case 2 - Parallel for:     π ≈ 0.210229621022962 | Time: 0.372s (race condition)  
✅ Case 3 - Critical:         π ≈ 3.142048314204831 | Time: 3.758s (idleness)  
✅ Case 4 - Private:          π ≈ 3.129574312957431 | Time: 0.320s  
✅ Case 5 - Firstprivate:     π ≈ 3.141803514180352 | Time: 0.227s  
✅ Case 6 - Lastprivate:      π ≈ 3.130124713012471 | Time: 0.330s | Last i: 9999998  
✅ Case 7 - Default(none):    π ≈ 3.141803114180311 | Time: 0.225s 
```

## 📚 Conclusão

A experiência reforça a importância de compreender o escopo de variáveis em ambientes paralelos e evidencia como diretivas como `critical` ou cláusulas como `private` e `firstprivate` podem influenciar tanto a correção quanto a performance. Além disso, `default(none)` se mostra uma excelente prática para tornar o código mais robusto e legível.

## 📁 Estrutura do Projeto

```bash
.
├── task-6.variable-scope-and-critical-regions/
│   ├── main.c
│   ├── out/
│   │   └── main.o
│   └── README.md
```

Desenvolvido como parte de um estudo sobre escopo de variáveis e paralelismo com OpenMP.
