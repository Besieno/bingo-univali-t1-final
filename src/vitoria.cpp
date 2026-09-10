/*
    vitoria.cpp - percorre os 25 números da cartela e pergunta para o sorteio
    se cada um já foi cantado. Basta um que não saiu para a cartela não estar completa.
    Equipe: ver comum.h
*/

#include "vitoria.h"
#include "sorteio.h"

bool cartelaCompleta(int mat[][TAM], int numSorteados[], int quantidade)
{ // Informa que todos os números da cartela foram sorteados.
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            if (!verificarSorteado(numSorteados, mat[i][j], quantidade))
            {
                return false;
            }
        }
    }

    return true;
}
