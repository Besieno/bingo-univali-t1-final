/*
    menu.cpp - desenho do menu, validação da opção e a tela Sobre.
    Equipe: ver comum.h
*/

#include <iostream>
#include "menu.h"

using namespace std;

int lerOpcao(int opcao) { // Função responsável por ler e validar a opção escolhida no menu
	do {
		cin >> opcao;
		if (cin.fail()) { // Não veio um número: o usuário digitou letra, símbolo ou encerrou a entrada
			if (cin.eof()) { // Não há mais nada pra ler, então não adianta perguntar de novo
				return 3;
			}
			cin.clear(); // Tira o cin do estado de erro, senão nenhuma leitura seguinte funciona
			cin.ignore(1000, '\n'); // Descarta o que foi digitado e não era número
			opcao = 0; // Valor inválido de propósito, pra repetir a pergunta
		}
		if (opcao < 1 or opcao > 3) { // Verifica se a opção está fora do intervalo válido
			cout << endl << "\tValor inválido, escolha uma das opções válidas: ";
		}
	} while (opcao < 1 or opcao > 3); // Repete enquanto a opção for inválida
	return opcao;
}

void sobre() { // Função responsável por exibir as informações do trabalho
	cout<<"\033c";
	cout << "\n\tEquipe de desenvolvimento: \n\t\tBernardo Sieno\n\t\tHenrique Dorow\n\t\tJoão Vitor Silva da Cruz\n\t\tJulio Cesar Manabe Padilha\n\t\tNicolas do Vale Mezencio\n"; // créditos
	cout << "\n\tProfessor: Rafael Ballotin Martins - Algoritmos e Programação 2";
	cout << "\n\tSetembro de 2026.\n";
	cout << "\n\n\tEnter para continuar...\n";
	cin.ignore();
	cin.ignore();
}

int chamarMenu() { // Função responsável por controlar o menu principal
		cout<<"\033c";
		cout << "---------------------------------------------------\n";
		cout << "       1- Jogar\n";
		cout << "       2- Sobre\n";
		cout << "       3- Sair\n";
		
		int opcao;
		cout << "\n\tEscolha uma opção: ";
		opcao = lerOpcao(opcao); // Lê e valida a opção escolhida
	return opcao;
}
