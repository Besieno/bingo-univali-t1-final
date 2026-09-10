/*
    jogadores.cpp - pergunta o nome dos 5 jogadores, um por vez.
    Equipe: ver comum.h
*/

#include <iostream>
#include <string>
#include "comum.h"
#include "jogadores.h"

using namespace std;

void nomearCartelas(string nome[]) { // Inicia a função responsável por armazenar os nomes dos jogadores.
        for (int i = 0; i < TAM; i++) {
            cout << "\n\tDigite o nome do " << i+1 << "º jogador: ";
            cin >> nome[i];      
        }
        cout << "\033c";
}
