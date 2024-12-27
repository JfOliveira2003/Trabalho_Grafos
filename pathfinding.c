#include "pathfinding.h"
#include "comparisons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h> // Para definir o valor de INF
#include <math.h>

///////////////////////////// ALGORITMO A* /////////////////////////

typedef struct open_list
{
    Noh *node;
    Noh *father;
    int F;
    struct open_list *next;
} open_list;

typedef struct closed_list
{
    Noh *node;
    Noh *father;
    struct closed_list *next;
} closed_list;

int search_in_open_list(Noh *node, open_list *my_open_list)
{
    open_list *current = my_open_list;
    while (current != NULL)
    {
        if (node->valor == current->node->valor)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
void compare_F(Noh *node, open_list *my_open_list, int F)
{
    open_list *current = my_open_list;
    while (current != NULL)
    {
        if (node->valor == current->node->valor)
        {
            if (F < current->F)
            {
                current->F = F;

                current->father = node;
            }
            break;
        }
        current = current->next;
    }
}

void add_to_open_list(Noh *node, Noh *father, int F, open_list *my_open_list)
{
    open_list *new_node = (open_list *)malloc(sizeof(open_list));
    if (new_node == NULL)
    {
        // Handle memory allocation failure (e.g., return an error code)
        return;
    }
    new_node->node = node;
    new_node->father = father;
    new_node->F = F;
    new_node->next = NULL;

    // Insert in the correct position (sorted by F value)
    if (my_open_list->node == NULL || F < my_open_list->F)
    {
        new_node->next = my_open_list;
        //return 1;
    }

    open_list *current = my_open_list;
    while (current->next != NULL && F >= current->next->F)
    {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;

    //return 1;
    // if (my_open_list->node == NULL)
    // {
    //     my_open_list->node = node;
    //     my_open_list->father = father;
    //     my_open_list->F = F;
    //     my_open_list->next = NULL;
    //     return 1;
    // }
    // else
    // {
    //     open_list *current = my_open_list;
    //     if (F < current->F)
    //     {
    //         open_list *new_open_list = (open_list *)malloc(sizeof(open_list));
    //         new_open_list->node = node;
    //         new_open_list->father = father;
    //         new_open_list->F = F;
    //         new_open_list->next = current;
    //         return 1;
    //     }
    //     open_list *previous = current;
    //     current = current->next;
    //     while (current->next != NULL)
    //     {
    //         if (F == current->F)
    //         {
    //             if (node->valor < current->node->valor)
    //             {
    //                 open_list *new_open_list = (open_list *)malloc(sizeof(open_list));
    //                 new_open_list->node = node;
    //                 new_open_list->father = father;
    //                 new_open_list->F = F;
    //                 previous->next = new_open_list;
    //                 new_open_list->next = current;
    //                 return 1;
    //             }
    //             else if (node->valor > current->node->valor)
    //             {
    //                 open_list *new_open_list = (open_list *)malloc(sizeof(open_list));
    //                 new_open_list->node = node;
    //                 new_open_list->father = father;
    //                 new_open_list->F = F;
    //                 new_open_list->next = current->next;
    //                 current->next = new_open_list;
    //                 return 1;
    //             }
    //         }
    //         if (F < current->F)
    //         {
    //             open_list *new_open_list = (open_list *)malloc(sizeof(open_list));
    //             new_open_list->node = node;
    //             new_open_list->father = father;
    //             new_open_list->F = F;
    //             previous->next = new_open_list;
    //             new_open_list->next = current;
    //         }
    //         current = current->next;
    //     }
    //     open_list *new_open_list = (open_list *)malloc(sizeof(open_list));
    //     new_open_list->node = node;
    //     new_open_list->father = father;
    //     new_open_list->F = F;
    //     new_open_list->next = NULL;
    //     current->next = new_open_list;
    //     return 1;
}
void remove_from_open_list(Noh *node, open_list *my_open_list)
{
    open_list *current = my_open_list;
    open_list *prev = NULL;

    while (current != NULL && current->node->valor != node->valor) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        // Node not found in the list
        return;
    }

    if (prev == NULL) {
        my_open_list = current->next; 
    } else {
        prev->next = current->next;
    }
    free(prev);
    free(current); 
}
void add_to_closed_list(Noh *node, Noh *father, closed_list *my_closed_list)
{
    if (my_closed_list->node == NULL)
    {
        my_closed_list->node = node;
        my_closed_list->father = father;
        my_closed_list->next = NULL;
    }
    else
    {
        closed_list *current = my_closed_list;
        while (current->next != NULL)
        {
            current = current->next;
        }
        closed_list *new_closed_list = (closed_list *)malloc(sizeof(closed_list));
        new_closed_list->node = node;
        new_closed_list->father = father;
        new_closed_list->next = NULL;
        current->next = new_closed_list;
    }
}

int heuristc(Noh *current_noh, Noh *goal, int size)
{
    int x1 = current_noh->valor % size;
    int y1 = current_noh->valor / size;

    int x2 = goal->valor % size;
    int y2 = goal->valor / size;

    return abs(x1 - x2) + abs(y1 - y2);
}

int cost_to_use_Node(Noh *current, Noh *goal, int size, int count)
{
    int heuristic, cost;
    heuristic = heuristc(current, goal, size);

    cost = count + heuristic + 1;
    return cost;
}

void search_near_noh(Noh *node, int **adjMatrix, int numNos, open_list *my_open_list, Noh *fim, int *visitados, int count)
{
    for (int i = 0; i < numNos; i++)
    {
        int node_adj_value = adjMatrix[node->valor][i];
        if (ehIgual(node_adj_value, 1))
        {
            Noh *node_adj = criarNoh(node_adj_value);
            if (search_in_open_list(node_adj, my_open_list))
            {
                int cost = cost_to_use_Node(node_adj, fim, numNos, count);
                compare_F(node_adj, my_open_list, cost);
            }
            else if (!search_in_open_list(node_adj, my_open_list))
            {
                int cost = cost_to_use_Node(node_adj, fim, numNos, count);
                add_to_open_list(node_adj, node, cost, my_open_list);
            }
        }
    }
}

int findPath(int **adjMatrix, int numNos, int inicio, int fim, Noh **caminho, int *visitados)
{
    Noh *atual = criarNoh(inicio);
    Noh *goal = criarNoh(fim);

    open_list *my_open_list = (open_list *)malloc(sizeof(open_list));
    my_open_list->node = NULL; 
    my_open_list->next = NULL; 
    closed_list *my_closed_list = (closed_list *)malloc(sizeof(closed_list));
    my_closed_list->node = NULL; 
    my_closed_list->next = NULL; 
    add_to_open_list(atual, NULL, 0, my_open_list);
    remove_from_open_list(atual, my_open_list);
    add_to_closed_list(atual, NULL, my_closed_list);
    visitados[0] = inicio;
    int count = 1;

    int final = ehIgual(fim, atual->valor);
    while (!final)
    {
        search_near_noh(atual, adjMatrix, numNos, my_open_list, goal, visitados, count);
        atual = my_open_list->node;
        remove_from_open_list(atual, my_open_list);
        add_to_closed_list(atual, my_open_list->father, my_closed_list);
        final = ehIgual(fim, atual->valor);
        visitados[count] = atual->valor;
        count++;
    }
    return 1;
}

//////////////////////////////////////////// BUSCA EM PROFUNDIDADE //////////////////////////////////////

// int findPathRec(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados, int* contadorVisitacao) {
//     visitados[inicio] = ++(*contadorVisitacao);  // Marca a ordem de visita��o do n� atual

//     adicionarNoh(caminho, inicio);Par
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
