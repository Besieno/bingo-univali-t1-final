/*
    menu.h - a porta de entrada do programa: Jogar, Sobre e Sair.
    Equipe: ver comum.h
*/

#ifndef MENU_H
#define MENU_H

#include <string>
#include "comum.h"

void lerOpcao(int &opcao); // Lê a opção do menu e só devolve quando for válida
void sobre();              // Tela com equipe, professor e mês/ano
void chamarMenu(int mat1[][TAM], int mat2[][TAM], int mat3[][TAM], int mat4[][TAM], int mat5[][TAM], std::string nome[]);

#endif
