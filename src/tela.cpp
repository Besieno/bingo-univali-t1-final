/*
    tela.cpp - as três funções de terminal.
    gotoxy, textbackground e ansiBG vêm do codefun_GDB.h que o professor entregou
    (está em referencia/): é material dele, não é código de internet.
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

int ansiBG(int color) {
    if (color < 8)
        return 40 + color;
    else
        return 100 + (color - 8);
}

void textbackground(int newcolor) {
    if (newcolor == BLACK)
        cout << "\033[49m";
    else
        cout << "\033[" << ansiBG(newcolor) << "m";
    cout.flush();
}
