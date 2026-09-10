/*
    cartela.h - tudo que cria e valida UMA cartela de bingo.
    Equipe: ver comum.h
*/

#ifndef CARTELA_H
#define CARTELA_H

#include "comum.h"

void bubblesort(int mat[][TAM], int linha);                                 // Ordena uma linha da cartela
bool numeroRepetidoNaLinha(int mat[][TAM], int linha, int num, int coluna); // Diz se o número já saiu nesta linha
void gerarCartela(int mat[][TAM]);                                          // Preenche a cartela inteira, linha por faixa
bool cartelasIguais(int a[][TAM], int b[][TAM]);                            // Diz se duas cartelas são a mesma

#endif
