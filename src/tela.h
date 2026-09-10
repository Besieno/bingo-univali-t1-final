/*
    tela.h - conversa com o terminal: onde escrever e de que cor.
    Equipe: ver comum.h
*/

#ifndef TELA_H
#define TELA_H

// Códigos de cor no mesmo padrão do codefun.h que o professor passou
#define BLACK 0
#define GREEN 2
#define RED 4

void gotoxy(int x, int y);       // Move o cursor para a coluna x, linha y
int ansiBG(int cor);             // Converte a cor do professor no código ANSI de fundo
void textbackground(int newcolor); // Liga a cor de fundo (BLACK volta ao normal)

#endif
