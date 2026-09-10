/*
    jogo.cpp - o maestro da partida. É a única função que conhece todas as outras:
    manda gerar as 5 cartelas diferentes, pede os nomes, desenha o tabuleiro,
    e aí repete "sorteia -> pinta -> espera Enter -> confere vencedor" até alguém ganhar.
    As coordenadas de tela estão todas juntas no começo, de propósito:
    mexer no layout é mexer em um lugar só.
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

void jogar(int mat1[][TAM], int mat2[][TAM], int mat3[][TAM], int mat4[][TAM], int mat5[][TAM], string nome[])
{                         // Função responsável por iniciar o jogo
    int numSorteados[75]; // Todos os números já cantados, guardados em ordem crescente
    int quantidade = 0;   // Quantos números já saíram nesta partida
    int ganhadores = 0;

    // Posição de cada parte do tabuleiro na tela, reunidas aqui pra ajustar o layout em um lugar só
    int x1 = 2, y1 = 1;
    int x2 = 42, y2 = 1;
    int x3 = 82, y3 = 1;
    int x4 = 2, y4 = 10;
    int x5 = 42, y5 = 10;
    int xSorteados = 2, ySorteados = 18;
    int xAviso = 2, yAviso = 23;

    cout << "\033c"; // Limpa a tela

    gerarCartela(mat1); // Gera cartela 1

    do
    {
        gerarCartela(mat2); // Gera cartela 2
    } while (cartelasIguais(mat1, mat2)); // verificar se as cartelas 1 e 2 são iguais
    do
    {
        gerarCartela(mat3); // Gera cartela 3
    } while (cartelasIguais(mat1, mat3) or cartelasIguais(mat2, mat3));
    do
    {
        gerarCartela(mat4); // Gera cartela 4
    } while (cartelasIguais(mat1, mat4) or cartelasIguais(mat2, mat4) or cartelasIguais(mat3, mat4));
    do
    {
        gerarCartela(mat5); // Gera cartela 5
    } while (cartelasIguais(mat1, mat5) or cartelasIguais(mat2, mat5) or cartelasIguais(mat3, mat5) or cartelasIguais(mat4, mat5));

    nomearCartelas(nome);

    exibir(mat1, 1, nome, x1, y1);
    exibir(mat2, 2, nome, x2, y2);
    exibir(mat3, 3, nome, x3, y3);
    exibir(mat4, 4, nome, x4, y4);
    exibir(mat5, 5, nome, x5, y5);

    gotoxy(xAviso, yAviso);
    cout << "Pressione Enter para comecar o sorteio...";
    cin.ignore(1000, '\n'); // Aguarda o Enter antes de cantar o primeiro número

    do
    {
        int num = sorteio(numSorteados, quantidade);

        mostrarSorteado(num, xAviso, yAviso);
        exibirSorteados(numSorteados, quantidade, num, xSorteados, ySorteados); // Lista em ordem crescente, com o atual destacado

        // Vermelho durante o sorteio
        pintarNumero(mat1, num, x1, y1, RED);
        pintarNumero(mat2, num, x2, y2, RED);
        pintarNumero(mat3, num, x3, y3, RED);
        pintarNumero(mat4, num, x4, y4, RED);
        pintarNumero(mat5, num, x5, y5, RED);

        // Espera apenas um Enter
        cin.ignore(1000, '\n');

        // Depois do Enter fica verde
        pintarNumero(mat1, num, x1, y1, GREEN);
        pintarNumero(mat2, num, x2, y2, GREEN);
        pintarNumero(mat3, num, x3, y3, GREEN);
        pintarNumero(mat4, num, x4, y4, GREEN);
        pintarNumero(mat5, num, x5, y5, GREEN);

        exibirSorteados(numSorteados, quantidade, 0, xSorteados, ySorteados); // 0 = nenhum número em destaque

        ganhadores = 0;

        if (cartelaCompleta(mat1, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[0] << " ganhou com a cartela 1!";
            ganhadores++;
        }

        if (cartelaCompleta(mat2, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[1] << " ganhou com a cartela 2!";
            ganhadores++;
        }

        if (cartelaCompleta(mat3, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[2] << " ganhou com a cartela 3!";
            ganhadores++;
        }

        if (cartelaCompleta(mat4, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[3] << " ganhou com a cartela 4!";
            ganhadores++;
        }

        if (cartelaCompleta(mat5, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[4] << " ganhou com a cartela 5!";
            ganhadores++;
        }

        if (ganhadores > 0)
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << "Pressione Enter para voltar ao menu...";
            cin.ignore(1000, '\n');
        }

    } while (ganhadores == 0 && quantidade < 75);
}
