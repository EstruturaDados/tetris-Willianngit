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
    int inicio, fim, qtd;
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


void filaInit(Fila *f) { f->inicio = 0; f->fim = 0; f->qtd = 0; }
int  filaCheia(const Fila *f) { return f->qtd == TAM_FILA; }
int  filaVazia(const Fila *f) { return f->qtd == 0; }

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->v[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->qtd++;
    return 1;
}

int dequeue(Fila *f, Peca *rem) {
    if (filaVazia(f)) return 0;
    *rem = f->v[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return 1;
}

void manterFilaCheia(Fila *f) {
    while (!filaCheia(f)) enqueue(f, gerarPeca());
}

int filaGet(const Fila *f, int i, Peca *out) {
    if (i < 0 || i >= f->qtd) return 0;
    int idx = (f->inicio + i) % TAM_FILA;
    *out = f->v[idx];
    return 1;
}

int filaSet(Fila *f, int i, Peca val) {
    if (i < 0 || i >= f->qtd) return 0;
    int idx = (f->inicio + i) % TAM_FILA;
    f->v[idx] = val;
    return 1;
}


void pilhaInit(Pilha *p) { p->topo = -1; }
int  pilhaVazia(const Pilha *p) { return p->topo == -1; }
int  pilhaCheia(const Pilha *p) { return p->topo == TAM_PILHA - 1; }
int  pilhaQtd(const Pilha *p) { return p->topo + 1; }

int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->v[++p->topo] = x;
    return 1;
}

int pop(Pilha *p, Peca *rem) {
    if (pilhaVazia(p)) return 0;
    *rem = p->v[p->topo--];
    return 1;
}


void printFilaInline(const Fila *f) {
    printf("Fila de Peças: ");
    for (int i = 0; i < f->qtd; i++) {
        Peca x; filaGet(f, i, &x);
        printf("[%c %d]%s", x.nome, x.id, (i < f->qtd - 1) ? " " : "");
    }
    printf("\n");
}

void printPilhaInline(const Pilha *p) {
    printf("Pilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d]%s", p->v[i].nome, p->v[i].id, (i > 0) ? " " : "");
    }
    printf("\n");
}

void printEstadoAtual(const Fila *f, const Pilha *p) {
    printf("=== Estado Atual ===\n\n");
    printFilaInline(f);
    printf("\n");
    printPilhaInline(p);
    printf("\nOpções:\n\n");
    printf("1 - Jogar peça da frente da fila\n");
    printf("2 - Enviar peça da fila para reserva (pilha)\n");
    printf("3 - Usar peça da reserva (pilha)\n");
    printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3\n");
    printf("    peças da pilha\n");
    printf("0 - Sair\n\n");
    printf("Opção: ");
}


int trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (f->qtd < 1 || pilhaVazia(p)) return 0;
    Peca frente; filaGet(f, 0, &frente);
    Peca topo = p->v[p->topo];
    filaSet(f, 0, topo);
    p->v[p->topo] = frente;
    return 1;
}

int trocar3Primeiros(Fila *f, Pilha *p) {
    if (f->qtd < 3 || pilhaQtd(p) < 3) return 0;

    Peca f0, f1, f2;
    filaGet(f, 0, &f0);
    filaGet(f, 1, &f1);
    filaGet(f, 2, &f2);

    Peca pTop = p->v[p->topo];
    Peca pMid = p->v[p->topo - 1];
    Peca pBas = p->v[p->topo - 2];

    filaSet(f, 0, pTop);
    filaSet(f, 1, pMid);
    filaSet(f, 2, pBas);

    p->v[p->topo]     = f0;
    p->v[p->topo - 1] = f1;
    p->v[p->topo - 2] = f2;

    return 1;
}

int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    Pilha pilha;
    filaInit(&fila);
    pilhaInit(&pilha);

    manterFilaCheia(&fila);

    int opcao;
    do {
        printEstadoAtual(&fila, &pilha);

        if (scanf("%d", &opcao) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        if (opcao == 1) {
            Peca x;
            if (dequeue(&fila, &x)) {
                manterFilaCheia(&fila);
            }

        } else if (opcao == 2) {
            Peca x;
            if (dequeue(&fila, &x)) {
                
                push(&pilha, x);
                manterFilaCheia(&fila);
            }

        } else if (opcao == 3) {
            Peca x;
            pop(&pilha, &x);

        } else if (opcao == 4) {
            trocarFrenteComTopo(&fila, &pilha);

        } else if (opcao == 5) {
            printf("\nTroca realizada entre os 3 primeiros da\n");
            printf("fila e os 3 da pilha.\n\n");
            if (trocar3Primeiros(&fila, &pilha)) {
                printf("=== Novo Estado ===\n\n");
                printFilaInline(&fila);
                printf("\n");
                printPilhaInline(&pilha);
                printf("\n");
            } else {
                printf("(Não foi possível trocar: faltam peças)\n\n");
            }

        }

    } while (opcao != 0);

    return 0;
}
