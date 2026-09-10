//Equipe de desenvolvimento: Bernardo Sieno, Henrique Dorow, João Vitor Silva da Cruz, Julio Cesar Manabe Padilha e Nicolas do Vale Mezencio
// Objetivo: implementar um bingo com 5 cartelas 5x5
// Mecânica: São geradas 5 cartelas DIFERENTES com intervalos diferentes em cada linha, a cada enter é sorteado um número e é verificado se ele está em alguma cartelas

// main.cpp - a largada: liga o sorteador, reserva a memória das 5 cartelas e
// roda o laço do menu. Toda a lógica está nos outros arquivos.
// Equipe: ver comum.h

#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "comum.h"
#include "menu.h"
#include "jogo.h"

using namespace std;

int main() {
	srand(time(NULL));
	int cartela1[TAM][TAM], cartela2[TAM][TAM], cartela3[TAM][TAM], cartela4[TAM][TAM], cartela5[TAM][TAM], numSorteados[(TAM*TAM)*3], opcao, quantidade=0;
	string nome[TAM];
    
	
	do {
	    opcao = chamarMenu();
    	switch(opcao) { // Executa a ação correspondente à opção
    		case 1:
    			jogar(cartela1, cartela2, cartela3, cartela4, cartela5, nome, numSorteados, quantidade);
    			break;
    		case 2:
    			sobre();
    			break;
    		case 3:
    			cout<<"\033c";
    			cout << "\n\tObrigado por jogar! :)"; // saída
    			break;
    		}
	} while(opcao != 3); // Continua exibindo o menu enquanto não escolher 3

	return 0;
}