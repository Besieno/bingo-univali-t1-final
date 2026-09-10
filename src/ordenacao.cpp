/*
    ordenacao.cpp - as duas versões do Bubble Sort.
    A variável cond é o sinalizador de troca: se uma passagem inteira não trocou
    nada, a sequência já está ordenada e o laço para antes da hora.
    Equipe: ver comum.h
*/

#include "ordenacao.h"

void bubblesort(int mat[][TAM], int linha) { // Inicia o algoritmo Bubble Sort para ordenar uma linha da cartela.
	int i, j, cond, temp;
	cond = 1; // Indica que ainda pode ser necessário realizar trocas
	
	for (i=TAM-1; (i >= 1) && (cond == 1); i--) { // Percorre a linha do final para o início (TAM-1 representa o último índice da linha)
	
		cond = 0; // Assume que não haverá mais trocas nesta passagem
		
		for (j=0; j < i ; j++) { // Compara elementos vizinhos da linha
			if (mat[linha][j+1] < mat[linha][j]) { // Verifica se os elementos estão fora de ordem
				temp = mat[linha][j];
				mat[linha][j] = mat[linha][j+1];
				mat[linha][j+1] = temp;
				cond = 1; // Indica que houve uma troca
			}
		}
	}
}

void bubblesort(int vet[], int n) {
    int i, j, cond, temp;
    cond = 1;
    for (i=n-1; (i >= 1) && (cond == 1); i--) {
        cond = 0;
        for (j=0; j < i ;j++) {
            if (vet[j+1] < vet[j]) {
                temp = vet[j];
                vet[j] = vet[j+1];
                vet[j+1] = temp;
                cond = 1;
            }
        }
    }
}
