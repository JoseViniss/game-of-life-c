#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MAX_X_POS 50
#define MAX_Y_POS 40

const char CHAR_FUNDO = '.';
const char CHAR_CELULA = 'X';
const int TEMPO_ESPERA = 200000; // microsegundos

char matrizPrincipal[MAX_Y_POS][MAX_X_POS];

typedef struct {
    int x;
    int y;
} Celula;

Celula *parsearCoordenadasIniciais(char *s, int *);
void iniciarMatriz();
void definirCelulasIniciais(Celula celulasIniciais[], size_t);
void loopConway();
void atualizarCelulas();
void imprimirCelulas();
int contarVizinhos(int, int);

int main() {
    int contagemCelulasIniciais = 0;
    char s[10000];

    printf("Digite uma lista de coordenadas x e y separadas por vírgulas para definir as células iniciais:\n");
    scanf("%9999[^\n]", s);

    Celula *celulasIniciais = parsearCoordenadasIniciais(s, &contagemCelulasIniciais);
    iniciarMatriz();
    definirCelulasIniciais(celulasIniciais, contagemCelulasIniciais);
    loopConway();
    free(celulasIniciais);

    return 0;
}

Celula *parsearCoordenadasIniciais(char *s, int *contagemCelulasIniciais) {
    Celula *listaCelulas = calloc(MAX_X_POS * MAX_Y_POS, sizeof(Celula));
    if (listaCelulas == NULL) {
        perror("Falha na alocação de memória. Encerrando agora");
        exit(EXIT_FAILURE);
    }

    int contagem = 0;
    char *token = strtok(s, ",");

    while (token != NULL && contagem < MAX_X_POS * MAX_Y_POS) {
        int x, y;
        if (sscanf(token, "%d %d", &x, &y) == 2) {
            if (x >= 0 && x < MAX_X_POS && y >= 0 && y < MAX_Y_POS) {
                listaCelulas[contagem++] = (Celula){.x = x, .y = y};
            } else {
                printf("Coordenadas (%d, %d) fora dos limites. Ignorando.\n", x, y);
            }
        }
        token = strtok(NULL, ",");
    }

    *contagemCelulasIniciais = contagem;
    return realloc(listaCelulas, contagem * sizeof(Celula));
}

void loopConway() {
    while (true) {
        imprimirCelulas();
        atualizarCelulas();
        usleep(TEMPO_ESPERA);
    }
}

void iniciarMatriz() {
    for (int i = 0; i < MAX_Y_POS; i++) {
        memset(matrizPrincipal[i], CHAR_FUNDO, sizeof(matrizPrincipal[i]));
    }
}

void definirCelulasIniciais(Celula listaCelulas[], size_t tamanho) {
    for (int i = 0; i < tamanho; i++) {
        matrizPrincipal[listaCelulas[i].y][listaCelulas[i].x] = CHAR_CELULA;
    }
}

void imprimirCelulas() {
    system("clear");
    for (int i = 0; i < MAX_Y_POS; i++) {
        for (int j = 0; j < MAX_X_POS; j++) {
            printf("%c", matrizPrincipal[i][j]);
        }
        printf("\n");
    }
}

int contarVizinhos(int posY, int posX) {
    int vizinhos = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue;

            int y = (posY + dy + MAX_Y_POS) % MAX_Y_POS;
            int x = (posX + dx + MAX_X_POS) % MAX_X_POS;

            if (matrizPrincipal[y][x] == CHAR_CELULA) {
                vizinhos++;
            }
        }
    }
    return vizinhos;
}

void atualizarCelulas() {
    char novaMatriz[MAX_Y_POS][MAX_X_POS];

    for (int i = 0; i < MAX_Y_POS; i++) {
        for (int j = 0; j < MAX_X_POS; j++) {
            int vizinhos = contarVizinhos(i, j);
            if (matrizPrincipal[i][j] == CHAR_CELULA) {
                novaMatriz[i][j] = (vizinhos < 2 || vizinhos > 3) ? CHAR_FUNDO : CHAR_CELULA;
            } else {
                novaMatriz[i][j] = (vizinhos == 3) ? CHAR_CELULA : CHAR_FUNDO;
            }
        }
    }

    memcpy(matrizPrincipal, novaMatriz, sizeof(matrizPrincipal));
}
