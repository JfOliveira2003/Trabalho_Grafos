#include "pathfinding.h"
#include "comparisons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

int heuristic(int node, int goal)
{
    return abs(node - goal);
}

int *createVetorInt(int size)
{
    return (int *)malloc(size * sizeof(int));
}

void addNoh(Noh **caminho, int valor)
{
    Noh *novoNoh = (Noh *)malloc(sizeof(Noh));
    novoNoh->valor = valor;
    novoNoh->proximo = NULL;

    if (*caminho == NULL)
    {
        *caminho = novoNoh;
    }
    else
    {
        Noh *temp = *caminho;
        while (temp->proximo != NULL)
        {
            temp = temp->proximo;
        }
        temp->proximo = novoNoh;
    }
}

// usado para a lista aberta e fechada
typedef struct
{
    int *nodes;
    int size;
} NodeList;

NodeList createNodeList(int capacity)
{
    NodeList list;
    list.nodes = (int *)malloc(capacity * sizeof(int));
    list.size = 0;
    return list;
}

void addNodeToList(NodeList *list, int node)
{
    list->nodes[list->size++] = node;
}

void removeNodeFromList(NodeList *list, int index)
{
    for (int i = index; ehMenor(i, list->size - 1); i++)
    {
        list->nodes[i] = list->nodes[i + 1];
    }
    list->size--;
}

int isNodeInList(NodeList *list, int node)
{
    for (int i = 0; ehMenor(i, list->size); i++)
    {
        if (ehIgual(list->nodes[i], node))
        {
            return 1;
        }
    }
    return 0;
}

void freeNodeList(NodeList *list)
{
    free(list->nodes);
    list->size = 0;
}

int findPath(int **adjMatrix, int numNodes, int startNode, int endNode, Noh **path, int *visitedNodes)
{
    int *costFromStart = createVetorInt(numNodes);
    int *heuristicCost = createVetorInt(numNodes);
    int *totalCost = createVetorInt(numNodes);
    int *previousNode = createVetorInt(numNodes);

    NodeList openList = createNodeList(numNodes);
    NodeList closedList = createNodeList(numNodes);

    int visitCounter = 0;

    // Initialize vectors
    for (int i = 0; ehMenor(i, numNodes); i++)
    {
        costFromStart[i] = INT_MAX;
        heuristicCost[i] = 0;
        totalCost[i] = INT_MAX;
        previousNode[i] = -1;
        visitedNodes[i] = 0;
    }

    // Initialize start node
    costFromStart[startNode] = 0;
    heuristicCost[startNode] = heuristic(startNode, endNode);
    totalCost[startNode] = heuristicCost[startNode];
    addNodeToList(&openList, startNode);
    visitedNodes[startNode] = ++visitCounter;

    while (openList.size > 0)
    {

        // find the node with the smallest totalCost in openList
        int currentNodeIndex = 0;
        int currentNode = openList.nodes[0];
        for (int i = 1; ehMenor(i, openList.size); i++)
        {
            if (ehMenor(totalCost[openList.nodes[i]], totalCost[currentNode]))
            {
                currentNode = openList.nodes[i];
                currentNodeIndex = i;
            }
        }

        // Remove da lista aberta e passa para a lista fechada
        removeNodeFromList(&openList, currentNodeIndex);
        addNodeToList(&closedList, currentNode);

        // End!
        if (ehIgual(currentNode,endNode))
        {
            // build path
            for (int node = endNode; ehDiferente(node,-1); node = previousNode[node])
            {
                addNoh(path, node);
            }

            free(costFromStart);
            free(heuristicCost);
            free(totalCost);
            free(previousNode);
            freeNodeList(&openList);
            freeNodeList(&closedList);
            return 1;
        }

        // Explore neighbors
        for (int neighbor = 0; ehMenor(neighbor, numNodes); neighbor++)
        {
            // check nó válido
            if (ehIgual(adjMatrix[currentNode][neighbor], 0) || isNodeInList(&closedList, neighbor))
                continue;

            int currentNodeCost = costFromStart[currentNode] + adjMatrix[currentNode][neighbor];

            if (ehIgual(isNodeInList(&openList, neighbor), 0))
            {
                addNodeToList(&openList, neighbor);
                heuristicCost[neighbor] = heuristic(neighbor, endNode);
            }

            if (ehMenor(currentNodeCost, costFromStart[neighbor]))
            {
                costFromStart[neighbor] = currentNodeCost;
                totalCost[neighbor] = costFromStart[neighbor] + heuristicCost[neighbor];
                previousNode[neighbor] = currentNode;
                visitedNodes[neighbor] = ++visitCounter;
            }
        }
    }

    // Default return, path not found
    free(costFromStart);
    free(heuristicCost);
    free(totalCost);
    free(previousNode);
    freeNodeList(&openList);
    freeNodeList(&closedList);
    return 0;
}
