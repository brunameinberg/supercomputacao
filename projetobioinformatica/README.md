# Relatório Projeto Bioinformática

Duas características desse projeto são a utilização de OpenMP e MPI. A seguir, terão as explicações das escolhas em relação a essas ferramentas.

## Exercício 1

1. #pragma omp parallel for reduction(+:localA, localT, localC, localG):

```cpp
#pragma omp parallel for reduction(+:localA, localT, localC, localG)
```

A escolha foi de paralelizar o loop entre os cores e utilizar a "*flag*" reduction para somar as variáveis deposi das threads.

2. MPI_Scatterv:

```cpp
MPI_Scatterv(buffer.data(), sendCounts.data(), displs.data(), MPI_CHAR, localBuffer.data(), localBufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);

```

Permite distribuir partes do arquivo de DNA entre os processos de forma não uniforme (é bom para garantir que, mesmo que nao seja divisível pelo número de ranks, ele faça a divisão mesmo assim), controlando exatamente quantos dados cada processo recebe.

3. MPI_Reduce:
```cpp
MPI_Reduce(localCount.data(), globalCount.data(), 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
```

Para, depois da contagem, ser reunida em variáveis de cada base, o reduce é ideal pois reduz os resultados em um único valor global.


## Exercício 2

1. #pragma omp parallel
```cpp
#pragma omp parallel
```

Usado para paralelizar o bloco de código que faz a transcrição do DNA para o RNA dentro da função Transcricao().

Como a transcrição de cada base de DNA é independente, podemos paralelizar facilmente o loop que itera sobre os caracteres do buffer (inputBuffer). Isso garante que a operação de transcrição (substituição das bases de DNA por RNA) seja feita de forma eficiente em múltiplos núcleos, acelerando o processo.

2. #pragma omp for nowait
```cpp
#pragma omp for nowait
```

A diretiva #pragma omp for nowait é utilizada para dividir um loop entre várias threads, mas, ao contrário da diretiva padrão #pragma omp for, ela não impõe uma barreira de sincronização após o loop. Ou seja, as threads não precisam esperar umas pelas outras ao final do loop, permitindo que cada thread continue com o trabalho independentemente das outras, assim evitando um possível gargalo.

3. #pragma omp critical

```cpp
#pragma omp critical
```


A diretiva #pragma omp critical é utilizada para garantir que uma região crítica do código seja executada por apenas uma thread de cada vez. No caso do código, a operação crítica é a inserção dos dados transcritos no vetor outputBuffer, que é compartilhado entre todas as threads. Embora o loop de transcrição seja paralelizado, as threads podem tentar modificar o outputBuffer ao mesmo tempo, o que pode levar a condições de corrida. A diretiva critical garante que apenas uma thread modifique o outputBuffer por vez, evitando corrupção de dados e garantindo a integridade do buffer final.


4. MPI_Bcast

```cpp
MPI_Bcast(&totalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(sendCounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(displs.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
```

O comando MPI_Bcast é utilizado para enviar dados de um processo (no caso o rank = 0) para todos os outros processos. No código, ele é usado para distribuir as informações sobre o tamanho total do arquivo (totalSize), os tamanhos dos blocos de dados a serem enviados a cada processo (sendCounts), e os deslocamentos (displs) para cada processo.
Motivo da escolha:
Como o arquivo é grande, e os dados precisam ser distribuídos entre os processos, MPI_Bcast é a maneira mais eficiente de garantir que todos os processos tenham acesso às mesmas informações antes de começar a distribuição dos dados.

5. MPI_Scatterv
```cpp
MPI_Scatterv(buffer.data(), sendCounts.data(), displs.data(), MPI_CHAR, localBuffer.data(), localBufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);
```

O comando MPI_Scatterv é usado para distribuir partes de um array (ou buffer) entre múltiplos processos, de forma não uniforme. No código, isso é utilizado para distribuir o conteúdo do arquivo entre os processos de forma eficiente.

6. MPI_Gather
```cpp
MPI_Gather(&localTranscribedSize, 1, MPI_INT, transcribedSizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
```

O comando MPI_Gather é usado para coletar dados de todos os processos e reunir esses dados em um único processo (rank == 0). No código, ele é usado para coletar os tamanhos dos buffers transcritos de cada processo (transcribedSizes) e também para reunir os buffers transcritos de cada processo no processo raiz.

7. MPI_Gatherv
```cpp
MPI_Gatherv(transcribedBuffer.data(), localTranscribedSize, MPI_CHAR, finalBuffer.data(), transcribedSizes.data(), transcribedDispls.data(), MPI_CHAR, 0, MPI_COMM_WORLD);

```

O comando MPI_Gatherv é uma outra versão do MPI_Gather, permitindo que os processos enviem quantidades de dados diferentes para o rank 0. No código, ele é usado para coletar as partes transcritas do arquivo de cada processo, que podem ter tamanhos diferentes, e armazená-las no processo raiz.


## Exercício 3

1. #pragma omp parallel for reduction(+:count)
```cpp
#pragma omp parallel for reduction(+:count)
```

O comando #pragma omp parallel for reduction(+:count) é utilizado para paralelizar o loop que conta a quantidade de ocorrências do padrão AUG no RNA. A diretiva parallel for divide o trabalho de iteração entre várias threads, permitindo que múltiplos núcleos de processamento da CPU sejam usados para aumentar a performance. A flag(+:count) garante que a variável count, que acumula o número de ocorrências do padrão AUG, seja somada corretamente entre as threads, sem risco de condições de corrida. Cada thread mantém uma cópia local de count, e no final da execução, o valor é somado.


2. MPI_Scatterv
```cpp
MPI_Scatterv(buffer.data(), sendCounts.data(), displs.data(), MPI_CHAR, localBuffer.data(), localBufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);
```
O comando MPI_Scatterv é utilizado para distribuir partes do vetor buffer entre os processos. O vetor sendCounts determina quantos dados cada processo vai receber, e displs define os deslocamentos, ou seja, a partir de qual posição cada processo começa a receber os dados. 

3. MPI_Reduce
```cpp
MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
```

O comando MPI_Reduce combina os resultados de todos os processos em um único valor. No código, ele é usado para somar as ocorrências de AUG de todos os processos em um único valor global, armazenado na variável globalCount.

4. MPI_Bcast
```cpp
MPI_Bcast(sendCounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
```
O comando MPI_Bcast é utilizado para enviar dados do processo raiz para todos os outros processos. No código, ele é usado para enviar o vetor sendCounts, que contém as quantidades de dados que cada processo deve receber, para todos os processos.


5. MPI_Gather
```cpp
MPI_Gather(&localTranscribedSize, 1, MPI_INT, transcribedSizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
```

O comando MPI_Gather é utilizado para coletar dados de todos os processos e reunir esses dados em um único processo (rank == 0). Neste código, ele é utilizado para reunir os tamanhos dos buffers transcritos de cada processo.

6. MPI_Gatherv
```cpp
MPI_Gatherv(transcribedBuffer.data(), localTranscribedSize, MPI_CHAR,
            finalBuffer.data(), transcribedSizes.data(), transcribedDispls.data(), MPI_CHAR, 0, MPI_COMM_WORLD);
```

No código, ele é utilizado para reunir as partes transcritas do arquivo de RNA, que podem ter tamanhos diferentes, e armazená-las no processo raiz.

## Exercício 4

1. #pragma omp parallel for
```cpp

#pragma omp parallel for
for (int i = start; i < start + length - 2; i += 3) {
    string codon = string(1, rnaBuffer[i]) + rnaBuffer[i+1] + rnaBuffer[i+2];
    if (tabelaAminoacidos.find(codon) != tabelaAminoacidos.end()) {
        #pragma omp critical
        proteinBuffer.push_back(tabelaAminoacidos[codon]); 
    }
}
```

O comando #pragma omp parallel for é utilizado para paralelizar o loop que percorre o vetor rnaBuffer e traduz os códons de RNA para aminoácidos. O loop itera sobre o RNA em "passos" de 3 bases, criando um códon de RNA a cada iteração.

2. #pragma omp critical
A operação de adicionar ao vetor proteinBuffer é uma operação crítica, pois o buffer é compartilhado entre as threads.

3. MPI_Bcast
```cpp
MPI_Bcast(&totalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
```

No código, ele é usado para enviar o tamanho total do arquivo rnaBuffer (armazenado na variável totalSize) para todos os processos.


4. MPI_Scatterv
```cpp
MPI_Scatterv(rnaBuffer.data(), sendCounts.data(), displs.data(), MPI_CHAR, localBuffer.data(), sendCounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
```

O comando MPI_Scatterv é utilizado para distribuir os dados do vetor rnaBuffer entre os processos de forma não uniforme, ou seja, os dados podem ser divididos em tamanhos diferentes entre os processos. 

5. MPI_Gather

```cpp
MPI_Gather(&localProteinSize, 1, MPI_INT, proteinSizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD)
```

O comando MPI_Gather é utilizado para coletar os tamanhos dos buffers de proteína transcritos (proteinSizes) de todos os processos e reunir essas informações no processo raiz (rank == 0). O processo raiz será responsável por armazenar as informações de tamanho de todos os buffers.

6. MPI_Gatherv
```cpp
MPI_Gatherv(proteinBuffer.data(), localProteinSize, MPI_CHAR, finalProteinBuffer.data(), proteinSizes.data(), proteinDispls.data(), MPI_CHAR, 0, MPI_COMM_WORLD);

```

No código, ele é utilizado para reunir as partes da proteína transcrita de cada processo, que podem ter tamanhos diferentes, e armazená-las no processo raiz.

