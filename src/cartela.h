/*
    cartela.h - cria e valida UMA cartela de bingo.
    Equipe: ver comum.h
*/

#ifndef CARTELA_H
#define CARTELA_H

#include "comum.h"

bool verifyNum(int mat[][TAM], int linha, int num, int coluna); // o número já saiu nesta linha?
void gerarCartela(int mat[][TAM]);                              // preenche a cartela, linha por faixa
bool verifyCartela(int a[][TAM], int b[][TAM]);                 // duas cartelas são a mesma?

#endif
