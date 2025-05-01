# 🧵 Parallel File Processor with OpenMP

Este projeto demonstra o uso de **OpenMP** para processar, em paralelo, uma **lista encadeada de arquivos fictícios** em linguagem C. Cada nó da lista representa um arquivo e é processado por uma tarefa independente, criada dinamicamente em tempo de execução.

## 💡 Objetivo

Implementar e analisar um programa que:

- Cria uma lista encadeada com nomes de arquivos fictícios;
- Utiliza **OpenMP** para paralelizar o processamento de cada nó com `#pragma omp task`;
- Imprime o nome do arquivo e a thread responsável por processá-lo;
- Avalia possíveis problemas de concorrência, repetição ou perda de nós.

## 🛠️ Como executar

Compile com suporte a OpenMP:

```bash
gcc-14 -fopenmp ./task-7.using-tasks/file_processor.c -o ./task-7.using-tasks/out/file_processor
```

Execute:

```bash
./task-7.using-tasks/out/file_processor
```

> 💡 Certifique-se de que seu compilador e sistema possuem suporte adequado ao OpenMP.

## 📄 Exemplo de saída

```text
File: file0.txt (ptr: 0x125e05a90), processed by thread 1
File: file1.txt (ptr: 0x125e05b00), processed by thread 2
...
File: file9.txt (ptr: 0x125e06140), processed by thread 0
```

## ⚠️ Observações

- A criação de tarefas deve ser feita por apenas uma thread, utilizando `#pragma omp single`, para evitar **condições de corrida**.
- O uso de `firstprivate(node)` garante que cada tarefa tenha sua própria cópia do ponteiro para o nó.
- A ausência de sincronização pode levar a **erros de segmentação** ou **resultados incorretos** (repetição ou perda de tarefas).
- Diferenças no comportamento entre sistemas (ex: Linux vs macOS) foram observadas, destacando a importância da **portabilidade e sincronização correta** em código paralelo.