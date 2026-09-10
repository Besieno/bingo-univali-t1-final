/*
    tela.cpp - implementação das três funções de terminal.
    gotoxy, textbackground e ansiBG vieram do codefun_GDB.h que o professor
    entregou: é material dele, não é código de internet.
    Equipe: ver comum.h
*/

#include <iostream>
#include "tela.h"

using namespace std;

void gotoxy(int x, int y) // Inicia a função responsável por posicionar o cursor em uma coordenada específica do terminal.
{
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H"; // Envia o comando ANSI para mover o cursor para a posição indicada.
    cout.flush();
}

int ansiBG(int cor)
{ // Converte o código de cor do codefun.h do professor para o código ANSI de fundo
    int bg[16] = {40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107};
    return bg[cor % 16];
}

void textbackground(int newcolor)
{
    if (newcolor == BLACK)
        cout << "\033[49m";
    else
        cout << "\033[" << ansiBG(newcolor) << "m";
    cout.flush();
}
