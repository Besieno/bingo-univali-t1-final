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
        cin.ignore(1000, '\n'); // Descarta o Enter que sobrou do menu, senão o primeiro getline vem vazio
        for (int i = 0; i < TAM; i++) {
            cout << "\n\tDigite o nome do " << i+1 << "º jogador: ";
            getline(cin, nome[i]); // getline lê a linha inteira, então nome composto não ocupa a vaga do próximo
        }
        cout << "\033c";
}
