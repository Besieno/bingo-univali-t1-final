/*
    cartela.cpp - geração da cartela: faixa por linha, sem repetir, em ordem crescente.
    Regra do enunciado: linha 1 = 1 a 15, linha 2 = 16 a 30, linha 3 = 31 a 45,
    linha 4 = 46 a 60, linha 5 = 61 a 75.
    Equipe: ver comum.h
*/

#include <stdlib.h>
#include "cartela.h"
#include "ordenacao.h"

bool verifyNum(int mat[][TAM], int linha, int num, int coluna) { // Função que verifica se um número já existe nas posições anteriores da linha atual
	for (int j = 0; j < coluna; j++) { // Percorre somente as colunas que já foram preenchidas
		if (mat[linha][j] == num) { // Verifica se o número gerado já apareceu na linha
			return true; // Número repetido encontrado
		} 
	}
	return false; // Número não foi encontrado
}

void gerarCartela(int mat[][TAM]) { // Função responsável por gerar uma cartela de Bingo
	int limiteFinal;
	int limiteInicial;
	int num;

	for (int i = 0; i < TAM; i++) { // Percorre cada linha da cartela
		limiteInicial = 15 * i + 1; // Define o primeiro número possível da linha - Linha 0: 1 - Linha 1: 16 - Linha 2: 31 - Linha 3: 46 - Linha 4: 61
		limiteFinal = 15*(1+i); // Define o último número possível da linha - Linha 0: 15 - Linha 1: 30 - Linha 2: 45 - Linha 3: 60 - Linha 4: 75
		for (int j = 0; j < TAM; j++) { 
			do {
				mat[i][j] = limiteInicial + rand() % (limiteFinal - limiteInicial + 1); // Gera um número aleatório dentro do intervalo definido para a linha atual
				num = mat[i][j];
			} while (verifyNum(mat, i, num, j)); // Gera outro número caso o número já tenha aparecido
		}
		bubblesort(mat, i); // Ordena a linha depois que seus 5 números foram gerados
	}

	
}

bool verifyCartela(int a[][TAM], int b[][TAM]) { // Retorna verdadeiro quando todas as posições das cartelas são iguais.
        for (int i = 0; i < TAM; i++) {
		    for (int j = 0; j < TAM; j++) {
			    if (a[i][j] != b[i][j] ) {
			        return false;
			    }
		    }
        }
    return true;
}
