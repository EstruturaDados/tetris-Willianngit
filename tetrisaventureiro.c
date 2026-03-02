#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA  5
#define TAM_PILHA 3

typedef struct {
    char nome; 
    int id;    
} Peca;

typedef struct {
    Peca v[TAM_FILA];
    int inicio;   
    int fim;      
    int qtd;      
} Fila;

typedef struct {
    Peca v[TAM_PILHA];
    int topo;     
} Pilha;


Peca gerarPeca(void) {
    static int proximoId = 0;
    const char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}


void filaInit(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

int filaVazia(const Fila *f) { return f->qtd == 0; }
int filaCheia(const Fila *f) { return f->qtd == TAM_FILA; }

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->v[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->qtd++;
    return 1;
}

int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) return 0;
    *removida = f->v[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return 1;
}


void pilhaInit(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(const Pilha *p) { return p->topo == -1; }
int pilhaCheia(const Pilha *p) { return p->topo == TAM_PILHA - 1; }

int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->v[++p->topo] = x;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) return 0;
    *removida = p->v[p->topo--];
    return 1;
}


void mostrarFila(const Fila *f) {
    printf("Fila de peças\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < f->qtd; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        printf("[%c %d]", f->v[idx].nome, f->v[idx].id);
        if (i < f->qtd - 1) printf(" ");
    }
    printf("\n");
}

void mostrarPilha(const Pilha *p) {
    printf("Pilha de reserva\n");
    printf("(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d]", p->v[i].nome, p->v[i].id);
        if (i > 0) printf(" ");
    }
    printf("\n");
}

void mostrarEstado(const Fila *f, const Pilha *p) {
    printf("\nEstado atual:\n");
    mostrarFila(f);
    mostrarPilha(p);
}

void mostrarMenu(void) {
    printf("\nOpções de Ação:\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("0 - Sair\n");
    printf("Opção: ");
}


void manterFilaCheia(Fila *f) {
    while (!filaCheia(f)) {
        enqueue(f, gerarPeca());
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    Pilha pilha;
    filaInit(&fila);
    pilhaInit(&pilha);

    
    manterFilaCheia(&fila);

    int op;
    do {
        mostrarEstado(&fila, &pilha);
        mostrarMenu();

        if (scanf("%d", &op) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("\nEntrada inválida.\n");
            continue;
        }

        if (op == 1) { 
            Peca x;
            if (dequeue(&fila, &x)) {
                printf("\nPeça jogada: [%c %d]\n", x.nome, x.id);
                manterFilaCheia(&fila);
            } else {
                printf("\nFila vazia (não deveria acontecer).\n");
            }

        } else if (op == 2) { 
            Peca x;
            if (!dequeue(&fila, &x)) {
                printf("\nFila vazia (não deveria acontecer).\n");
                continue;
            }

            if (push(&pilha, x)) {
                printf("\nPeça reservada: [%c %d]\n", x.nome, x.id);
                manterFilaCheia(&fila);
            } else {
                
                printf("\nPilha cheia. Peça descartada: [%c %d]\n", x.nome, x.id);
                manterFilaCheia(&fila);
            }

        } else if (op == 3) { 
            Peca x;
            if (pop(&pilha, &x)) {
                printf("\nPeça reservada usada: [%c %d]\n", x.nome, x.id);
                manterFilaCheia(&fila); /* mantém fila cheia após a ação */
            } else {
                printf("\nPilha vazia. Não há peça reservada para usar.\n");
            }

        } else if (op != 0) {
            printf("\nOpção inválida.\n");
        }

    } while (op != 0);

    printf("\nSaindo...\n");
    return 0;
}
