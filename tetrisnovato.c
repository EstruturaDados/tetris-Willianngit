#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5  // a fila deve conter exatamente 5 posições (capacidade)

typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

typedef struct {
    Peca v[N];
    int inicio;   // aponta para a frente (de onde remove)
    int fim;      // aponta para onde insere
    int qtd;      // quantidade atual na fila
} Fila;

/* Gera peça automaticamente (nome aleatório, id crescente). */
Peca gerarPeca(void) {
    static int proximoId = 0;
    const char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

int filaCheia(Fila *f) { return f->qtd == N; }
int filaVazia(Fila *f) { return f->qtd == 0; }

/* Enqueue: insere no final se houver espaço. */
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;

    f->v[f->fim] = p;
    f->fim = (f->fim + 1) % N;
    f->qtd++;
    return 1;
}

/* Dequeue: remove da frente se houver peça. */
int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) return 0;

    *removida = f->v[f->inicio];
    f->inicio = (f->inicio + 1) % N;
    f->qtd--;
    return 1;
}

void mostrarFila(Fila *f) {
    printf("Confira a seguir seu estado:\n");
    printf("Fila de peças\n");

    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }

    for (int i = 0; i < f->qtd; i++) {
        int idx = (f->inicio + i) % N;
        printf("[%c %d]", f->v[idx].nome, f->v[idx].id);
        if (i < f->qtd - 1) printf(" ");
    }
    printf("\n");
}

void menu(void) {
    printf("\nOpções de ação:\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Inserir nova peça (enqueue)\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    inicializarFila(&fila);

    /* Inicializar a fila com 5 peças geradas automaticamente. */
    for (int i = 0; i < N; i++) {
        enqueue(&fila, gerarPeca());
    }

    int op;
    do {
        mostrarFila(&fila);
        menu();

        if (scanf("%d", &op) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("\nEntrada inválida.\n");
            continue;
        }

        if (op == 1) {
            Peca p;
            if (dequeue(&fila, &p)) {
                printf("\nPeça jogada: [%c %d]\n", p.nome, p.id);
            } else {
                printf("\nFila vazia, não há peça para jogar.\n");
            }

        } else if (op == 2) {
            Peca nova = gerarPeca();
            if (enqueue(&fila, nova)) {
                printf("\nPeça inserida: [%c %d]\n", nova.nome, nova.id);
            } else {
                printf("\nFila cheia, não é possível inserir.\n");
            }

        } else if (op != 0) {
            printf("\nOpção inválida.\n");
        }

    } while (op != 0);

    return 0;
}
