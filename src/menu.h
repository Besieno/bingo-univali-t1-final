/*
    menu.h - a porta de entrada: Jogar, Sobre e Sair.
    Nesta versão chamarMenu só DESENHA o menu e devolve a opção escolhida;
    quem decide o que fazer com ela é o main.
    Equipe: ver comum.h
*/

#ifndef MENU_H
#define MENU_H

int lerOpcao(int opcao); // lê e valida a opção (só devolve 1, 2 ou 3)
void sobre();            // tela com equipe, professor e mês/ano
int chamarMenu();        // desenha o menu e devolve a opção escolhida

#endif
