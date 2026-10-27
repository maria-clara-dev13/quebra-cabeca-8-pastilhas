// Maria Clara Miranda de Sá 21.2.4187 turma 33
// Trabalho Prático - Quebra Cabeça de 8 pastilhas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definindo constantes para o tamanho máximo e mínimo do tabuleiro
#define MAX_SIZE 10
#define MIN_SIZE 3

#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_BG_COLOR_BLACK "\x1b[40m"
#define ANSI_BG_COLOR_RED "\x1b[41m"
#define ANSI_BG_COLOR_GREEN "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW "\x1b[43m"
#define ANSI_BG_COLOR_BLUE "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
#define ANSI_BG_COLOR_CYAN "\x1b[46m"
#define ANSI_BG_COLOR_WHITE "\x1b[47m"


// Estrutura que representa o estado do jogo
typedef struct {
    int tamanho;          // Tamanho do tabuleiro (n x n)
    int **tabuleiro;      // Matriz que representa o tabuleiro
    int movimentos;       // Contador de movimentos feitos
    int linhaBranco;      // Linha da posição vazia (branca)
    int colunaBranco;     // Coluna da posição vazia (branca)
} QuebraCabeca;

// Funções declaradas para manipulação do jogo
void inicializaTabuleiro(QuebraCabeca *game);
void liberaTabuleiro(QuebraCabeca *game);
void imprimeTabuleiro(QuebraCabeca *game);
void salvaJogo(QuebraCabeca *game, char *arquivo);
void carregaJogo(QuebraCabeca *game, const char *arquivo);
void embaralha(QuebraCabeca *game, char dificuldade);
int resolvido(QuebraCabeca *game);
void imprimeInstrucoes();
int movePeca(QuebraCabeca *game, int peca);

// Inicializa o tabuleiro alocando memória para a matriz
void inicializaTabuleiro(QuebraCabeca *game) {
    game->tabuleiro = (int **)malloc(game->tamanho * sizeof(int *)); // Aloca memória para as linhas
    for (int i = 0; i < game->tamanho; i++) {
        game->tabuleiro[i] = (int *)malloc(game->tamanho * sizeof(int)); // Aloca memória para as colunas
    }
}

// Libera a memória alocada para o tabuleiro
void liberaTabuleiro(QuebraCabeca *game) {
    for (int i = 0; i < game->tamanho; i++) {
        free(game->tabuleiro[i]); // Libera cada linha
    }
    free(game->tabuleiro); // Libera a matriz
}

// Imprime o menu na tela
void imprimeMenu() {
    printf(ANSI_BOLD"\n\t-----------Bem-vindo ao Quebra-Cabeças--------------\n"ANSI_RESET); // Mensagem de boas-vindas
    printf("\nX. Sair do Jogo\n");
    printf("N. Iniciar um novo jogo\n");
    printf("C. Continuar o jogo atual\n");
    printf("S. Continuar um jogo salvo\n");
    printf("A. Ajuda com as instruções do jogo\n");
    
    printf("\nEscolha a opção (digite “voltar” em qualquer lugar para voltar ao menu principal): ");
}

void imprimeTabuleiro(QuebraCabeca *game) {
    printf("\n");
    for (int i = 0; i < game->tamanho; i++) {
        // Imprime a linha superior do tabuleiro
        for (int j = 0; j < game->tamanho; j++) {
            printf("+---");
        }
        printf("+\n");

        // Imprime cada célula do tabuleiro
        for (int j = 0; j < game->tamanho; j++) {
            if (game->tabuleiro[i][j] == 0) {
                printf("|   ");  // Espaço vazio
            } else {
                // Imprime o número com cores diferentes, dependendo do valor
                switch (game->tabuleiro[i][j] % 7) { // Use mod 7 para variar as cores
                    case 0: 
                        printf("|" ANSI_BOLD ANSI_COLOR_BLUE "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                    case 1: 
                        printf("|" ANSI_BOLD ANSI_COLOR_GREEN "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                    case 2: 
                        printf("|" ANSI_BOLD ANSI_COLOR_YELLOW "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                    case 3: 
                        printf("|" ANSI_BOLD ANSI_COLOR_RED "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                    case 4: 
                        printf("|" ANSI_BOLD ANSI_COLOR_CYAN "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                    case 5: 
                        printf("|" ANSI_BOLD ANSI_COLOR_MAGENTA "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                    case 6: 
                        printf("|" ANSI_BOLD ANSI_COLOR_WHITE "%2d" ANSI_RESET " ", game->tabuleiro[i][j]); 
                        break;
                }
            }
        }
        printf("|\n");
    }

    // Imprime a linha inferior do tabuleiro
    for (int j = 0; j < game->tamanho; j++) {
        printf("+---");
    }
    printf("+\n");
}


// Salva o estado do jogo em um arquivo
void salvaJogo(QuebraCabeca *game, char *arquivo) {
    FILE *file = fopen(arquivo, "w"); // Abre o arquivo para escrita
    if (!file) {
        printf("\nErro ao abrir o arquivo para salvar\n");
        return; // Retorna se não conseguir abrir
    }

    // Escreve tamanho do tabuleiro e estado do tabuleiro no arquivo
    fprintf(file, "%d\n", game->tamanho);
    for (int i = 0; i < game->tamanho; i++) {
        for (int j = 0; j < game->tamanho; j++) {
            fprintf(file, "%d ", game->tabuleiro[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "%d\n", game->movimentos); // Salva o número de movimentos

    fclose(file); // Fecha o arquivo
    printf("Arquivo \"%s\" salvo com sucesso!\n", arquivo);
}

// Carrega o estado do jogo a partir de um arquivo
void carregaJogo(QuebraCabeca *game, const char *arquivo) {
    FILE *file = fopen(arquivo, "r"); // Abre o arquivo para leitura
    if (!file) {
        printf("\nErro ao abrir o arquivo para carregar\n");
        return; // Retorna se não conseguir abrir
    }

    liberaTabuleiro(game); // Libera tabuleiro atual

    fscanf(file, "%d", &game->tamanho); // Lê o tamanho do tabuleiro

    inicializaTabuleiro(game); // Inicializa um novo tabuleiro

    // Lê o estado do tabuleiro do arquivo
    for (int i = 0; i < game->tamanho; i++) {
        for (int j = 0; j < game->tamanho; j++) {
            fscanf(file, "%d", &game->tabuleiro[i][j]);
        }
    }
    fscanf(file, "%d", &game->movimentos); // Lê o número de movimentos

    fclose(file); // Fecha o arquivo
    printf("Arquivo \"%s\" Carregado. Vamos começar o jogo!\n", arquivo);
}

// Embaralha o tabuleiro de acordo com a dificuldade
void embaralha(QuebraCabeca *game, char dificuldade) {
    int numMovimento; // Variável para determinar o número de movimentos de embaralhamento
    switch (dificuldade) {
        case 'F': case 'f': // Fácil
            numMovimento = 5 * game->tamanho;
            break;
        case 'M': case 'm': // Médio
            numMovimento = 10 * game->tamanho;
            break;
        case 'D': case 'd': // Difícil
            numMovimento = 20 * game->tamanho;
            break;
        default:
            numMovimento = 0; // Dificuldade inválida
            break;
    }

    // Inicializa o tabuleiro em ordem crescente
    int valor = 1;
    for (int i = 0; i < game->tamanho; i++) {
        for (int j = 0; j < game->tamanho; j++) {
            game->tabuleiro[i][j] = valor++;
        }
    }
    game->tabuleiro[game->tamanho - 1][game->tamanho - 1] = 0; // Última posição é vazia
    game->linhaBranco = game->tamanho - 1; // Atualiza a posição do espaço vazio
    game->colunaBranco = game->tamanho - 1;

    //imprimeTabuleiro(game); // Imprime o tabuleiro inicial

    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    for (int i = 0; i < numMovimento; i++) {
        int direcao = rand() % 4; // Escolhe uma direção aleatória
        int r1 = game->linhaBranco; // Posição da linha branca
        int r2 = game->colunaBranco; // Posição da coluna branca

        // Calcula nova posição da peça a mover
        switch (direcao) {
            case 0: 
                r1--; 
                break; // Move para cima
            case 1: 
                r1++; 
                break; // Move para baixo
            case 2: 
                r2--; 
                break; // Move para esquerda
            case 3: 
                r2++; 
                break; // Move para direita
        }

        // Verifica se a nova posição é válida e realiza a troca
        if (r1 >= 0 && r1 < game->tamanho && r2 >= 0 && r2 < game->tamanho) {
            int temp = game->tabuleiro[r1][r2];
            game->tabuleiro[r1][r2] = game->tabuleiro[game->linhaBranco][game->colunaBranco];
            game->tabuleiro[game->linhaBranco][game->colunaBranco] = temp;
            game->linhaBranco = r1; // Atualiza a posição do espaço vazio
            game->colunaBranco = r2;
        }
    }
    printf("\nTabuleiro embaralhado. Vamos começar o jogo!\n");
}

// Verifica se o tabuleiro está na posição resolvida
int resolvido(QuebraCabeca *game) {
    int valor = 1; // Valor esperado
    for (int i = 0; i < game->tamanho; i++) {
        for (int j = 0; j < game->tamanho; j++) {
            // Checa se a última posição é zero
            if (i == game->tamanho - 1 && j == game->tamanho - 1) {
                if (game->tabuleiro[i][j] != 0)
                    return 0; // Não está resolvido
            } else if (game->tabuleiro[i][j] != valor++) {
                return 0; // Não está resolvido
            }
        }
    }
    return 1; // Retorna 1 se está resolvido
}

// Imprime instruções de como jogar
void imprimeInstrucoes() {
    printf(ANSI_BOLD "\nInstruções: " ANSI_RESET);
    printf("\n\nO jogo conhecido como quebra cabeça de 8 pastilhas é um quebra cabeça deslizante clássico que envolve o rearranjo de peças numeradas em uma grade. \nO objetivo é colocar os números em ordem crescente, movendo as peças uma de cada vez. \nVocê pode mover uma peça adjacente ao espaço vazio para ocupar o espaço. \nO espaço vazio se move para onde a peça estava.Planeje seus movimentos com antecedência para evitar ficar preso. \nÀs vezes, você precisará mover peças para fora do caminho para alcançar outras. Boa sorte!\n\nAqui estão alguns comandos para você começar a jogar: \n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\n<num>: " ANSI_RESET "Para mover a peça com o número correspondente.\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\nsalvar <nome_do_arquivo>: " ANSI_RESET "Para salvar o jogo em um arquivo.\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\nvoltar:" ANSI_RESET "Para voltar ao menu inicial.\n");
    
}

// Move uma peça do tabuleiro se for possível
int movePeca(QuebraCabeca *game, int peca) {
    // Busca a peça no tabuleiro
    for (int i = 0; i < game->tamanho; i++) {
        for (int j = 0; j < game->tamanho; j++) {
            if (game->tabuleiro[i][j] == peca) {
                // Verifica se a peça pode ser movida
                if ((i == game->linhaBranco && (j == game->colunaBranco - 1 || j == game->colunaBranco + 1)) ||
                    (j == game->colunaBranco && (i == game->linhaBranco - 1 || i == game->linhaBranco + 1))) {
                    // Move a peça para a posição vazia
                    game->tabuleiro[game->linhaBranco][game->colunaBranco] = peca;
                    game->tabuleiro[i][j] = 0; // Atualiza a posição da peça
                    game->linhaBranco = i; // Atualiza a posição do espaço vazio
                    game->colunaBranco = j;
                    game->movimentos++; // Incrementa o contador de movimentos
                    return 1; // Moveu com sucesso
                } else {
                    printf("\nNão é possível mover a peça %d.\n", peca);
                    return 0; // Movimento inválido
                }
            }
        }
    }
    printf("\nPeça %d não encontrada.\n", peca);
    return 0; // Peça não encontrada
}

// Função principal do programa
int main() {
    QuebraCabeca jogo; // Declaração da estrutura do jogo
    memset(&jogo, 0, sizeof(jogo)); // Inicializa a estrutura com zero

    char opcao[20]; // Armazena a opção do usuário
    char arquivo[32]; // Armazena o nome do arquivo
    char dificuldade; // Armazena o nível de dificuldade
    int peca; // Armazena a peça a ser movida
    int jogoAtivo = 0; // Rastrea se um jogo foi iniciado

    // Loop principal do menu do jogo
    while (1) {
        // Apresenta opções ao usuário
        imprimeMenu();
        scanf("%s", opcao); // Limita a entrada

        // Verifica a opção escolhida
        if (strcmp(opcao, "X") == 0 || strcmp(opcao, "x") == 0) {
            liberaTabuleiro(&jogo); // Libera memória alocada
            printf("\nSaindo do jogo...\n");
            break; // Sai do loop
        } else if (strcmp(opcao, "N") == 0 || strcmp(opcao, "n") == 0) {
            // Inicia um novo jogo
            do {
                printf("\nDigite a dimensão do tabuleiro (3 ≤ n ≤ 10): ");
                scanf("%d", &jogo.tamanho);
            } while (jogo.tamanho < MIN_SIZE || jogo.tamanho > MAX_SIZE);
            
            inicializaTabuleiro(&jogo); // Inicializa o tabuleiro
            if (jogo.tabuleiro == NULL) {
               printf("\nErro ao alocar memória para o tabuleiro.\n");
               exit(1); // Sai se a alocação falhar
           }
            // Preenche o tabuleiro com números em ordem
            for (int i = 0; i < jogo.tamanho; i++) {
                for (int j = 0; j < jogo.tamanho; j++) {
                    jogo.tabuleiro[i][j] = i * jogo.tamanho + j + 1;
                }
            }
            jogo.tabuleiro[jogo.tamanho - 1][jogo.tamanho - 1] = 0; // Última posição é vazia
            jogo.linhaBranco = jogo.tamanho - 1; // Atualiza a posição do espaço vazio
            jogo.colunaBranco = jogo.tamanho - 1;

            imprimeTabuleiro(&jogo); // Imprime o tabuleiro inicial
            printf("\nTabuleiro ordenado!");
            printf("\nDigite o nível de dificuldade para embaralhar, fácil (F), médio (M) ou difícil (D): ");
            scanf(" %c", &dificuldade); // Lê a dificuldade
            embaralha(&jogo, dificuldade); // Embaralha o tabuleiro
            jogoAtivo = 1; // Marca que um jogo foi iniciado
        } 
        else if (strcmp(opcao, "C") == 0 || strcmp(opcao, "c") == 0) {
            if (jogoAtivo) {
                // Se houver um jogo ativo, continue a partir dele
                printf("\nContinuando o jogo atual...\n");
                imprimeTabuleiro(&jogo);
            } else {
                printf("\nNão há jogo ativo para continuar.\n");
            }
        } 
        else if (strcmp(opcao, "S") == 0 || strcmp(opcao, "s") == 0) {
            // Carrega um jogo salvo
            printf("\nDigite o nome do arquivo: ");
            scanf("%s", arquivo); // Limita a entrada
            carregaJogo(&jogo, arquivo); // Carrega o jogo do arquivo
            jogoAtivo = 1; // Marca que um jogo foi iniciado
        } 
        else if (strcmp(opcao, "A") == 0 || strcmp(opcao, "a") == 0) {
            imprimeInstrucoes(); // Imprime as instruções do jogo
            //scanf("%s", opcao);
            //embaralha(&jogo, dificuldade);
            //imprimeTabuleiro(&jogo);
        } 
        else {
            printf("Opção inválida! Tente novamente.\n"); // Mensagem de erro para opção inválida
            continue;
        }

        // Loop para a interação durante o jogo
        while (jogoAtivo) {
            imprimeTabuleiro(&jogo); // Imprime o tabuleiro atual
            printf("Digite um comando ou número da peça que quer mover: ");
            scanf("%s", opcao); // Limita a entrada

            // Verifica se o usuário deseja voltar ao menu
            if (strcmp(opcao, "voltar") == 0) {
                jogoAtivo = 0; // Sai do loop do jogo
                continue; 
            } 

            if (strncmp(opcao, "salvar", 6) == 0) {
                scanf("%s", arquivo); // Lê o nome do arquivo para salvar
                salvaJogo(&jogo, arquivo); // Salva o jogo
            } else {
                peca = atoi(opcao); // Converte a entrada para um número
                if (movePeca(&jogo, peca)) { // Tenta mover a peça
                    if (resolvido(&jogo)) { // Verifica se o jogo foi resolvido
                        imprimeTabuleiro(&jogo); // Imprime o tabuleiro final
                        printf("\nParabéns! Você resolveu o quebra-cabeça em %d movimentos.\n", jogo.movimentos);
                        jogoAtivo = 0; // Sai do loop de interação
                        return 0; 
                    }
                }
            }
            printf("Quantidade de jogadas até agora: %d\n", jogo.movimentos); // Mostra número de movimentos
        }
    }
    
    liberaTabuleiro(&jogo); // Libera memória alocada ao final do jogo
    return 0; // Retorna 0 para indicar que o programa terminou com sucesso
}

