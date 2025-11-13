#include <stdio.h>
#include <stdlib.h> // Para funções de aleatoriedade (rand, srand)
#include <time.h>   // Para inicializar a semente de aleatoriedade (time)
#include <stdbool.h> // Para usar tipos booleanos (true/false)

// Constantes
#define CAPACIDADE_MAXIMA 5 // Tamanho fixo da fila, conforme requisito

// Variáveis globais para rastreio
static int proximoId = 0; // Gerador de ID único para as peças

// --- Estruturas de Dados ---

// 1. Definição da estrutura para uma Peca (Bloco de Tetris)
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // ID numérico único
} Peca;

// 2. Definição da estrutura para a Fila de Peças
typedef struct {
    Peca elementos[CAPACIDADE_MAXIMA]; // Array estático para os elementos
    int frente; // Índice da frente (próxima peça a ser jogada/removida)
    int traseira; // Índice da traseira (próximo local para inserção)
    int tamanho; // Número atual de elementos na fila
} FilaPecas;

// --- Funções Auxiliares ---

// Função para gerar uma nova peça automaticamente
Peca gerarPeca() {
    // Tipos de peças disponíveis: 'I', 'O', 'T', 'L' (simplificado para 4 tipos)
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);

    // Seleciona um tipo aleatoriamente
    char tipoAleatorio = tipos[rand() % numTipos];

    // Cria a nova peça
    Peca novaPeca = {
        .nome = tipoAleatorio,
        .id = proximoId++ // Atribui o ID atual e incrementa para o próximo
    };

    return novaPeca;
}

// Inicializa a fila
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->traseira = 0;
    fila->tamanho = 0;

    // Inicializa a fila com a capacidade máxima de elementos
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        // Enqueue inicial: insere a peça na posição 'traseira'
        fila->elementos[fila->traseira] = gerarPeca();
        fila->traseira = (fila->traseira + 1) % CAPACIDADE_MAXIMA;
        fila->tamanho++;
    }
}

// Verifica se a fila está cheia
bool estaCheia(FilaPecas *fila) {
    return fila->tamanho == CAPACIDADE_MAXIMA;
}

// Verifica se a fila está vazia
bool estaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

// --- Operações da Fila ---

/**
 * Insere uma nova peça no final da fila (enqueue).
 */
void inserirPeca(FilaPecas *fila, Peca novaPeca) {
    if (estaCheia(fila)) {
        printf("ERRO: A fila de peças futuras está CHEIA! Nao e possivel inserir mais.\n");
        return;
    }

    // Insere o elemento na posição da 'traseira'
    fila->elementos[fila->traseira] = novaPeca;

    // Move a 'traseira' para a próxima posição (circular)
    fila->traseira = (fila->traseira + 1) % CAPACIDADE_MAXIMA;

    // Incrementa o tamanho da fila
    fila->tamanho++;

    printf("PECA INSERIDA: [%c %d] adicionada ao final da fila.\n", novaPeca.nome, novaPeca.id);
}

/**
 * Remove e retorna a peça da frente da fila (dequeue / jogar peça).
 */
Peca jogarPeca(FilaPecas *fila) {
    if (estaVazia(fila)) {
        printf("ERRO: A fila de pecas futuras esta VAZIA! Nao ha pecas para jogar.\n");
        // Retorna uma peça nula/erro para indicar falha
        Peca pecaNula = {'X', -1};
        return pecaNula;
    }

    // Pega a peça na posição da 'frente' (a ser removida)
    Peca pecaJogada = fila->elementos[fila->frente];

    // Move a 'frente' para a próxima posição (circular)
    fila->frente = (fila->frente + 1) % CAPACIDADE_MAXIMA;

    // Decrementa o tamanho da fila
    fila->tamanho--;

    printf("PECA JOGADA: [%c %d] foi removida da frente da fila.\n", pecaJogada.nome, pecaJogada.id);
    return pecaJogada;
}

// Exibe o estado atual da fila, seguindo a ordem de jogo
void exibirFila(FilaPecas *fila) {
    printf("\n--- Fila de Pecas Futuras (Capacidade: %d/%d) ---\n", fila->tamanho, CAPACIDADE_MAXIMA);

    if (estaVazia(fila)) {
        printf("A fila esta vazia.\n");
        return;
    }

    printf("[FRENTE] ");

    // Percorre a fila a partir da 'frente' ate o numero total de elementos
    for (int i = 0; i < fila->tamanho; i++) {
        // Calcula o indice atual no array (circularmente)
        int indice = (fila->frente + i) % CAPACIDADE_MAXIMA;

        // Pega a peça e exibe
        Peca p = fila->elementos[indice];
        printf("[%c %d]", p.nome, p.id);

        // Adiciona um separador se nao for o ultimo elemento
        if (i < fila->tamanho - 1) {
            printf(" -> ");
        }
    }

    printf(" [TRASEIRA]\n");
    printf("-----------------------------------------\n");
}

// --- Função Principal e Menu ---

int main() {
    // Inicializa a semente para geração de peças aleatórias
    srand(time(NULL));

    // Declara e inicializa a fila
    FilaPecas filaDePecas;
    inicializarFila(&filaDePecas);

    int opcao;
    Peca pecaRemovida; // Variável para armazenar a peça jogada

    do {
        // Exibe o estado da fila
        exibirFila(&filaDePecas);

        // Exibe as opções de ação
        printf("\nOpcoes de acao:\n");
        printf("-----------------------------------------\n");
        printf("Codigo\tAcao\n");
        printf("1\tJogar peca (dequeue)\n");
        printf("2\tInserir nova peca (enqueue)\n");
        printf("0\tSair\n");
        printf("-----------------------------------------\n");
        printf("Digite o codigo da acao: ");

        // Lê a opção do usuário
        if (scanf("%d", &opcao) != 1) {
            // Trata entrada inválida (não numérica)
            printf("\nEntrada invalida. Por favor, digite um numero.\n");
            // Limpa o buffer de entrada para evitar loops infinitos
            while (getchar() != '\n');
            opcao = -1; // Garante que o loop continue
            continue;
        }

        printf("\n"); // Espaçamento para clareza

        switch (opcao) {
            case 1:
                // 1. Jogar peça (Dequeue)
                pecaRemovida = jogarPeca(&filaDePecas);
                if (pecaRemovida.id != -1) {
                    // Se a peça foi removida com sucesso, gera e insere uma nova para manter o fluxo
                    Peca proximaPeca = gerarPeca();
                    inserirPeca(&filaDePecas, proximaPeca);
                }
                break;

            case 2:
                // 2. Inserir nova peça (Enqueue)
                // Gera uma nova peça para a inserção
                Peca novaPeca = gerarPeca();
                inserirPeca(&filaDePecas, novaPeca);
                break;

            case 0:
                // 0. Sair
                printf("Saindo do simulador Tetris Stack.\n");
                break;

            default:
                printf("OPCAO INVALIDA. Por favor, escolha 1, 2 ou 0.\n");
                break;
        }

        // Aguarda o usuário para a próxima iteração
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            // Limpa o buffer e espera por um ENTER
            while (getchar() != '\n'); // Limpa a linha pendente do scanf
            getchar(); // Espera pelo novo ENTER
            printf("\n\n");
        }

    } while (opcao != 0);

    return 0;
}