/*
    exibicao.h - o que aparece na tela: as cartelas, o número da vez
    e a lista de todos os números já sorteados.
    Equipe: ver comum.h
*/

#ifndef EXIBICAO_H
#define EXIBICAO_H

#include <string>
#include "comum.h"

void exibir(int mat[][TAM], int contador, std::string nome[], int x, int y); // Desenha uma cartela e o dono dela
void pintarNumero(int mat[][TAM], int num, int x, int y, int cor);           // Pinta o número dentro da cartela
void mostrarSorteado(int num, int x, int y);                                 // Escreve "Numero sorteado: N"
void exibirSorteados(int lista[], int quantidade, int atual, int x, int y);  // Lista os sorteados em ordem crescente

#endif
