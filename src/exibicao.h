/*
    exibicao.h - o que aparece na tela: as cartelas, a lista de sorteados
    e a marcação colorida dos números.
    Equipe: ver comum.h
*/

#ifndef EXIBICAO_H
#define EXIBICAO_H

#include <string>
#include "comum.h"

void mostrarSorteado(int num, int numSorteados[], int quantidade);            // lista todos os sorteados, em ordem
void apagarSorteado();                                                        // limpa a área da mensagem
void pintarNumero(int mat[][TAM], int num, int x, int y, int cor);            // pinta o número dentro da cartela
void exibir(int mat[][TAM], int contador, std::string nome[], int x, int y);  // desenha uma cartela e o dono dela

#endif
