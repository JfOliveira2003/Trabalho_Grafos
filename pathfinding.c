#include "pathfinding.h"
#include "comparisons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>  // Para definir o valor de INF
#include <math.h>




///////////////////////////// ALGORITMO A* /////////////////////////

typedef struct open_list{
    Noh* node;
    Noh* father;
    int F;
    open_list* next;
} open_list;

typedef struct closed_list{
    Noh* node;
    Noh* father;
    closed_list* next;
} closed_list;

int search_in_open_list(Noh* node, open_list* my_open_list){
        Noh* current = open_list->node;
        if(node->valor == current->valor){
            return current;
        } else if(open_list->next == NULL){
            return 0;
        }
        open_list* next = open_list->next;
        search_in_open_list(node, next);
}
int add_to_open_list(Noh* node, Noh* father, int F, open_list* my_open_list){
    if(my_open_list->node == NULL){
        my_open_list->node = node;
        my_open_list->father = father;
        my_open_list->F = F;
        my_open_list->next = NULL;
    } else {
        open_list* current = my_open_list;
        if(F < current->F){
            open_list* new_open_list = (open_list *)malloc(sizeof(open_list));
            new_open_list->node = node;
            new_open_list->father = father;
            new_open_list->F = F;
            new_open_list->next = current;
            return 1;
        }
        open_list* previous = current;
        current = current->next;
        while(current->next != NULL){
            if(F < current->F){
                open_list* new_open_list = (open_list *)malloc(sizeof(open_list));
                new_open_list->node = node;
                new_open_list->father = father;
                new_open_list->F = F;
                previous->next = new_open_list;
                new_open_list->next = current;
            }
            current = current->next;
        }
        open_list* new_open_list = (open_list *)malloc(sizeof(open_list));
        new_open_list->node = node;
        new_open_list->father = father;
        new_open_list->F = F;
        new_open_list->next = NULL;
        current->next = new_open_list;
    }
}
int remove_from_open_list(Noh* node, open_list* my_open_list){
    open_list* current = my_open_list;
    open_list* previous = NULL;
    while(current->node->valor != node->valor){
        previous = current;
        current = current->next;
    }
    if(previous == NULL){
        my_open_list = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
}
void add_to_closed_list(Noh* node, Noh* father, closed_list* my_closed_list){
    if(my_closed_list->node == NULL){
        my_closed_list->node = node;
        my_closed_list->father = father;
        my_closed_list->next = NULL;
    } else {
        closed_list* current = my_closed_list;
        while(current->next != NULL){
            current = current->next;
        }
        closed_list* new_closed_list = (closed_list *)malloc(sizeof(closed_list));
        new_closed_list->node = node;
        new_closed_list->father = father;
        new_closed_list->next = NULL;
        current->next = new_closed_list;
    }
}

int heuristc(Noh current_noh, Noh goal, int size){
    int x1 = current_noh.valor % size;
    int y1 = current_noh.valor / size;

    int x2 = goal.valor % size;
    int y2 = goal.valor / size;

    return abs(x1 - x2) + abs(y1 - y2);
}

int cost_to_use_Node(Noh* current, Noh* goal, int* visitados, int size){
    int i, amount, heuristic, cost;
    i = 0;
    while(visitados[i] != NULL){
        amount++;
        i++;
    }
    int heuristic = heuristc(current, goal, size);
    
    cost = amount + heuristic + 1;
    return cost;
}


search_near_noh(Noh* node, int** adjMatrix, int numNos, open_list* my_open_list, int fim, int* visitados){
    for(int i = 0; i < numNos; i++){
        int node_adj_value = adjMatrix[node->valor][i];
        if(ehIgual(node_adj, 1)){
            Noh* node_adj = criarNoh(node_adj_value);
            if(search_in_open_list(node_adj, my_open_list){
                int cost = cost_to_use_Node(node_adj, fim, visitados, numNos);
                add_to_open_list(node_adj, node, cost, my_open_list);
            }
            int cost = cost_to_use_Node(node_adj, fim, visitados, numNos);
            add_to_open_list(node_adj, node, cost, my_open_list);
        }
    }
}


int findPath(int **adjMatrix, int numNos, int inicio, int fim, Noh **caminho, int *visitados)
{
    Noh* atual = criarNoh(inicio);

    open_list* my_open_list = (open_list *)malloc(sizeof(open_list));
    add_to_open_list(atual, NULL, 0, my_open_list);
    add_to_closed_list(atual, NULL, my_closed_list);
    visitados[0] = inicio;

    
    
    int final = ehIgual(fim, atual->valor);
    while(!final){
        search_near_noh(atual, adjMatrix, numNos, my_open_list, fim, visitados);

    }
    return 1;
}

int findPath(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados);




//////////////////////////////////////////// BUSCA EM PROFUNDIDADE //////////////////////////////////////

// int findPathRec(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados, int* contadorVisitacao) {
//     visitados[inicio] = ++(*contadorVisitacao);  // Marca a ordem de visita��o do n� atual

//     adicionarNoh(caminho, inicio);

//     if (ehIgual(inicio, fim)) {
//         return 1;
//     }

//     for (int i = 0; ehMenor(i, numNos); i++) {
//         // Usa visitados para verificar se o n� j� foi visitado
//         if (ehIgual(adjMatrix[inicio][i], 1) && ehIgual(visitados[i], 0)) {
//             if (findPathRec(adjMatrix, numNos, i, fim, caminho, visitados, contadorVisitacao)) {
//                 return 1;
//             }
//         }
//     }

//     removerNoh(caminho, inicio);
//     return 0;
// }

// int findPath(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados) {
//     int contadorVisitacao = 0;  // Inicializa o contador de visita��o

//     for (int i = 0; ehMenor(i, numNos); i++) {
//         visitados[i] = 0;  // Zera o vetor de n�s visitados
//     }

//     return findPathRec(adjMatrix, numNos, inicio, fim, caminho, visitados, &contadorVisitacao);
// }
