//#ifndef CONSOLE_H
//#define CONSOLE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <limits>

using namespace std;

#define BLACK        0
#define BLUE         1
#define GREEN        2
#define AQUA         3
#define RED          4
#define PURPLE       5
#define YELLOW       6
#define WHITE        7
#define GRAY         8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTAQUA    11
#define LIGHTRED     12
#define LIGHTPURPLE  13
#define LIGHTYELLOW  14
#define BRIGHTWHITE  15

// Posiciona o cursor (terminal ANSI conta linhas/colunas a partir de 1)
void gotoxy(int x, int y)
{
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    cout.flush();
}

void clrscr(){
    cout << "\033[2J\033[H";
    cout.flush();
}

void delay(unsigned long t) {
    usleep(t * 1000); // usleep usa microssegundos
}

int random(int max) {
    return (rand() % max);
}

void randomize() {
    srand((unsigned) time(NULL));
}

void pausar(){
    if (cin.rdbuf()->in_avail() > 0)
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.get();
}

// Mapeia codigos 0-15 (estilo Windows) para codigos ANSI
static int ansiFG(int c) {
    static const int fg[16] = {30,34,32,36,31,35,33,37,90,94,92,96,91,95,93,97};
    return fg[c % 16];
}
static int ansiBG(int c) {
    static const int bg[16] = {40,44,42,46,41,45,43,47,100,104,102,106,101,105,103,107};
    return bg[c % 16];
}

void textcolor(int forecolor, int backcolor) {
    if (backcolor == BLACK)
        cout << "\033[" << ansiFG(forecolor) << ";49m";
    else
        cout << "\033[" << ansiFG(forecolor) << ";" << ansiBG(backcolor) << "m";
    cout.flush();
}

void textbackground(int newcolor) {
    if (newcolor == BLACK)
        cout << "\033[49m";
    else
        cout << "\033[" << ansiBG(newcolor) << "m";
    cout.flush();
}

void movimenta (int tecla, int *x, int *y)
{
    switch(tecla)
    {
        case 72: gotoxy(*x,--*y); break;      //cima
        case 80: gotoxy(*x,++*y); break;     //baixo
        case 75: gotoxy(--*x,*y); break;  //esquerda
        case 77: gotoxy(++*x,*y); break;   //direita
    }
}

//#endif