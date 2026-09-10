/*
    sorteio.cpp - sorteia sem repetir e mantém a lista sempre ordenada.
    A lista fica ordenada por INSERÇÃO (o método do PDF de ordenação da disciplina):
    em vez de reordenar 75 números a cada rodada, empurra os maiores e encaixa o novo.
    Equipe: ver comum.h
*/

#include <stdlib.h>
#include "sorteio.h"

bool verificarSorteado(int vet[], int num, int quantidade)
{ // Verifica se um número já saiu em algum sorteio anterior
    for (int i = 0; i < quantidade; i++)
    { // Percorre somente as posições já preenchidas do vetor de sorteados
        if (vet[i] == num)
        {                // Verifica se o número gerado já foi cantado
            return true; // Número repetido encontrado
        }
    }
    return false; // Número não foi encontrado
}

void inserirEmOrdem(int lista[], int &quantidade, int valor)
{ // Guarda o número novo já na posição certa, deixando a lista sempre em ordem crescente
    int pos = quantidade;

    while (pos > 0 and lista[pos - 1] > valor)
    { // Empurra pra direita todos os que forem maiores que o valor novo
        lista[pos] = lista[pos - 1];
        pos--;
    }

    lista[pos] = valor;
    quantidade++; // quantidade vem por referência, então o contador também sobe para quem chamou
}

int sorteio(int numSorteados[], int &quantidade)
{
    int num;

    do
    {
        num = rand() % 75 + 1; // Gera um número aleatório entre 1 e 75.
    } while (verificarSorteado(numSorteados, num, quantidade)); // Verifica se o número gerado já foi sorteado.

    inserirEmOrdem(numSorteados, quantidade, num); // Guarda o número na lista ordenada e soma 1 na quantidade

    return num;
}
