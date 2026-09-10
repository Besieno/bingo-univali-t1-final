/*
    menu.cpp - laço do menu e validação da opção.
    O enunciado é explícito: o programa só encerra pela opção Sair,
    então tudo que não for 3 volta pro menu no fim.
    Equipe: ver comum.h
*/

#include <iostream>
#include <string>
#include "menu.h"
#include "jogo.h"

using namespace std;

void lerOpcao(int &opcao)
{ // Função responsável por ler e validar a opção escolhida no menu
    do
    {
        cin >> opcao;

        if (cin.fail())
        { // Não veio um número: o usuário digitou letra, símbolo ou encerrou a entrada
            if (cin.eof())
            { // Não há mais nada pra ler, então não adianta perguntar de novo
                opcao = 3;
                return;
            }
            cin.clear(); // Tira o cin do estado de erro, senão nenhuma leitura seguinte funciona
            opcao = 0;   // Valor inválido de propósito, pra repetir a pergunta
        }

        cin.ignore(1000, '\n'); // Descarta o resto da linha, incluindo o Enter

        if (opcao < 1 or opcao > 3)
        { // Verifica se a opção está fora do intervalo válido
            cout << endl
                 << "\tValor inválido, escolha uma das opções válidas: ";
        }
    } while (opcao < 1 or opcao > 3); // Repete enquanto a opção for inválida
}

void sobre()
{ // Função responsável por exibir as informações do trabalho
    cout << "\033c";
    cout << "\n\tEquipe de desenvolvimento: \n\t\tBernardo Sieno\n\t\tHenrique Dorow\n\t\tJoão Vitor Silva da Cruz\n\t\tJulio Cesar Manabe Padilha\n\t\tNicolas do Vale Mezencio\n"; // créditos
    cout << "\n\tProfessor: Rafael Ballotin Martins - Algoritmos e Programação 2";
    cout << "\n\tSetembro de 2026.\n";
    cout << "\n\n\tEnter para continuar...\n";
    cin.ignore(1000, '\n');
}

void chamarMenu(int mat1[][TAM], int mat2[][TAM], int mat3[][TAM], int mat4[][TAM], int mat5[][TAM], string nome[])
{ // Função responsável por controlar o menu principal
    int opcao = 0;

    do
    { // Mantém o menu funcionando até o usuário escolher Sair
        cout << "\033c";
        cout << "---------------------------------------------------\n";
        cout << "       1- Jogar\n";
        cout << "       2- Sobre\n";
        cout << "       3- Sair\n";
        cout << "\n       Escolha uma opcao: ";
        lerOpcao(opcao); // Lê e valida a opção escolhida
        switch (opcao)
        { // Executa a ação correspondente à opção
        case 1:
            jogar(mat1, mat2, mat3, mat4, mat5, nome);
            break;
        case 2:
            sobre();
            break;
        case 3:
            cout << "\033c";
            cout << "\n\tObrigado por jogar! :)"; // saída
            break;
        }
    } while (opcao != 3); // Continua exibindo o menu enquanto não escolher 3
}
