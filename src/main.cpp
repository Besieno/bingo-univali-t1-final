/*
    Trabalho M1 - BINGO
    Universidade do Vale do Itajaí - Escola Politécnica
    Algoritmos e Programação II (22817)
    Professor: Rafael Ballotin Martins
    Setembro de 2026

    Equipe de desenvolvimento:
        Bernardo Sieno
        Henrique Dorow
        João Vitor Silva da Cruz
        Julio Cesar Manabe Padilha
        Nicolas do Vale Mezencio

    main.cpp - só a largada: liga o sorteador, reserva a memória das 5 cartelas
    e entrega o controle ao menu. Toda a lógica está nos outros arquivos.
*/

#include <string>
#include <time.h>
#include <stdlib.h>
#include "comum.h"
#include "menu.h"

using namespace std;

int main()
{
    srand(time(NULL));
    system("mode con: cols=120 lines=30 > nul 2>&1"); // Abre a janela no tamanho que o tabuleiro precisa (o > nul evita mensagem de erro se o terminal não aceitar)

    int cartela1[TAM][TAM], cartela2[TAM][TAM], cartela3[TAM][TAM], cartela4[TAM][TAM], cartela5[TAM][TAM];
    string nome[TAM];

    chamarMenu(cartela1, cartela2, cartela3, cartela4, cartela5, nome);

    return 0;
}
