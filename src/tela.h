/*
    tela.h - conversa com o terminal: onde escrever e de que cor.
    Equipe: ver comum.h
*/

#ifndef TELA_H
#define TELA_H

#define BLACK 0
#define RED 1
#define GREEN 2

void gotoxy(int x, int y);         // Move o cursor para a coluna x, linha y
int ansiBG(int color);             // Converte o código de cor no código ANSI de fundo
void textbackground(int newcolor); // Liga a cor de fundo (BLACK volta ao normal)

#endif
