/*
    exibicao.cpp - desenho do tabuleiro. Nada aqui decide regra de jogo:
    só posiciona o cursor, escolhe a cor e imprime.
    A lista de sorteados é reordenada a cada rodada pelo bubblesort de vetor
    (ver ordenacao.h), então sai sempre em ordem crescente.
    Equipe: ver comum.h
*/

#include <iostream>
#include <string>
#include "exibicao.h"
#include "tela.h"
#include "ordenacao.h"

using namespace std;

void mostrarSorteado(int num, int numSorteados[], int quantidade) { // Exibe o número que acabou de ser sorteado.
    int coluna=0;
    gotoxy(0, 35); // Posiciona a mensagem do número sorteado na parte inferior da tela.
    bubblesort(numSorteados, quantidade);
    cout << "Números sorteados " << "(" << quantidade << "/75)\n";
    for (int i = 0; i < quantidade; i++) {
        if (numSorteados[i] < 10) {
            cout << "  ";
        } else {
            cout << " ";
        }
        
        if (numSorteados[i] == num) {
            textbackground(RED);
        }
        
        cout << numSorteados[i];
        textbackground(BLACK);
        
        coluna++;
        if (coluna == 25) {
            cout << endl;
            coluna = 0;
        }
    }
}

void apagarSorteado() { // Inicia a função responsável por apagar a mensagem do número sorteado.
    gotoxy(0, 35);
    cout << "";
}

void pintarNumero(int mat[][TAM], int num, int x, int y, int cor) { // Inicia a função responsável por destacar um número específico dentro da cartela.
    for (int i = 0; i < TAM; i++) {
        int posicaoX = x; // Reinicia a posição horizontal para o início da linha.

        for (int j = 0; j < TAM; j++) {

            if (mat[i][j] == num) { // Verifica se o número da cartela corresponde ao número sorteado.
                gotoxy(posicaoX, y + 3 + i);

                textbackground(cor); // Aplica a cor escolhida ao número.
                cout << mat[i][j];
                textbackground(BLACK);

                return; // Encerra a função após encontrar e pintar o número.
            }

            
            if (mat[i][j] < 10) // Calcula onde o próximo número estará depois do \t
                posicaoX++;
            else 
                posicaoX += 2;

            posicaoX = ((posicaoX / 8) + 1) * 8; // Calcula a próxima posição de acordo com a tabulação utilizada na exibição.
        }
    }
}

void exibir(int mat[][TAM], int contador, string nome[], int x, int y) { // Inicia a função responsável por exibir uma cartela e seu jogador.
    gotoxy(x, y);
    cout << "Cartela: " << contador; // Mostra o nome do jogador associado à cartela.

    gotoxy(x, y + 1);
    cout << "Jogador: " << nome[contador - 1];

    for (int i = 0; i < TAM; i++) {
        gotoxy(x, y + 3 + i);

        for (int j = 0; j < TAM; j++) {
            cout << mat[i][j] << "\t";
        }
    }
}
