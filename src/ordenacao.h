/*
    ordenacao.h - Bubble Sort, nas DUAS formas que o jogo precisa.

    É o mesmo nome de função recebendo parâmetros diferentes: uma ordena uma LINHA
    de uma cartela (matriz), a outra ordena o VETOR dos números já sorteados.
    Isso é SOBRECARGA DE FUNÇÃO - tem um PDF só sobre isso na disciplina.
    Quem escolhe qual das duas roda é o compilador, pelo tipo do argumento.

    Equipe: ver comum.h
*/

#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "comum.h"

void bubblesort(int mat[][TAM], int linha); // ordena a linha indicada de uma cartela
void bubblesort(int vet[], int n);          // ordena as n primeiras posições de um vetor

#endif
