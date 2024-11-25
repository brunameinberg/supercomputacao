# Questão 5 – Fundamentos de Paralelismo

## Parte A – Modelo de Memória Compartilhada e Memória Distribuída
### Modelo de Memória Compartilhada

#### Descrição:
- Todos os processadores compartilham um único espaço de memória acessível.
- Os threads executando tarefas podem acessar e modificar os mesmos dados na memória.

**Prós:**
- Comunicação rápida, pois os dados estão em uma memória comum.
- Simples de programar, já que não é necessário enviar e receber mensagens.
- Excelente para máquinas com múltiplos núcleos (como CPUs modernas).

**Contras:**
- Não escalável para um grande número de processadores, devido à contenção na memória compartilhada.
- Difícil de gerenciar conflitos de acesso (condições de corrida).

**Bibliotecas:**
- OpenMP: Usa o modelo de memória compartilhada.
- Thrust: Também pode usar memória compartilhada em GPUs, dependendo da configuração.

### Modelo de Memória Distribuída
#### Descrição:
- Cada processador tem sua própria memória local.
- Os dados são compartilhados entre processadores por meio de troca de mensagens.

**Prós:**
- Altamente escalável, adequado para clusters de computadores e supercomputadores.
- Evita problemas de contenção de memória.

**Contras:**
- A programação é mais complexa, pois exige gerenciamento explícito de comunicação.
- Comunicação entre nós pode ser lenta, dependendo da rede.

**Bibliotecas:**
- MPI: Usa o modelo de memória distribuída.

Comparação dos Modelos
Característica	Memória Compartilhada	Memória Distribuída
Comunicação	Interna à memória	Via troca de mensagens
Escalabilidade	Limitada (número de núcleos)	Muito escalável
Programação	Mais simples	Mais complexa
Exemplo de uso	Multithreading em CPU	Computação em clusters
Bibliotecas	OpenMP, Thrust	MPI

## Parte B – Escalonamento Dinâmico em OpenMP
### O que é escalonamento dinâmico?
No OpenMP, escalonamento dinâmico refere-se à alocação das iterações de um loop às threads de maneira dinâmica, conforme as threads vão ficando disponíveis.
Cada thread solicita novas tarefas (iterações do loop) à medida que termina sua tarefa atual.
É especificado com o modificador schedule(dynamic, chunk_size):

```cpp
#pragma omp parallel for schedule(dynamic, 10)
for (int i = 0; i < N; i++) {
    // Cálculo pesado
}
``` 

Vantagens do escalonamento dinâmico
Adaptação à variabilidade de carga:

Em aplicações onde o tempo de execução de cada iteração do loop varia significativamente, o escalonamento dinâmico distribui a carga de forma equilibrada.
Exemplo: Processamento de dados onde alguns elementos demandam mais processamento que outros.
Redução de ociosidade:

Threads que terminam suas tarefas rapidamente recebem novas iterações, evitando que fiquem ociosas enquanto outras ainda estão ocupadas.
Melhora no balanceamento de carga:

Evita que algumas threads fiquem sobrecarregadas enquanto outras processam tarefas leves.
Quando usar escalonamento dinâmico?
Em casos de variabilidade de carga, como:
Processamento de imagens com regiões que exigem mais computação.
Cálculos baseados em listas heterogêneas, onde diferentes elementos têm complexidade distinta.
Algoritmos de simulação onde a carga é imprevisível.
Exemplo Prático:

```cpp
#include <iostream>
#include <omp.h>

int main() {
    const int N = 20;
    int results[N];

    #pragma omp parallel for schedule(dynamic, 2)
    for (int i = 0; i < N; i++) {
        // Simula carga variável
        int work = i % 5 + 1;
        #pragma omp critical
        std::cout << "Thread " << omp_get_thread_num() << " processando i=" << i 
                  << " com carga " << work << std::endl;
        results[i] = work; // Trabalho fictício
    }

    return 0;
}
```

Comparação com Escalonamento Estático
Característica	Estático	Dinâmico
Alocação	Pré-determinada (fixa)	Feita conforme necessário
Balanceamento de carga	Ruim se as tarefas variarem	Bom para cargas heterogêneas
Sobrecarga	Menor	Maior
Exemplo de uso	Tarefas homogêneas	Tarefas de duração imprevisível
Com isso, você pode destacar os conceitos essenciais de paralelismo na questão!