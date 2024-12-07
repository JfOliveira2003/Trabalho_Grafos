#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Estrutura para um n� da lista encadeada
typedef struct Noh {
    int posicaoY;
    int posicaoX;
    int valor;             // �ndice do n�
    struct Noh* proximo;   // Ponteiro para o pr�ximo n�
} Noh;

// Fun��es para manipular a lista encadeada
Noh* criarNoh(int valor);
void adicionarNoh(Noh** caminho, int valor);
void adicionarNohNoFinal(Noh** caminho, int valor);
void liberarLista(Noh* caminho);
void removerNoh(Noh** caminho, int valor);
#endif // LINKED_LIST_H
