#include "pathfinding.h"
#include "comparisons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>  // Para definir o valor de INF
#include <math.h>


int heuristic(int node_y, int node_x, int end_y, int end_x){

    return abs(node_x - end_x) + abs(node_y - end_y);
}
///////////////////////////// ALGORITMO A* /////////////////////////

typedef struct PriorityQueue {
    int* nodes;            
    int* priorities;       
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->nodes = (int*)malloc(capacity * sizeof(int));
    pq->priorities = (int*)malloc(capacity * sizeof(int));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void push(PriorityQueue* pq, int node, int priority) {
    if (pq->size == pq->capacity) return;
    pq->nodes[pq->size] = node;
    pq->priorities[pq->size] = priority;
    pq->size++;
}

int pop(PriorityQueue* pq) {
    int minIndex = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->priorities[i] < pq->priorities[minIndex]) {
            minIndex = i;
        }
    }
    int minNode = pq->nodes[minIndex];
    pq->size--;
    for (int i = minIndex; i < pq->size; i++) {
        pq->nodes[i] = pq->nodes[i + 1];
        pq->priorities[i] = pq->priorities[i + 1];
    }
    return minNode;
}

int contains(PriorityQueue* pq, int node) {
    for (int i = 0; i < pq->size; i++) {
        if (pq->nodes[i] == node) return 1;
    }
    return 0;
}

void addNodeToPath(Noh** caminho, int valor) {
    Noh* novoNoh = (Noh*)malloc(sizeof(Noh));
    novoNoh->valor = valor;
    novoNoh->proximo = *caminho;
    *caminho = novoNoh;
}

int findPath(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados) {
    int* gScores = (int*)malloc(numNos * sizeof(int));
    int* fScores = (int*)malloc(numNos * sizeof(int));
    int* cameFrom = (int*)malloc(numNos * sizeof(int));
    
    for (int i = 0; i < numNos; i++) {
        gScores[i] = INT_MAX;
        fScores[i] = INT_MAX;
        cameFrom[i] = -1;
        visitados[i] = 0;
    }

    gScores[inicio] = 0;
    fScores[inicio] = heuristic(0, inicio, 0, fim); // Update with correct node coordinates if needed

    PriorityQueue* openSet = createPriorityQueue(numNos);
    push(openSet, inicio, fScores[inicio]);

    while (openSet->size > 0) {
        int current = pop(openSet);
        visitados[current] = 1;

        if (current == fim) {
            *caminho = NULL;
            while (current != -1) {
                addNodeToPath(caminho, current);
                current = cameFrom[current];
            }
            free(gScores);
            free(fScores);
            free(cameFrom);
            free(openSet->nodes);
            free(openSet->priorities);
            free(openSet);
            return 1; // Path found
        }

        for (int neighbor = 0; neighbor < numNos; neighbor++) {
            if (adjMatrix[current][neighbor] > 0 && !visitados[neighbor]) {
                int tentative_gScore = gScores[current] + adjMatrix[current][neighbor];
                if (tentative_gScore < gScores[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScores[neighbor] = tentative_gScore;
                    fScores[neighbor] = gScores[neighbor] + heuristic(0, neighbor, 0, fim); // Update coordinates if needed

                    if (!contains(openSet, neighbor)) {
                        push(openSet, neighbor, fScores[neighbor]);
                    }
                }
            }
        }
    }

    free(gScores);
    free(fScores);
    free(cameFrom);
    free(openSet->nodes);
    free(openSet->priorities);
    free(openSet);
    return 0; // Path not found
}
