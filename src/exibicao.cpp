/*
    exibicao.cpp - desenho do tabuleiro.
    Nada aqui decide regra de jogo: só posiciona o cursor e escolhe a cor.
    Equipe: ver comum.h
*/

#include <iostream>
#include <string>
#include "exibicao.h"
#include "tela.h"

using namespace std;

void mostrarSorteado(int num, int x, int y)
{ // Exibe o número que acabou de ser sorteado.
    gotoxy(x, y);
    cout << "                                         "; // Limpa o que já estava escrito na linha, senão sobra pedaço do texto anterior
    gotoxy(x, y);
    cout << "Numero sorteado: " << num;
}

void pintarNumero(int mat[][TAM], int num, int x, int y, int cor)
{ // Inicia a função responsável por destacar um número específico dentro da cartela.
    for (int i = 0; i < TAM; i++)
    {
        int posicaoX = x; // Reinicia a posição horizontal para o início da linha.

        for (int j = 0; j < TAM; j++)
        {

            if (mat[i][j] == num)
            { // Verifica se o número da cartela corresponde ao número sorteado.
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

void exibir(int mat[][TAM], int contador, string nome[], int x, int y)
{ // Inicia a função responsável por exibir uma cartela e seu jogador.
    gotoxy(x, y);
    cout << "Cartela: " << contador; // Mostra o número de identificação da cartela.

    gotoxy(x, y + 1);
    cout << "Jogador: " << nome[contador - 1]; // Mostra o nome do jogador dono dessa cartela.

    for (int i = 0; i < TAM; i++)
    {
        gotoxy(x, y + 3 + i);

        for (int j = 0; j < TAM; j++)
        {
            cout << mat[i][j] << "\t";
        }
    }
}

void exibirSorteados(int lista[], int quantidade, int atual, int x, int y)
{ // Mostra todos os números já sorteados, em ordem crescente
    int coluna = 0;
    int linha = 0;

    gotoxy(x, y);
    cout << "Numeros sorteados (" << quantidade << "/75):";

    for (int i = 0; i < quantidade; i++)
    {
        gotoxy(x + coluna * 3, y + 1 + linha); // Cada número ocupa 3 colunas da tela

        if (lista[i] < 10)
        { // Alinha os números de um dígito com os de dois
            cout << " ";
        }

        if (lista[i] == atual) // O número que acabou de sair fica em vermelho
            textbackground(RED);
        else // Os que já tinham saído ficam em verde
            textbackground(GREEN);

        cout << lista[i];
        textbackground(BLACK);

        coluna++;
        if (coluna == 25)
        { // Quebra a linha a cada 25 números
            coluna = 0;
            linha++;
        }
    }
}
