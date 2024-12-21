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

};


int heuristc(int position_y_Noh, int position_x_Noh){

    int heuristic_manhattan, somatorioX, somatorioY;
    int i, j;
    for(i = position_y_Noh; i < 18; i++){
        somatorioX++;
        for(j = position_x_Noh; i < 18; j++)
            somatorioY++;
    }
    heuristic_manhattan = somatorioY + somatorioX;
    return heuristic_manhattan;
}

int cost_to_use_Node(int* visitados, Noh* noh){
    int i, amount, heuristic, cost;
    i = 0;
    while(visitados[i] != NULL){
        amount++;
        i++;
    }
    heuristic = heuristc(noh->posicaoY, noh->posicaoX);
    
    cost = amount + heuristc;
    return cost;
}

// int findPath(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados){
    
// }







//////////////////////////////////////////// BUSCA EM PROFUNDIDADE //////////////////////////////////////

int findPathRec(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados, int* contadorVisitacao) {
    visitados[inicio] = ++(*contadorVisitacao);  // Marca a ordem de visita��o do n� atual

    adicionarNoh(caminho, inicio);

    if (ehIgual(inicio, fim)) {
        return 1;
    }

    for (int i = 0; ehMenor(i, numNos); i++) {
        // Usa visitados para verificar se o n� j� foi visitado
        if (ehIgual(adjMatrix[inicio][i], 1) && ehIgual(visitados[i], 0)) {
            if (findPathRec(adjMatrix, numNos, i, fim, caminho, visitados, contadorVisitacao)) {
                return 1;
            }
        }
    }

    removerNoh(caminho, inicio);
    return 0;
}

int findPath(int** adjMatrix, int numNos, int inicio, int fim, Noh** caminho, int* visitados) {
    int contadorVisitacao = 0;  // Inicializa o contador de visita��o

    for (int i = 0; ehMenor(i, numNos); i++) {
        visitados[i] = 0;  // Zera o vetor de n�s visitados
    }

    return findPathRec(adjMatrix, numNos, inicio, fim, caminho, visitados, &contadorVisitacao);
}
