#include <stdio.h>
#include <stdlib.h> // Para rand, srand
#include <time.h>   // Para time
#include <stdbool.h>

// Constantes
#define CAPACIDADE_FILA 5   // Tamanho fixo da fila de peças futuras
#define CAPACIDADE_PILHA 3  // Capacidade máxima da pilha de reserva

// Variáveis globais para rastreio
static int proximoId = 0; // Gerador de ID único para as peças

// --- Estruturas de Dados ---

// 1. Definição da estrutura para uma Peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // ID numérico único
} Peca;

// 2. Definição da estrutura para a Fila Circular de Peças Futuras
typedef struct {
    Peca elementos[CAPACIDADE_FILA];
    int frente;
    int traseira;
    int tamanho;
} FilaPecas;

// 3. Definição da estrutura para a Pilha de Peças Reservadas
typedef struct {
    Peca elementos[CAPACIDADE_PILHA];
    int topo; // Índice do topo (posição do último elemento inserido)
} PilhaReserva;

// --- Funções Auxiliares ---

// Função para gerar uma nova peça automaticamente
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);
    char tipoAleatorio = tipos[rand() % numTipos];

    Peca novaPeca = {
        .nome = tipoAleatorio,
        .id = proximoId++
    };

    return novaPeca;
}

// --- Funções da FILA (Queue) ---

void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->traseira = 0;
    fila->tamanho = 0;

    // Preenche a fila com CAPACIDADE_FILA peças iniciais
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        fila->elementos[fila->traseira] = gerarPeca();
        fila->traseira = (fila->traseira + 1) % CAPACIDADE_FILA;
        fila->tamanho++;
    }
}

// Insere uma nova peça no final da fila (enqueue)
void inserirFila(FilaPecas *fila, Peca novaPeca) {
    if (fila->tamanho == CAPACIDADE_FILA) {
        // Isso não deve ocorrer com a regra de manter o tamanho, mas é um bom safe-guard
        printf("ERRO: Fila de peças cheia.\n");
        return;
    }
    fila->elementos[fila->traseira] = novaPeca;
    fila->traseira = (fila->traseira + 1) % CAPACIDADE_FILA;
    fila->tamanho++;
}

// Remove e retorna a peça da frente da fila (dequeue / jogar peça)
Peca removerFila(FilaPecas *fila) {
    if (fila->tamanho == 0) {
        printf("ERRO: Fila de peças vazia.\n");
        Peca pecaNula = {'X', -1};
        return pecaNula;
    }

    Peca pecaRemovida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;

    return pecaRemovida;
}

// --- Funções da PILHA (Stack) ---

void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1; // Pilha vazia
}

// Verifica se a pilha está cheia
bool pilhaEstaCheia(PilhaReserva *pilha) {
    return pilha->topo == CAPACIDADE_PILHA - 1;
}

// Verifica se a pilha está vazia
bool pilhaEstaVazia(PilhaReserva *pilha) {
    return pilha->topo == -1;
}

// Insere uma peça no topo da pilha (push)
void inserirPilha(PilhaReserva *pilha, Peca peca) {
    if (pilhaEstaCheia(pilha)) {
        printf("ERRO: Pilha de reserva cheia. Nao e possivel reservar mais pecas.\n");
        return;
    }
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
    printf("PECA RESERVADA: [%c %d] movida para o topo da pilha.\n", peca.nome, peca.id);
}

// Remove e retorna a peça do topo da pilha (pop / usar peça reservada)
Peca removerPilha(PilhaReserva *pilha) {
    if (pilhaEstaVazia(pilha)) {
        printf("ERRO: Pilha de reserva vazia. Nao ha pecas para usar.\n");
        Peca pecaNula = {'X', -1};
        return pecaNula;
    }

    Peca pecaUsada = pilha->elementos[pilha->topo];
    pilha->topo--;
    return pecaUsada;
}

// --- Funções de Exibição ---

void exibirFila(FilaPecas *fila) {
    printf("Fila de pecas futuras (%d/%d): [FRENTE] ", fila->tamanho, CAPACIDADE_FILA);

    if (fila->tamanho == 0) {
        printf("Vazia");
    } else {
        for (int i = 0; i < fila->tamanho; i++) {
            int indice = (fila->frente + i) % CAPACIDADE_FILA;
            Peca p = fila->elementos[indice];
            printf("[%c %d]", p.nome, p.id);
            if (i < fila->tamanho - 1) {
                printf(" -> ");
            }
        }
    }
    printf(" [TRASEIRA]\n");
}

void exibirPilha(PilhaReserva *pilha) {
    printf("Pilha de reserva (Topo -> Base) (%d/%d): ", pilha->topo + 1, CAPACIDADE_PILHA);

    if (pilhaEstaVazia(pilha)) {
        printf("Vazia\n");
    } else {
        // Percorre do topo (indice 'topo') ate a base (indice 0)
        for (int i = pilha->topo; i >= 0; i--) {
            Peca p = pilha->elementos[i];
            printf("[%c %d]", p.nome, p.id);
            if (i > 0) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void exibirEstadoAtual(FilaPecas *fila, PilhaReserva *pilha) {
    printf("\n=========================================\n");
    printf("ESTADO ATUAL DO JOGO TETRIS STACK\n");
    exibirFila(fila);
    exibirPilha(pilha);
    printf("=========================================\n");
}

// --- Funções de Ação ---

void jogarPecaAcao(FilaPecas *fila) {
    Peca pecaJogada = removerFila(fila);

    if (pecaJogada.id != -1) {
        printf("ACAO 1: PECA JOGADA. [%c %d] removida da fila.\n", pecaJogada.nome, pecaJogada.id);
        // Regra: A cada ação, uma nova peça é gerada e adicionada ao final da fila
        Peca novaPeca = gerarPeca();
        inserirFila(fila, novaPeca);
    }
}

void reservarPecaAcao(FilaPecas *fila, PilhaReserva *pilha) {
    if (pilhaEstaCheia(pilha)) {
        printf("ACAO 2: RESERVA CANCELADA. Pilha cheia.\n");
        return;
    }

    if (fila->tamanho == 0) {
        printf("ACAO 2: RESERVA CANCELADA. Fila de pecas vazia.\n");
        return;
    }

    // 1. Move a peça da FRENTE da fila
    Peca pecaReservada = removerFila(fila);

    // 2. Insere a peça no TOPO da pilha
    inserirPilha(pilha, pecaReservada);

    // 3. Regra: Gera e adiciona uma nova peça ao final da fila
    Peca novaPeca = gerarPeca();
    inserirFila(fila, novaPeca);
}

void usarPecaReservadaAcao(PilhaReserva *pilha) {
    Peca pecaUsada = removerPilha(pilha);

    if (pecaUsada.id != -1) {
        printf("ACAO 3: PECA RESERVADA USADA. [%c %d] removida da pilha.\n", pecaUsada.nome, pecaUsada.id);
        // Observação: Nesta ação, a fila NÃO é alterada, apenas a pilha.
        // A regra de adicionar nova peça ao final da fila se aplica apenas a "cada ação" que
        // mexe com a fila (jogar ou reservar), para manter o fluxo.
    }
}

// --- Função Principal e Menu ---

int main() {
    // Inicializa a semente para geração de peças aleatórias
    srand(time(NULL));

    // Inicializa as estruturas
    FilaPecas filaDePecas;
    PilhaReserva pilhaDeReserva;
    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    int opcao;

    do {
        exibirEstadoAtual(&filaDePecas, &pilhaDeReserva);
        // 

        // Exibe as opções de ação
        printf("\nOpcoes de acao:\n");
        printf("-----------------------------------------\n");
        printf("Codigo\tAcao\n");
        printf("1\tJogar peca (Dequeue/Fila)\n");
        printf("2\tReservar peca (Fila -> Pilha)\n");
        printf("3\tUsar peca reservada (Pop/Pilha)\n");
        printf("0\tSair\n");
        printf("-----------------------------------------\n");
        printf("Digite o codigo da acao: ");

        // Lê a opção do usuário
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida. Por favor, digite um numero.\n");
            while (getchar() != '\n');
            opcao = -1;
            continue;
        }

        printf("\n");

        switch (opcao) {
            case 1:
                // 1. Jogar peça (Dequeue)
                jogarPecaAcao(&filaDePecas);
                break;

            case 2:
                // 2. Reservar peça (Fila -> Pilha)
                reservarPecaAcao(&filaDePecas, &pilhaDeReserva);
                break;

            case 3:
                // 3. Usar peça reservada (Pop)
                usarPecaReservadaAcao(&pilhaDeReserva);
                break;

            case 0:
                // 0. Sair
                printf("Saindo do simulador Tetris Stack.\n");
                break;

            default:
                printf("OPCAO INVALIDA. Por favor, escolha 1, 2, 3 ou 0.\n");
                break;
        }

        // Aguarda o usuário para a próxima iteração
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            while (getchar() != '\n');
            getchar();
            printf("\n\n");
        }

    } while (opcao != 0);

    return 0;
}