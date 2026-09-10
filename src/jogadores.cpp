/*
    jogadores.cpp - pergunta o nome de cada jogador, um por vez.
    Usa getline em vez de cin >> porque cin >> para no primeiro espaço:
    "João Vitor" viraria dois jogadores.
    Equipe: ver comum.h
*/

#include <iostream>
#include <string>
#include "comum.h"
#include "jogadores.h"

using namespace std;

void nomearCartelas(string nome[])
{ // Inicia a função responsável por armazenar os nomes dos jogadores.
    for (int i = 0; i < TAM; i++)
    {
        cout << "Digite o nome do jogador da cartela " << (i + 1) << ": ";
        getline(cin, nome[i]); // getline lê a linha inteira, então nome composto não ocupa a vaga do jogador seguinte
    }
    cout << "\033c";
}
