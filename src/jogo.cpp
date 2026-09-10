/*
    jogo.cpp - o maestro da partida. É a única função que conhece todas as outras:
    gera as 5 cartelas diferentes, pede os nomes, desenha o tabuleiro e repete
    "sorteia -> mostra -> pinta de vermelho -> Enter -> pinta de verde -> confere vencedor"
    até alguém completar a cartela ou os 75 números acabarem.
    Equipe: ver comum.h
*/

#include <iostream>
#include <string>
#include "jogo.h"
#include "cartela.h"
#include "jogadores.h"
#include "sorteio.h"
#include "exibicao.h"
#include "vitoria.h"
#include "tela.h"

using namespace std;

void jogar(int mat1[][TAM], int mat2[][TAM], int mat3[][TAM], int mat4[][TAM], int mat5[][TAM], string nome[], int numSorteados[], int quantidade) { // Função responsável por iniciar o jogo
	int ganhadores = 0;
	cout<<"\033c"; // Limpa a tela
	
	gerarCartela(mat1);  // Gera cartela 1
	
	do {
	    gerarCartela(mat2);  // Gera cartela 2
	} while (verifyCartela(mat1, mat2)); // verificar se as cartelas 1 e 2 são iguais
	do {
	    gerarCartela(mat3);  // Gera cartela 3
    } while (verifyCartela(mat1, mat3) or verifyCartela(mat2, mat3)); 
    do {
        gerarCartela(mat4);  // Gera cartela 4
    } while (verifyCartela(mat1, mat4) or verifyCartela(mat2, mat4) or verifyCartela(mat3, mat4));
    do {
	    gerarCartela(mat5);  // Gera cartela 5
    } while (verifyCartela(mat1, mat5) or verifyCartela(mat2, mat5) or verifyCartela(mat3, mat5) or verifyCartela(mat4, mat5));
    
    nomearCartelas(nome);
    
    gotoxy(55, 30);
    cout << "Enter para continuar...";

    exibir(mat1, 1, nome, 10, 2);
	exibir(mat2, 2, nome, 90, 2);
	exibir(mat3, 3, nome, 50, 12);
	exibir(mat4, 4, nome, 10, 22);
	exibir(mat5, 5, nome, 90, 22);
	
	cin.ignore(); // Aguarda o usuário antes de retornar ao menu
	cin.ignore();
	
	do {
        int num = sorteio(numSorteados, quantidade);
        quantidade++;
        
        mostrarSorteado(num, numSorteados, quantidade);
    
        // Vermelho durante o sorteio
        
        pintarNumero(mat1, num, 10, 2, RED);
        pintarNumero(mat2, num, 90, 2, RED);
        pintarNumero(mat3, num, 50, 12, RED);
        pintarNumero(mat4, num, 10, 22, RED);
        pintarNumero(mat5, num, 90, 22, RED);
    
        // Espera apenas um Enter
        cin.ignore();
    
        // Depois do Enter fica verde
        pintarNumero(mat1, num, 10, 2, GREEN);
        pintarNumero(mat2, num, 90, 2, GREEN);
        pintarNumero(mat3, num, 50, 12, GREEN);
        pintarNumero(mat4, num, 10, 22, GREEN);
        pintarNumero(mat5, num, 90, 22, GREEN);
    
        apagarSorteado();

        ganhadores = 0;
        
        
        cout << endl << endl << endl << endl;
        
        if (cartelaCompleta(mat1, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela do jogador " << nome[0] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat2, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela do jogador " << nome[1] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat3, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela do jogador " << nome[2] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat4, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela do jogador " << nome[3] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat5, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela do jogador " << nome[4] << " ganhou!";
            ganhadores++;
        }
        
        if (ganhadores == 1) {
            textbackground(GREEN);
            cout << "\nTem 1 ganhador!";
            cin.ignore();
            cin.ignore();
        } else if (ganhadores > 1) {
            textbackground(GREEN);
            cout << "\nTem " << ganhadores << " ganhadores!";
            cin.ignore();
            cin.ignore();
        }
        
    } while (ganhadores == 0 && quantidade < 75);
}
