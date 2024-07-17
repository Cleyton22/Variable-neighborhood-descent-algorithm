#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define KMAX 3

typedef struct {
    int identification;
    double coordinateX;
    double coordinateY;
    int profit;
} Vertice;




typedef struct {
    Vertice *vertices;
    int amountVertices;
    double travelCost;
    double totalLatency;
    int totalProfit;
    int depotPosition;
} Solution;




typedef struct NodeVertice {
    Vertice vertice;
    struct NodeVertice *next;
} NodeVertice;




typedef struct {
    NodeVertice *head;
    int size;
} ListVertice;




typedef struct NodeSolution {
    Solution *solution;
    struct NodeSolution *next;
} NodeSolution;




typedef struct {
    NodeSolution *head;
    int size;
} ListSolution;




ListVertice *createListVertice() {
    ListVertice *list = malloc(sizeof(ListVertice));
    list->head = NULL;
    list->size = 0;
    return list;
}




NodeVertice* addToListVertice(ListVertice *list, Vertice vertice, NodeVertice *lastNode) {
    NodeVertice *node = malloc(sizeof(NodeVertice));
    node->vertice = vertice;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        lastNode->next = node;
    }

    list->size++;
    return node;
}




void removeFromListVertice(ListVertice *list, Vertice vertice) {
    NodeVertice *current = list->head;
    NodeVertice *previous = NULL;

    while (current != NULL && current->vertice.identification != vertice.identification) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    list->size--;
}




int isListVerticeEmpty(ListVertice *list) {
    return list->size == 0;
}




ListSolution *createListSolution() {
    ListSolution *list = malloc(sizeof(ListSolution));
    list->head = NULL;
    list->size = 0;
    return list;
}




NodeSolution* addToListSolution(ListSolution *list, Solution *solution, NodeSolution *lastNode) {
    NodeSolution *node = malloc(sizeof(NodeSolution));
    node->solution = solution;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        lastNode->next = node;
    }

    list->size++;
    return node;
}




void removeFromListSolution(ListSolution *list, Solution *solution) {
    NodeSolution *current = list->head;
    NodeSolution *previous = NULL;

    while (current != NULL && current->solution != solution) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    list->size--;
}




int isListSolutionEmpty(ListSolution *list) {
    return list->size == 0;
}




void printInstanceData(Vertice *allVertices, int numVertices) {
    printf("Instancia completa:\n\n");
    for (int i = 0; i < numVertices; i++) {
        printf("%d %.1lf %.1lf %d\n", allVertices[i].identification, allVertices[i].coordinateX, allVertices[i].coordinateY, allVertices[i].profit);
    }
}




void printVertice(Vertice vertice){
    printf("Vertice %d\n", vertice.identification);
    printf("Posicao: X %.2lf Y %.2lf\n", vertice.coordinateX, vertice.coordinateY);
    printf("Lucro: %d\n\n", vertice.profit);
}




void printListVertice(ListVertice *list){
    for (NodeVertice *node = list->head; node != NULL; node = node->next) {
        printVertice(node->vertice);
    }
}




void printSolution(Solution *solution) {
    for (int i = 0; i < solution->amountVertices; i++) {
        printf("Vertice %d %.2lf %.2lf %d\n", solution->vertices[i].identification, solution->vertices[i].coordinateX, solution->vertices[i].coordinateY, solution->vertices[i].profit);
    }
    printf("\nCusto total: %.2lf\n", solution->travelCost);
    printf("Latencia total: %.2lf\n", solution->totalLatency);
    printf("Lucro total: %d\n", solution->totalProfit);
    printf("Quantidade de clientes atendidos: %d\n", solution->amountVertices - 1);
    printf("Posicao do deposito: %d\n", solution->depotPosition);
    printf("\n");
}




double verticesDistanceCalculator(Vertice verticeA, Vertice verticeB){
    double distanceAB = sqrt((verticeA.coordinateX - verticeB.coordinateX) * (verticeA.coordinateX - verticeB.coordinateX) + (verticeA.coordinateY - verticeB.coordinateY) * (verticeA.coordinateY - verticeB.coordinateY));
    return distanceAB;
}




void solutionTotalTravelCostAndProfit(Solution *solution){
    for (int i = 0; i < (solution->amountVertices); i ++){
        if (solution->amountVertices != (i+1)){
            solution->travelCost += verticesDistanceCalculator(solution->vertices[i], solution->vertices[i + 1]);
            solution->totalProfit += solution->vertices[i].profit;
        } else{
            solution->travelCost += verticesDistanceCalculator(solution->vertices[i], solution->vertices[0]);
            solution->totalProfit += solution->vertices[i].profit;
        }
    }
}




void sumOrSubtractVerticeLatency(Solution *solution, int verticePosition, int adjacentPosition, int option){
    if (solution->depotPosition == 0){
        switch (option){
        case 1:
            //Subtrair usando o anterior
            solution->totalLatency -= (((solution->amountVertices - 1) - (verticePosition - 1)) * verticesDistanceCalculator(solution->vertices[adjacentPosition], solution->vertices[verticePosition]));
            break;
        case 2:
            //Subtrair usando o próximo
            solution->totalLatency -= (((solution->amountVertices - 1) - (adjacentPosition - 1)) * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
            break;
        case 3:
            //Somar usando o anterior
            solution->totalLatency += (((solution->amountVertices - 1) - (verticePosition - 1)) * verticesDistanceCalculator(solution->vertices[adjacentPosition], solution->vertices[verticePosition]));
            break;
        case 4:
            //Somar usando o próximo
            solution->totalLatency += (((solution->amountVertices - 1) - (adjacentPosition - 1)) * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
            break;
        default:
            break;
        }
    } else {
        if (verticePosition < solution->depotPosition) {
            switch (option){
            case 1:
                //Subtrair usando o anterior
                solution->totalLatency -= ((solution->depotPosition - verticePosition) * verticesDistanceCalculator(solution->vertices[adjacentPosition], solution->vertices[verticePosition]));
                break;
            case 2:
                //Subtrair usando o próximo
                solution->totalLatency -= ((solution->depotPosition - adjacentPosition) * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
                break;
            case 3:
                //Somar usando o anterior
                solution->totalLatency += ((solution->depotPosition - verticePosition) * verticesDistanceCalculator(solution->vertices[adjacentPosition], solution->vertices[verticePosition]));
                break;
            case 4:
                //Somar usando o próximo
                solution->totalLatency += ((solution->depotPosition - adjacentPosition) * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
                break;
            default:
                break;
            }
        } else{
            switch (option){
            case 1:
                //Subtrair usando o anterior
                solution->totalLatency -= ((solution->depotPosition + (solution->amountVertices - verticePosition)) * verticesDistanceCalculator(solution->vertices[adjacentPosition], solution->vertices[verticePosition]));
                break;
            case 2:
                //Subtrair usando o próximo
                if (adjacentPosition != 0){
                    solution->totalLatency -= ((solution->depotPosition + (solution->amountVertices - adjacentPosition)) * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
                } else {
                    solution->totalLatency -= (solution->depotPosition * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
                }
                break;
            case 3:
                //Somar usando o anterior
                solution->totalLatency += ((solution->depotPosition + (solution->amountVertices - verticePosition)) * verticesDistanceCalculator(solution->vertices[adjacentPosition], solution->vertices[verticePosition]));
                break;
            case 4:
                //Somar usando o próximo
                if (adjacentPosition != 0){
                    solution->totalLatency += ((solution->depotPosition + (solution->amountVertices - adjacentPosition)) * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
                } else {
                    solution->totalLatency += (solution->depotPosition * verticesDistanceCalculator(solution->vertices[verticePosition], solution->vertices[adjacentPosition]));
                }
                break;
            default:
                break;
            }
        }
    }
}




void solutionTotalLatency(Solution *solution){
    for (int i = 0; i < solution->amountVertices; i++){
        if (solution->vertices[i].identification == 1){
            solution->depotPosition = i;
            break;
        }
    }

    for (int i = 0; i < solution->amountVertices; i++){
        if (i != solution->depotPosition){
            if ((i - 1) == -1){
                sumOrSubtractVerticeLatency(solution, i, solution->amountVertices - 1, 3);
            }else{
                sumOrSubtractVerticeLatency(solution, i, i - 1, 3);
            }
        }
    }
}




int strictDominance(Solution *solutionA, Solution *solutionB){
    if (solutionA->travelCost < solutionB->travelCost && solutionA->totalLatency < solutionB->totalLatency && solutionA->totalProfit > solutionB->totalProfit){
        return 1;
    } else{
        return 0;
    }
}




int dominance(Solution *solutionA, Solution *solutionB){
    if (solutionA->travelCost <= solutionB->travelCost && solutionA->totalLatency < solutionB->totalLatency && solutionA->totalProfit >= solutionB->totalProfit){
        return 1;
    } else if(solutionA->travelCost < solutionB->travelCost && solutionA->totalLatency <= solutionB->totalLatency && solutionA->totalProfit >= solutionB->totalProfit){
        return 1;
    } else if(solutionA->travelCost <= solutionB->travelCost && solutionA->totalLatency <= solutionB->totalLatency && solutionA->totalProfit > solutionB->totalProfit){
        return 1;
    } else{
        return 0;
    }
}




Solution* copySolution(Solution *originalSolution){
    Solution *copiedSolution = malloc(sizeof(Solution));
    copiedSolution->amountVertices = originalSolution->amountVertices;
    copiedSolution->totalProfit = originalSolution->totalProfit;
    copiedSolution->travelCost = originalSolution->travelCost;
    copiedSolution->totalLatency = originalSolution->totalLatency;
    copiedSolution->depotPosition = originalSolution->depotPosition;

    copiedSolution->vertices = malloc((copiedSolution->amountVertices) * sizeof(Vertice));

    for (int i = 0; i < copiedSolution->amountVertices; i++){
        copiedSolution->vertices[i] = originalSolution->vertices[i];
    }

    return copiedSolution;
}




Solution copySolutionForPFIAC(Solution *originalSolution){
    Solution copiedSolution;
    copiedSolution.amountVertices = originalSolution->amountVertices;
    copiedSolution.totalProfit = originalSolution->totalProfit;
    copiedSolution.totalLatency = originalSolution->totalLatency;
    copiedSolution.depotPosition = originalSolution->depotPosition;

    copiedSolution.vertices = malloc((copiedSolution.amountVertices) * sizeof(Vertice));

    for (int i = 0; i < copiedSolution.amountVertices; i++){
        copiedSolution.vertices[i] = originalSolution->vertices[i];
    }

    copiedSolution.travelCost = originalSolution->travelCost + verticesDistanceCalculator(copiedSolution.vertices[copiedSolution.amountVertices - 1], copiedSolution.vertices[0]);

    return copiedSolution;
}




Solution* paretoFrontInitialAproximationConstructor(int numVertices, Vertice *allVertices, double alpha){
    Solution *paretoFrontIA = malloc((numVertices - 1) * sizeof(Solution));

    int currentPFIASolution = 0;

    Solution *solution = malloc(sizeof(Solution));
    solution->travelCost = 0;
    solution->totalLatency = 0;
    solution->totalProfit = 0;
    solution->vertices = malloc(numVertices * sizeof(Vertice));
    solution->vertices[0] = allVertices[0];
    solution->amountVertices = 1;
    solution->depotPosition = 0;

    ListVertice *candidatesList = createListVertice();
    NodeVertice *lastNode = NULL;

    for (int i = 1; i < numVertices; i++) {
        lastNode = addToListVertice(candidatesList, allVertices[i], lastNode);
    }

    while (candidatesList->size > 0){
        double gmin = DBL_MAX;
        double gmax = DBL_MIN;

        for (NodeVertice *node = candidatesList->head; node != NULL; node = node->next){
            double cost = verticesDistanceCalculator(solution->vertices[solution->amountVertices - 1], node->vertice);

            double g = cost / node->vertice.profit;

            if(g < gmin){
                gmin = g;
            }
            if(g > gmax){
                gmax = g;
            }
        }

        double threshold = gmin + alpha * (gmax - gmin);

        Vertice *restrictedCandidatesList = malloc((numVertices - solution->amountVertices) * sizeof(Vertice));

        int sizeRCL = 0;

        for (NodeVertice *node = candidatesList->head; node != NULL; node = node->next){
            double cost = verticesDistanceCalculator(solution->vertices[solution->amountVertices - 1], node->vertice);

            double g = cost / node->vertice.profit;

            if(g <= threshold){
                restrictedCandidatesList[sizeRCL] = node->vertice;

                sizeRCL++;
            }
        }

        int randomIndex = rand() % sizeRCL;

        Vertice selectedVertice = restrictedCandidatesList[randomIndex];

        solution->vertices[solution->amountVertices] = selectedVertice;
        solution->travelCost += verticesDistanceCalculator(solution->vertices[solution->amountVertices - 1], selectedVertice);
        solution->totalProfit += selectedVertice.profit;
        solution->amountVertices++;

        double latency = 0;

        for (int i = 0; i < (solution->amountVertices - 1); i++){
            latency += verticesDistanceCalculator(solution->vertices[i], solution->vertices[i+1]);
        }

        solution->totalLatency += latency;

        removeFromListVertice(candidatesList, selectedVertice);

        paretoFrontIA[currentPFIASolution] = copySolutionForPFIAC(solution);

        currentPFIASolution++;

        free(restrictedCandidatesList);
    }

    free(candidatesList);
    free(solution->vertices);
    free(solution);

    return paretoFrontIA;
}




void twoExchange(Solution *solution, int verticePositionA, int verticePositionB, Vertice verticeA, Vertice verticeB){
    if (verticePositionA != solution->depotPosition && verticePositionB != solution->depotPosition){
        int previousPositionA = verticePositionA - 1;

        if (previousPositionA == -1){
            previousPositionA = (solution->amountVertices - 1);
        }

        int nextPositionA = verticePositionA + 1;

        Vertice temporaryVertice = verticeB;

        solution->travelCost -= verticesDistanceCalculator(solution->vertices[previousPositionA], solution->vertices[verticePositionA]);
        solution->travelCost -= verticesDistanceCalculator(solution->vertices[verticePositionA], solution->vertices[nextPositionA]);

        sumOrSubtractVerticeLatency(solution, verticePositionA, previousPositionA, 1);

        if (nextPositionA != verticePositionB && nextPositionA != solution->depotPosition){
            sumOrSubtractVerticeLatency(solution, verticePositionA, nextPositionA, 2);
        }

        int previousPositionB = verticePositionB - 1;
        int nextPositionB = verticePositionB + 1;

        if (nextPositionB == solution->amountVertices){
            nextPositionB = 0;
        }

        if (previousPositionB != verticePositionA){
            solution->travelCost -= verticesDistanceCalculator(solution->vertices[previousPositionB], solution->vertices[verticePositionB]);
        }

        solution->travelCost -= verticesDistanceCalculator(solution->vertices[verticePositionB], solution->vertices[nextPositionB]);

        sumOrSubtractVerticeLatency(solution, verticePositionB, previousPositionB, 1);

        if (nextPositionB != solution->depotPosition){
            sumOrSubtractVerticeLatency(solution, verticePositionB, nextPositionB, 2);
        }

        solution->vertices[verticePositionB] = verticeA;
        solution->vertices[verticePositionA] = temporaryVertice;

        solution->travelCost += verticesDistanceCalculator(solution->vertices[previousPositionA], solution->vertices[verticePositionA]);
        solution->travelCost += verticesDistanceCalculator(solution->vertices[verticePositionA], solution->vertices[nextPositionA]);

        sumOrSubtractVerticeLatency(solution, verticePositionA, previousPositionA, 3);
        sumOrSubtractVerticeLatency(solution, verticePositionB, previousPositionB, 3);

        if (nextPositionA != verticePositionB && nextPositionA != solution->depotPosition){
            sumOrSubtractVerticeLatency(solution, verticePositionA, nextPositionA, 4);
        }

        if (nextPositionB != solution->depotPosition){
            sumOrSubtractVerticeLatency(solution, verticePositionB, nextPositionB, 4);
        }

        if (previousPositionB != verticePositionA){
            solution->travelCost += verticesDistanceCalculator(solution->vertices[previousPositionB], solution->vertices[verticePositionB]);
        }

        solution->travelCost += verticesDistanceCalculator(solution->vertices[verticePositionB], solution->vertices[nextPositionB]);
    } else {
        solution->totalLatency = 0;
        int previousPositionA = verticePositionA - 1;

        if (previousPositionA == -1){
            previousPositionA = (solution->amountVertices - 1);
        }

        int nextPositionA = verticePositionA + 1;

        Vertice temporaryVertice = verticeB;

        solution->travelCost -= verticesDistanceCalculator(solution->vertices[previousPositionA], solution->vertices[verticePositionA]);
        solution->travelCost -= verticesDistanceCalculator(solution->vertices[verticePositionA], solution->vertices[nextPositionA]);

        int previousPositionB = verticePositionB - 1;
        int nextPositionB = verticePositionB + 1;

        if (nextPositionB == solution->amountVertices){
            nextPositionB = 0;
        }

        if (previousPositionB != verticePositionA){
            solution->travelCost -= verticesDistanceCalculator(solution->vertices[previousPositionB], solution->vertices[verticePositionB]);
        }

        solution->travelCost -= verticesDistanceCalculator(solution->vertices[verticePositionB], solution->vertices[nextPositionB]);

        solution->vertices[verticePositionB] = verticeA;
        solution->vertices[verticePositionA] = temporaryVertice;

        if (solution->vertices[verticePositionA].identification == 1){
            solution->depotPosition = verticePositionA;
        }

        if (solution->vertices[verticePositionB].identification == 1){
            solution->depotPosition = verticePositionB;
        }

        solution->travelCost += verticesDistanceCalculator(solution->vertices[previousPositionA], solution->vertices[verticePositionA]);
        solution->travelCost += verticesDistanceCalculator(solution->vertices[verticePositionA], solution->vertices[nextPositionA]);

        if (previousPositionB != verticePositionA){
            solution->travelCost += verticesDistanceCalculator(solution->vertices[previousPositionB], solution->vertices[verticePositionB]);
        }

        solution->travelCost += verticesDistanceCalculator(solution->vertices[verticePositionB], solution->vertices[nextPositionB]);

        for (int i = 0; i < solution->amountVertices; i++){
            if (i != solution->depotPosition){
                if ((i - 1) == -1){
                    sumOrSubtractVerticeLatency(solution, i, solution->amountVertices - 1, 3);
                }else{
                    sumOrSubtractVerticeLatency(solution, i, i - 1, 3);
                }
            }
        }
    }
}




Solution* neighborhoodN1(Solution *solution){
    if (solution->amountVertices == 3){
        Vertice verticeA = solution->vertices[1];
        Vertice verticeB = solution->vertices[2];

        solution->vertices[1] = verticeB;
        solution->vertices[2] = verticeA;

        solution->totalLatency = 0;
        solution->totalLatency += (verticesDistanceCalculator(solution->vertices[0], solution->vertices[1]) * 2);
        solution->totalLatency += verticesDistanceCalculator(solution->vertices[1], solution->vertices[2]);

        return solution;
    } else{
        int verticePositionA = rand() % solution->amountVertices;
        int verticePositionB = rand() % solution->amountVertices;

        while (verticePositionA == verticePositionB){
            verticePositionB = rand() % solution->amountVertices;
        }

        if (verticePositionA > verticePositionB){
            int temporary = verticePositionA;
            verticePositionA = verticePositionB;
            verticePositionB = temporary;
        }

        Solution *neighbor = copySolution(solution);

        twoExchange(neighbor, verticePositionA, verticePositionB, neighbor->vertices[verticePositionA], neighbor->vertices[verticePositionB]);

        if (strictDominance(neighbor, solution) == 1){
            solution->travelCost = neighbor->travelCost;
            solution->totalLatency = neighbor->totalLatency;
            solution->depotPosition = neighbor->depotPosition;

            Vertice temporaryVertice = solution->vertices[verticePositionA];
            solution->vertices[verticePositionA] = solution->vertices[verticePositionB];
            solution->vertices[verticePositionB] = temporaryVertice;
        }

        free(neighbor->vertices);
        free(neighbor);
        return solution;
    }
}




Solution* neighborhoodN2(Solution *solution, Vertice *allVertices, int numVertices){
    if (solution->amountVertices == 2){
        Solution *neighbor = copySolution(solution);

        int randomNewVertice = rand() % numVertices;

        while (randomNewVertice == 0 || allVertices[randomNewVertice].identification == neighbor->vertices[1].identification){
            randomNewVertice = rand() % numVertices;
        }

        neighbor->vertices[1] = allVertices[randomNewVertice];
        neighbor->totalLatency = verticesDistanceCalculator(neighbor->vertices[0], neighbor->vertices[1]);
        neighbor->travelCost = (verticesDistanceCalculator(neighbor->vertices[0], neighbor->vertices[1]) * 2);
        neighbor->totalProfit = neighbor->vertices[1].profit;

        if (strictDominance(neighbor, solution) == 1){
            solution->travelCost = neighbor->travelCost;
            solution->totalLatency = neighbor->totalLatency;
            solution->totalProfit = neighbor->totalProfit;
            solution->vertices[1] = neighbor->vertices[1];
        }

        free(neighbor->vertices);
        free(neighbor);
        return solution;
    } else{
        int *containedVertices = calloc(numVertices + 1, sizeof(int));

        for (int i = 0; i < solution->amountVertices; i++){
            containedVertices[solution->vertices[i].identification] = 1;
        }

        int *notContainedVertices = malloc(((numVertices - 1) - (solution->amountVertices - 1)) * sizeof(int));

        int currentPosition = 0;

        for (int i = 2; i < (numVertices + 1); i++){
            if (containedVertices[i] == 0){
                notContainedVertices[currentPosition] = i;

                currentPosition++;

                if (currentPosition == ((numVertices - 1) - (solution->amountVertices - 1))){
                    break;
                }
            }
        }

        int oldVerticePosition = rand() % solution->amountVertices;

        while(oldVerticePosition == solution->depotPosition){
            oldVerticePosition = rand() % solution->amountVertices;
        }

        int previousPosition = oldVerticePosition - 1;
        int nextPosition = oldVerticePosition + 1;

        if (previousPosition == -1){
            previousPosition = (solution->amountVertices - 1);
        }

        if (nextPosition == solution->amountVertices){
            nextPosition = 0;
        }

        int newVertice = rand() % ((numVertices - 1) - (solution->amountVertices - 1));

        int newVerticePosition = notContainedVertices[newVertice] - 1;

        Solution *neighbor = copySolution(solution);

        neighbor->travelCost -= verticesDistanceCalculator(neighbor->vertices[previousPosition], neighbor->vertices[oldVerticePosition]);
        neighbor->travelCost -= verticesDistanceCalculator(neighbor->vertices[oldVerticePosition], neighbor->vertices[nextPosition]);
        neighbor->totalProfit -= neighbor->vertices[oldVerticePosition].profit;

        sumOrSubtractVerticeLatency(neighbor, oldVerticePosition, previousPosition, 1);

        if (nextPosition != solution->depotPosition){
            sumOrSubtractVerticeLatency(neighbor, oldVerticePosition, nextPosition, 2);
        }

        neighbor->vertices[oldVerticePosition] = allVertices[newVerticePosition];

        neighbor->travelCost += verticesDistanceCalculator(neighbor->vertices[previousPosition], neighbor->vertices[oldVerticePosition]);
        neighbor->travelCost += verticesDistanceCalculator(neighbor->vertices[oldVerticePosition], neighbor->vertices[nextPosition]);
        neighbor->totalProfit += neighbor->vertices[oldVerticePosition].profit;

        sumOrSubtractVerticeLatency(neighbor, oldVerticePosition, previousPosition, 3);

        if (nextPosition != solution->depotPosition){
            sumOrSubtractVerticeLatency(neighbor, oldVerticePosition, nextPosition, 4);
        }

        if (strictDominance(neighbor, solution) == 1){
            solution->travelCost = neighbor->travelCost;
            solution->totalLatency = neighbor->totalLatency;
            solution->totalProfit = neighbor->totalProfit;
            solution->vertices[oldVerticePosition] = allVertices[newVerticePosition];
        }

        free(neighbor->vertices);
        free(neighbor);
        free(containedVertices);
        free(notContainedVertices);
        return solution;
    }
}




int depotMovingVerification (Solution *solution, int firstEdgePosition, int secondEdgePosition){
    if (firstEdgePosition == solution->depotPosition || secondEdgePosition == solution->depotPosition){
        return 1;
    } else if (firstEdgePosition < secondEdgePosition){
        if (solution->depotPosition > firstEdgePosition && solution->depotPosition < secondEdgePosition){
            return 1;
        } else {
            return 0;
        }
    } else {
        if (solution->depotPosition > firstEdgePosition && solution->depotPosition > secondEdgePosition){
            return 1;
        } else if (solution->depotPosition < firstEdgePosition && solution->depotPosition < secondEdgePosition){
            return 1;
        } else {
            return 0;
        }
    }
}




void twoOptOperator(Solution *solution, double amountVerticesInRoute, int firstEdgePosition, int secondEdgePosition){
    Vertice *verticeMemory = malloc(amountVerticesInRoute * sizeof(Vertice));

    int previousFirstEdgePosition = firstEdgePosition - 1;

    if (previousFirstEdgePosition == -1){
        previousFirstEdgePosition = (solution->amountVertices - 1);
    }

    solution->travelCost -= verticesDistanceCalculator(solution->vertices[previousFirstEdgePosition], solution->vertices[firstEdgePosition]);

    int nextSecondEdgePosition = secondEdgePosition + 1;

    if (nextSecondEdgePosition == solution->amountVertices){
        nextSecondEdgePosition = 0;
    }

    solution->travelCost -= verticesDistanceCalculator(solution->vertices[secondEdgePosition], solution->vertices[nextSecondEdgePosition]);

    int routePosition = firstEdgePosition;
    int verticeMemoryPosition = 0;

    if (depotMovingVerification(solution, firstEdgePosition, secondEdgePosition) == 0){
        for (int i = 0; i < amountVerticesInRoute; i++){
            if (routePosition == solution->amountVertices){
                routePosition = 0;
            }

            verticeMemory[verticeMemoryPosition] = solution->vertices[routePosition];

            verticeMemoryPosition++;

            if ((routePosition - 1) != -1){
                sumOrSubtractVerticeLatency(solution, routePosition, routePosition - 1, 1);
            } else{
                sumOrSubtractVerticeLatency(solution, routePosition, (solution->amountVertices - 1), 1);
            }

            if (i == (amountVerticesInRoute - 1) && nextSecondEdgePosition != solution->depotPosition){
                sumOrSubtractVerticeLatency(solution, routePosition, nextSecondEdgePosition, 2);
            }

            routePosition++;
        }

        routePosition = firstEdgePosition;
        verticeMemoryPosition--;

        for (int i = 0; i < amountVerticesInRoute; i++){
            if (routePosition == solution->amountVertices){
                routePosition = 0;
            }

            solution->vertices[routePosition] = verticeMemory[verticeMemoryPosition];

            verticeMemoryPosition--;

            if ((routePosition - 1) != -1){
                sumOrSubtractVerticeLatency(solution, routePosition, routePosition - 1, 3);
            } else{
                sumOrSubtractVerticeLatency(solution, routePosition, (solution->amountVertices - 1), 3);
            }

            if (i == (amountVerticesInRoute - 1) && nextSecondEdgePosition != solution->depotPosition){
                sumOrSubtractVerticeLatency(solution, routePosition, nextSecondEdgePosition, 4);
            }

            routePosition++;
        }
    } else{
        for (int i = 0; i < amountVerticesInRoute; i++){
            if (routePosition == solution->amountVertices){
                routePosition = 0;
            }

            verticeMemory[verticeMemoryPosition] = solution->vertices[routePosition];

            verticeMemoryPosition++;
            routePosition++;
        }

        routePosition = firstEdgePosition;
        verticeMemoryPosition--;

        for (int i = 0; i < amountVerticesInRoute; i++){
            if (routePosition == solution->amountVertices){
                routePosition = 0;
            }

            solution->vertices[routePosition] = verticeMemory[verticeMemoryPosition];

            if (verticeMemory[verticeMemoryPosition].identification == 1){
                solution->depotPosition = routePosition;
            }

            verticeMemoryPosition--;
            routePosition++;
        }

        solution->totalLatency = 0;

        for (int i = 0; i < solution->amountVertices; i++){
            if (i != solution->depotPosition){
                if ((i - 1) == -1){
                    sumOrSubtractVerticeLatency(solution, i, solution->amountVertices - 1, 3);
                }else{
                    sumOrSubtractVerticeLatency(solution, i, i - 1, 3);
                }
            }
        }
    }

    solution->travelCost += verticesDistanceCalculator(solution->vertices[previousFirstEdgePosition], solution->vertices[firstEdgePosition]);
    solution->travelCost += verticesDistanceCalculator(solution->vertices[secondEdgePosition], solution->vertices[nextSecondEdgePosition]);

    free(verticeMemory);
}




Solution* neighborhoodN3(Solution *solution, double gama){
    if (solution->amountVertices == 3){
        Vertice verticeA = solution->vertices[1];
        Vertice verticeB = solution->vertices[2];

        solution->vertices[1] = verticeB;
        solution->vertices[2] = verticeA;

        solution->totalLatency = 0;
        solution->totalLatency += (verticesDistanceCalculator(solution->vertices[0], solution->vertices[1]) * 2);
        solution->totalLatency += verticesDistanceCalculator(solution->vertices[1], solution->vertices[2]);

        return solution;
    } else {
        if (2 > ((solution->amountVertices - 1) * gama)) {
            return solution;
        } else{
            int randomAmountToMove = rand() % solution->amountVertices;

            while (randomAmountToMove < 2 || randomAmountToMove > ((solution->amountVertices - 1) * gama)){
                randomAmountToMove = rand() % solution->amountVertices;
            }

            int initialPosition = rand() % solution->amountVertices;

            int firstEdgePosition = initialPosition;
            int secondEdgePosition = (firstEdgePosition + randomAmountToMove) - 1;

            if (secondEdgePosition > (solution->amountVertices - 1)){
                secondEdgePosition -= solution->amountVertices;
            }

            if (randomAmountToMove == 2 || randomAmountToMove == 3){
                if (firstEdgePosition > secondEdgePosition){
                    int temporary = firstEdgePosition;
                    firstEdgePosition = secondEdgePosition;
                    secondEdgePosition = temporary;
                }

                Solution *neighbor = copySolution(solution);

                twoExchange(neighbor, firstEdgePosition, secondEdgePosition, neighbor->vertices[firstEdgePosition], neighbor->vertices[secondEdgePosition]);

                if (strictDominance(neighbor, solution) == 1){
                    solution->travelCost = neighbor->travelCost;
                    solution->totalLatency = neighbor->totalLatency;
                    solution->depotPosition = neighbor->depotPosition;

                    for (int i = 0; i < solution->amountVertices; i++){
                        solution->vertices[i] = neighbor->vertices[i];
                    }
                }

                free(neighbor->vertices);
                free(neighbor);
                return solution;
            } else{
                Solution *neighbor = copySolution(solution);

                twoOptOperator(neighbor, randomAmountToMove, firstEdgePosition, secondEdgePosition);

                if (strictDominance(neighbor, solution) == 1){
                    solution->travelCost = neighbor->travelCost;
                    solution->totalLatency = neighbor->totalLatency;
                    solution->depotPosition = neighbor->depotPosition;

                    for (int i = 0; i < solution->amountVertices; i++){
                        solution->vertices[i] = neighbor->vertices[i];
                    }
                }

                free(neighbor->vertices);
                free(neighbor);
                return solution;
            }
        }
    }
}




int chooseAplhaValue(){
    int alphaPicked;

    printf("Selecione um dos valores abaixo para alpha:\n\n");
    printf("1 - alpha = 0.00\n");
    printf("2 - alpha = 0.25\n");
    printf("3 - alpha = 0.50\n");
    printf("4 - alpha = 0.75\n");
    printf("5 - alpha = 1.00\n\n");
    printf("Digite sua opcao: ");
    scanf("%d", &alphaPicked);

    while (alphaPicked < 1 || alphaPicked > 5){
        printf("\nErro!\n\n");
        printf("Digite sua opcao: ");
        scanf("%d", &alphaPicked);
    }

    return alphaPicked;
}




int chooseGamaValue(){
    int gamaPicked;

    printf("\nSelecione um dos valores abaixo para gama:\n\n");
    printf("1 - gama = 0.25\n");
    printf("2 - gama = 0.50\n");
    printf("3 - gama = 0.75\n");
    printf("4 - gama = 1.00\n\n");
    printf("Digite sua opcao: ");
    scanf("%d", &gamaPicked);

    while (gamaPicked < 1 || gamaPicked > 4){
        printf("\nErro!\n\n");
        printf("Digite sua opcao: ");
        scanf("%d", &gamaPicked);
    }

    printf("\n");

    return gamaPicked;
}




double assignAlphaValue(int alphaPicked){
    double alpha;

    switch (alphaPicked){
        case 1:
            alpha = 0;
            break;
        case 2:
            alpha = 0.25;
            break;
        case 3:
            alpha = 0.50;
            break;
        case 4:
            alpha = 0.75;
            break;
        case 5:
            alpha = 1;
            break;
        default:
            break;
    }

    return alpha;
}




double assignGamaValue(int gamaPicked){
    double gama;

    switch (gamaPicked){
        case 1:
            gama = 0.25;
            break;
        case 2:
            gama = 0.50;
            break;
        case 3:
            gama = 0.75;
            break;
        case 4:
            gama = 1;
            break;
        default:
            break;
    }

    return gama;
}




void onlyAddNonDominatedSolutions(ListSolution *allNonDominatedSolutions, Solution *candidateSolution){
    if(isListSolutionEmpty(allNonDominatedSolutions)){
        Solution *newSolution = copySolution(candidateSolution);
        NodeSolution *lastNode = NULL;
        lastNode = addToListSolution(allNonDominatedSolutions, newSolution, lastNode);
    } else{
        int proceed = 0;

        for (NodeSolution *current = allNonDominatedSolutions->head; current != NULL; current = current->next) {
            if((dominance(current->solution, candidateSolution) == 1)){
                proceed = 1;
                break;
            }

            if(current->next == NULL){
                Solution *newSolution = copySolution(candidateSolution);
                NodeSolution *newNode = malloc(sizeof(NodeSolution));
                newNode->solution = newSolution;
                newNode->next = NULL;
                current->next = newNode;
                allNonDominatedSolutions->size++;
                break;
            }
        }

        if(proceed == 0){
            NodeSolution *current2 = allNonDominatedSolutions->head;
            NodeSolution *previous2 = NULL;

            while (current2 != NULL) {
                NodeSolution *nextNode = current2->next;
                if((dominance(candidateSolution, current2->solution) == 1)){
                    if(previous2 == NULL){
                        allNonDominatedSolutions->head = current2->next;
                    }else{
                        previous2->next = current2->next;
                    }
                    free(current2->solution->vertices);
                    free(current2->solution);
                    free(current2);
                    allNonDominatedSolutions->size--;
                } else {
                    previous2 = current2;
                }
                current2 = nextNode;
            }
        }
    }
}




void vndMainLoop(Vertice *allVertices, int numVertices, double alpha, double gama){
    clock_t start, end, pauseStart, pauseEnd;
    double cpuTimeUsed;

    start = clock();

    Solution *paretoFrontIA = paretoFrontInitialAproximationConstructor(numVertices, allVertices, alpha);

    pauseStart = clock();

    FILE *outfile = fopen("1.1 ValoresCustoViajemSoluçõesIniciais.in", "w");
    if (outfile == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    FILE *outfile2 = fopen("1.2 ValoresLatênciaSoluçõesIniciais.in", "w");
    if (outfile2 == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    FILE *outfile3 = fopen("1.3 ValoresLucroSoluçõesIniciais.in", "w");
    if (outfile2 == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < (numVertices - 1); i++){
        fprintf(outfile, "%.2lf\n", paretoFrontIA[i].travelCost);
        fprintf(outfile2, "%.2lf\n", paretoFrontIA[i].totalLatency);
        fprintf(outfile3, "%d\n", paretoFrontIA[i].totalProfit);
    }

    fclose(outfile);
    fclose(outfile2);
    fclose(outfile3);

    pauseEnd = clock();

    for (int i = 0; i < (numVertices - 1); i++){
        int k = 1;

        while (k <= KMAX){
            int dominance = 0;
            Solution *newSolution = copySolution(&paretoFrontIA[i]);

            switch (k){
                case 1:
                    if (newSolution->amountVertices > 2){
                        newSolution = neighborhoodN1(newSolution);
                    } else{
                        k = k + 1;
                        break;
                    }

                    dominance = strictDominance(newSolution, &paretoFrontIA[i]);

                    if (dominance == 1){
                        paretoFrontIA[i].travelCost = newSolution->travelCost;
                        paretoFrontIA[i].totalLatency = newSolution->totalLatency;
                        paretoFrontIA[i].totalProfit = newSolution->totalProfit;
                        paretoFrontIA[i].depotPosition = newSolution->depotPosition;

                        for (int j = 0; j < paretoFrontIA[i].amountVertices; j++){
                            paretoFrontIA[i].vertices[j] = newSolution->vertices[j];
                        }

                        k = 1;
                    } else{
                        k = k + 1;
                    }

                    break;
                case 2:
                    if (newSolution->amountVertices < numVertices){
                        newSolution = neighborhoodN2(newSolution, allVertices, numVertices);
                    } else{
                        k = k + 1;
                        break;
                    }

                    dominance = strictDominance(newSolution, &paretoFrontIA[i]);

                    if (dominance == 1){
                        paretoFrontIA[i].travelCost = newSolution->travelCost;
                        paretoFrontIA[i].totalLatency = newSolution->totalLatency;
                        paretoFrontIA[i].totalProfit = newSolution->totalProfit;
                        paretoFrontIA[i].depotPosition = newSolution->depotPosition;

                        for (int j = 0; j < paretoFrontIA[i].amountVertices; j++){
                            paretoFrontIA[i].vertices[j] = newSolution->vertices[j];
                        }

                        k = 1;
                    } else{
                        k = k + 1;
                    }

                    break;
                case 3:
                    if (newSolution->amountVertices > 2){
                        newSolution = neighborhoodN3(newSolution, gama);
                    } else{
                        k = k + 1;
                        break;
                    }

                    dominance = strictDominance(newSolution, &paretoFrontIA[i]);

                    if (dominance == 1){
                        paretoFrontIA[i].travelCost = newSolution->travelCost;
                        paretoFrontIA[i].totalLatency = newSolution->totalLatency;
                        paretoFrontIA[i].totalProfit = newSolution->totalProfit;
                        paretoFrontIA[i].depotPosition = newSolution->depotPosition;

                        for (int j = 0; j < paretoFrontIA[i].amountVertices; j++){
                            paretoFrontIA[i].vertices[j] = newSolution->vertices[j];
                        }

                        k = 1;
                    } else{
                        k = k + 1;
                    }

                    break;
                default:
                    break;
            }

            free(newSolution->vertices);
            free(newSolution);
        }
    }

    end = clock();

    ListSolution *allNonDominatedSolutions = createListSolution();

    for (int i = 0; i < (numVertices - 1); i++){
        onlyAddNonDominatedSolutions(allNonDominatedSolutions, &paretoFrontIA[i]);
    }

    FILE *outfile4 = fopen("2.1 ValoresCustoViajemSoluçõesFinais.in", "w");
    if (outfile4 == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    FILE *outfile5 = fopen("2.2 ValoresLatênciaSoluçõesFinais.in", "w");
    if (outfile5 == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    FILE *outfile6 = fopen("2.3 ValoresLucroSoluçõesFinais.in", "w");
    if (outfile6 == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    FILE *outfile7 = fopen("2.4 QuantidadeClientesSoluçõesFinais.in", "w");
    if (outfile7 == NULL) {
        printf("Nao foi possivel abrir o arquivo para escrita.\n");
        return;
    }

    for (NodeSolution *node = allNonDominatedSolutions->head; node != NULL; node = node->next) {
        fprintf(outfile4, "%.2lf\n", node->solution->travelCost);
        fprintf(outfile5, "%.2lf\n", node->solution->totalLatency);
        fprintf(outfile6, "%d\n", node->solution->totalProfit);
        fprintf(outfile7, "%d\n", node->solution->amountVertices);
    }

    fclose(outfile4);
    fclose(outfile5);
    fclose(outfile6);
    fclose(outfile7);

    cpuTimeUsed = ((double) (end - start - (pauseEnd - pauseStart))) / CLOCKS_PER_SEC;
    printf("VND levou %f segundos para executar \n", cpuTimeUsed);
}




int main() {
    srand(time(0));

    FILE *file = fopen("Y-berlin52.in", "r");
    if (file == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return 1;
    }

    int numVertices;
    fscanf(file, "%d", &numVertices);

    Vertice *allVertices = malloc(numVertices * sizeof(Vertice));
    for (int i = 0; i < numVertices; i++) {
        fscanf(file, "%d %lf %lf %d", &allVertices[i].identification, &allVertices[i].coordinateX, &allVertices[i].coordinateY, &allVertices[i].profit);
    }

    fclose(file);

    int alphaPicked = chooseAplhaValue();
    int gamaPicked = chooseGamaValue();
    double alpha = assignAlphaValue(alphaPicked);
    double gama = assignGamaValue(gamaPicked);

    vndMainLoop(allVertices, numVertices, alpha, gama);

    return 0;
}

