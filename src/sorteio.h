/*
    sorteio.h - a urna: sortear de 1 a 75 sem repetir.
    Equipe: ver comum.h
*/

#ifndef SORTEIO_H
#define SORTEIO_H

bool verificarSorteado(int vet[], int num, int quantidade); // o número já foi cantado?
int sorteio(int numSorteados[], int quantidade);            // sorteia um número novo e guarda

#endif
