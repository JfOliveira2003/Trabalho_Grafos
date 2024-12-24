// Função principal recriada
int findPath(int **adjMatrix, int numNos, int inicio, int fim, Noh **caminho, int *visitados) {
    int *G = criarVetorInt(numNos);
    int *H = criarVetorInt(numNos);
    int *F = criarVetorInt(numNos);
    int *s = criarVetorInt(numNos);
    int *openList = criarVetorInt(numNos);
    int *closedList = criarVetorInt(numNos);

    if (!G || !H || !F || !s || !openList || !closedList) {
        printf("Erro de alocação de memória\n");
        return 0;
    }

    int openListSize = 0, closedListSize = 0;
    int contadorVisitacao = 0;

    for (int i = 0; i < numNos; i++) {
        G[i] = INT_MAX;
        H[i] = 0;
        F[i] = INT_MAX;
        s[i] = -1;
        openList[i] = -1;
        closedList[i] = -1;
        visitados[i] = 0;
    }

    G[inicio] = 0;
    H[inicio] = heuristic(inicio, fim);
    F[inicio] = G[inicio] + H[inicio];
    openList[openListSize++] = inicio;
    visitados[inicio] = ++contadorVisitacao;

    while (openListSize > 0) {
        // Encontrar o nó com menor F na openList
        int currentNodeIndex = 0;
        int currentNode = openList[0];
        for (int i = 1; i < openListSize; i++) {
            int node = openList[i];
            if (F[node] < F[currentNode]) {
                currentNode = node;
                currentNodeIndex = i;
            }
        }

        // Remover currentNode da openList
        for (int i = currentNodeIndex; i < openListSize - 1; i++) {
            openList[i] = openList[i + 1];
        }
        openListSize--;

        // Adicionar currentNode à closedList
        closedList[closedListSize++] = currentNode;

        if (currentNode == fim) {
            // Reconstruir o caminho
            int pathLength = 0;
            for (int node = fim; node != -1; node = s[node]) {
                pathLength++;
            }

            int *pathNodes = criarVetorInt(pathLength);
            if (!pathNodes) {
                printf("Erro de alocação de memória\n");
                free(G);
                free(H);
                free(F);
                free(s);
                free(openList);
                free(closedList);
                return 0;
            }

            int node = fim;
            int index = pathLength - 1;
            while (node != -1) {
                pathNodes[index--] = node;
                node = s[node];
            }

            for (int i = 0; i < pathLength; i++) {
                adicionarNoh(caminho, pathNodes[i]);
            }

            free(pathNodes);
            free(G);
            free(H);
            free(F);
            free(s);
            free(openList);
            free(closedList);
            return 1;
        }

        // Expandir os vizinhos do currentNode
        for (int i = 0; i < numNos; i++) {
            if (adjMatrix[currentNode][i] != 0) {
                int naListaFechada = 0;
                for (int j = 0; j < closedListSize; j++) {
                    if (closedList[j] == i) {
                        naListaFechada = 1;
                        break;
                    }
                }
                if (naListaFechada) continue;

                int tentativeG = G[currentNode] + adjMatrix[currentNode][i];

                int naListaAberta = 0;
                for (int j = 0; j < openListSize; j++) {
                    if (openList[j] == i) {
                        naListaAberta = 1;
                        break;
                    }
                }

                if (!naListaAberta) {
                    openList[openListSize++] = i;
                    H[i] = heuristic(i, fim);
                }

                if (tentativeG < G[i]) {
                    G[i] = tentativeG;
                    F[i] = G[i] + H[i];
                    s[i] = currentNode;
                    visitados[i] = ++contadorVisitacao;
                }
            }
        }
    }

    free(G);
    free(H);
    free(F);
    free(s);
    free(openList);
    free(closedList);

    return 0;
}