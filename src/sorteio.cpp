/*
    sorteio.cpp - sorteia de 1 a 75 e só aceita número que ainda não saiu.
    Equipe: ver comum.h
*/

#include <stdlib.h>
#include "sorteio.h"

bool verificarSorteado(int vet[], int num, int quantidade) { // Função que verifica se um número já existe nas posições anteriores da linha atual
	for (int i = 0; i < quantidade; i++) { // Percorre somente as colunas que já foram preenchidas
		if (vet[i] == num) { // Verifica se o número gerado já apareceu na linha
			return true; // Número repetido encontrado
		} 
	}
	return false; // Número não foi encontrado
}

int sorteio(int numSorteados[], int quantidade) {
    int num;
    
    do {
        num = rand() % 75 + 1; // Gera um número aleatório entre 1 e 75.
    } while (verificarSorteado(numSorteados, num, quantidade)); // Verifica se o número gerado já foi sorteado.
    
    numSorteados[quantidade] = num;
    
    return num;
}
