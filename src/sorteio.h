/*
    sorteio.h - a urna: sortear de 1 a 75 sem repetir e guardar em ordem crescente.
    Equipe: ver comum.h
*/

#ifndef SORTEIO_H
#define SORTEIO_H

bool verificarSorteado(int vet[], int num, int quantidade);      // Diz se o número já foi cantado
void inserirEmOrdem(int lista[], int &quantidade, int valor);    // Guarda o número já na posição certa
int sorteio(int numSorteados[], int &quantidade);                // Sorteia um número novo e guarda

#endif
