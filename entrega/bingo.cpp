/*
    Trabalho M1 - BINGO
    Universidade do Vale do Itajaí - Escola Politécnica
    Algoritmos e Programação II (22817)
    Professor: Rafael Ballotin Martins
    Setembro de 2026

    Equipe de desenvolvimento:
        Bernardo Sieno
        Henrique Dorow
        João Vitor Silva da Cruz
        Julio Cesar Manabe Padilha
        Nicolas do Vale Mezencio

    Objetivo: implementar um bingo com 5 cartelas 5x5.
    Mecanica: sao geradas 5 cartelas DIFERENTES, com intervalos diferentes em cada
    linha; a cada Enter e sorteado um numero e conferido se ele esta em alguma cartela.

    Este arquivo foi montado a partir da pasta src/, onde o mesmo codigo esta
    separado em um arquivo por parte do jogo (tela, ordenacao, cartela, sorteio,
    vitoria, exibicao, jogadores, jogo, menu, main). O conteudo das sub-rotinas
    e identico: aqui elas aparecem todas juntas, na ordem em que compilam.
*/

#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define TAM 5 // A cartela é 5x5: 5 linhas de 5 números

// Códigos de cor no mesmo padrão do codefun.h que o professor passou
#define BLACK 0
#define RED 1
#define GREEN 2

// ------------------------------------------------------------------------
// TELA - posicionar o cursor e escolher a cor (do codefun_GDB.h do professor)
// ------------------------------------------------------------------------

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

// ------------------------------------------------------------------------
// ORDENACAO - Bubble Sort em duas sobrecargas: linha de cartela e vetor de sorteados
// ------------------------------------------------------------------------

void bubblesort(int mat[][TAM], int linha) { // Inicia o algoritmo Bubble Sort para ordenar uma linha da cartela.
	int i, j, cond, temp;
	cond = 1; // Indica que ainda pode ser necessário realizar trocas
	
	for (i=TAM-1; (i >= 1) && (cond == 1); i--) { // Percorre a linha do final para o início (TAM-1 representa o último índice da linha)
	
		cond = 0; // Assume que não haverá mais trocas nesta passagem
		
		for (j=0; j < i ; j++) { // Compara elementos vizinhos da linha
			if (mat[linha][j+1] < mat[linha][j]) { // Verifica se os elementos estão fora de ordem
				temp = mat[linha][j];
				mat[linha][j] = mat[linha][j+1];
				mat[linha][j+1] = temp;
				cond = 1; // Indica que houve uma troca
			}
		}
	}
}

void bubblesort(int vet[], int n) {
    int i, j, cond, temp;
    cond = 1;
    for (i=n-1; (i >= 1) && (cond == 1); i--) {
        cond = 0;
        for (j=0; j < i ;j++) {
            if (vet[j+1] < vet[j]) {
                temp = vet[j];
                vet[j] = vet[j+1];
                vet[j+1] = temp;
                cond = 1;
            }
        }
    }
}

// ------------------------------------------------------------------------
// CARTELA - gerar a cartela: faixa por linha, sem repetir, em ordem crescente
// ------------------------------------------------------------------------

bool verifyNum(int mat[][TAM], int linha, int num, int coluna) { // Função que verifica se um número já existe nas posições anteriores da linha atual
	for (int j = 0; j < coluna; j++) { // Percorre somente as colunas que já foram preenchidas
		if (mat[linha][j] == num) { // Verifica se o número gerado já apareceu na linha
			return true; // Número repetido encontrado
		} 
	}
	return false; // Número não foi encontrado
}

void gerarCartela(int mat[][TAM]) { // Função responsável por gerar uma cartela de Bingo
	int limiteFinal;
	int limiteInicial;
	int num;

	for (int i = 0; i < TAM; i++) { // Percorre cada linha da cartela
		limiteInicial = 15 * i + 1; // Define o primeiro número possível da linha - Linha 0: 1 - Linha 1: 16 - Linha 2: 31 - Linha 3: 46 - Linha 4: 61
		limiteFinal = 15*(1+i); // Define o último número possível da linha - Linha 0: 15 - Linha 1: 30 - Linha 2: 45 - Linha 3: 60 - Linha 4: 75
		for (int j = 0; j < TAM; j++) { 
			do {
				mat[i][j] = limiteInicial + rand() % (limiteFinal - limiteInicial + 1); // Gera um número aleatório dentro do intervalo definido para a linha atual
				num = mat[i][j];
			} while (verifyNum(mat, i, num, j)); // Gera outro número caso o número já tenha aparecido
		}
		bubblesort(mat, i); // Ordena a linha depois que seus 5 números foram gerados
	}

	
}

bool verifyCartela(int a[][TAM], int b[][TAM]) { // Retorna verdadeiro quando todas as posições das cartelas são iguais.
        for (int i = 0; i < TAM; i++) {
		    for (int j = 0; j < TAM; j++) {
			    if (a[i][j] != b[i][j] ) {
			        return false;
			    }
		    }
        }
    return true;
}

// ------------------------------------------------------------------------
// SORTEIO - sortear de 1 a 75 sem repetir
// ------------------------------------------------------------------------

bool verificarSorteado(int vet[], int num, int quantidade) { // Função que verifica se um número já existe nas posições anteriores da linha atual
	for (int i = 0; i < quantidade; i++) { // Percorre somente as colunas que já foram preenchidas
		if (vet[i] == num) { // Verifica se o número gerado já apareceu na linha
			return true; // Número repetido encontrado
		} 
	}
	return false; // Número não foi encontrado
}

int sorteio(int numSorteados[], int quantidade) {
    int num;
    
    do {
        num = rand() % 75 + 1; // Gera um número aleatório entre 1 e 75.
    } while (verificarSorteado(numSorteados, num, quantidade)); // Verifica se o número gerado já foi sorteado.
    
    numSorteados[quantidade] = num;
    
    return num;
}

// ------------------------------------------------------------------------
// VITORIA - a cartela esta completa?
// ------------------------------------------------------------------------

bool cartelaCompleta(int mat[][TAM], int numSorteados[], int quantidade) { // Informa que todos os números da cartela foram sorteados.
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (!verificarSorteado(numSorteados, mat[i][j], quantidade)) {
                return false;
            }
        }
    }

    return true;
}

// ------------------------------------------------------------------------
// EXIBICAO - desenhar as cartelas, a lista de sorteados e a marcacao colorida
// ------------------------------------------------------------------------

void mostrarSorteado(int num, int numSorteados[], int quantidade) { // Exibe o número que acabou de ser sorteado.
    int coluna=0, linha=0;
    gotoxy(0, 35); // Posiciona a mensagem do número sorteado na parte inferior da tela.
    bubblesort(numSorteados, quantidade);
    cout << "Números sorteados " << "(" << quantidade << "/75)\n";
    for (int i = 0; i < quantidade; i++) {
        if (numSorteados[i] < 10) {
            cout << "  ";
        } else {
            cout << " ";
        }
        
        if (numSorteados[i] == num) {
            textbackground(RED);
        }
        
        cout << numSorteados[i];
        textbackground(BLACK);
        
        coluna++;
        if (coluna == 25) {
            cout << endl;
            coluna = 0;
            linha++;
        }
    }
}

void apagarSorteado() { // Inicia a função responsável por apagar a mensagem do número sorteado.
    gotoxy(0, 35);
    cout << "";
}

void pintarNumero(int mat[][TAM], int num, int x, int y, int cor) { // Inicia a função responsável por destacar um número específico dentro da cartela.
    for (int i = 0; i < TAM; i++) {
        int posicaoX = x; // Reinicia a posição horizontal para o início da linha.

        for (int j = 0; j < TAM; j++) {

            if (mat[i][j] == num) { // Verifica se o número da cartela corresponde ao número sorteado.
                gotoxy(posicaoX, y + 3 + i);

                textbackground(cor); // Aplica a cor escolhida ao número.
                cout << mat[i][j];
                textbackground(BLACK);

                return; // Encerra a função após encontrar e pintar o número.
            }

            
            if (mat[i][j] < 10) // Calcula onde o próximo número estará depois do \t
                posicaoX++;
            else 
                posicaoX += 2;

            posicaoX = ((posicaoX / 8) + 1) * 8; // Calcula a próxima posição de acordo com a tabulação utilizada na exibição.
        }
    }
}

void exibir(int mat[][TAM], int contador, string nome[], int x, int y) { // Inicia a função responsável por exibir uma cartela e seu jogador.
    gotoxy(x, y);
    cout << "Cartela: " << contador; // Mostra o nome do jogador associado à cartela.

    gotoxy(x, y + 1);
    cout << "Jogador: " << nome[contador - 1];

    for (int i = 0; i < TAM; i++) {
        gotoxy(x, y + 3 + i);

        for (int j = 0; j < TAM; j++) {
            cout << mat[i][j] << "\t";
        }
    }
}

// ------------------------------------------------------------------------
// JOGADORES - o nome do dono de cada cartela
// ------------------------------------------------------------------------

void nomearCartelas(string nome[]) { // Inicia a função responsável por armazenar os nomes dos jogadores.
        cin.ignore(1000, '\n'); // Descarta o Enter que sobrou do menu, senão o primeiro getline vem vazio
        for (int i = 0; i < TAM; i++) {
            cout << "\n\tDigite o nome do " << i+1 << "º jogador: ";
            getline(cin, nome[i]); // getline lê a linha inteira, então nome composto não ocupa a vaga do próximo
        }
        cout << "\033c";
}

// ------------------------------------------------------------------------
// JOGO - a partida: gera, pergunta, desenha, sorteia e confere o vencedor
// ------------------------------------------------------------------------

void jogar(int mat1[][TAM], int mat2[][TAM], int mat3[][TAM], int mat4[][TAM], int mat5[][TAM], string nome[], int numSorteados[], int quantidade) { // Função responsável por iniciar o jogo
	int ganhadores = 0;
	cout<<"\033c"; // Limpa a tela
	
	gerarCartela(mat1);  // Gera cartela 1
	
	do {
	    gerarCartela(mat2);  // Gera cartela 2
	} while (verifyCartela(mat1, mat2)); // verificar se as cartelas 1 e 2 são iguais
	do {
	    gerarCartela(mat3);  // Gera cartela 3
    } while (verifyCartela(mat1, mat3) or verifyCartela(mat2, mat3)); 
    do {
        gerarCartela(mat4);  // Gera cartela 4
    } while (verifyCartela(mat1, mat4) or verifyCartela(mat2, mat4) or verifyCartela(mat3, mat4));
    do {
	    gerarCartela(mat5);  // Gera cartela 5
    } while (verifyCartela(mat1, mat5) or verifyCartela(mat2, mat5) or verifyCartela(mat3, mat5) or verifyCartela(mat4, mat5));
    
    nomearCartelas(nome);
    
    gotoxy(55, 30);
    cout << "Enter para continuar...";

    exibir(mat1, 1, nome, 10, 2);
	exibir(mat2, 2, nome, 90, 2);
	exibir(mat3, 3, nome, 50, 12);
	exibir(mat4, 4, nome, 10, 22);
	exibir(mat5, 5, nome, 90, 22);
	
	cin.ignore(1000, '\n'); // Aguarda o Enter antes de começar o sorteio (com getline não sobra nada pra descartar)
	
	do {
        int num = sorteio(numSorteados, quantidade);
        quantidade++;
        
        mostrarSorteado(num, numSorteados, quantidade);
    
        // Vermelho durante o sorteio
        
        pintarNumero(mat1, num, 10, 2, RED);
        pintarNumero(mat2, num, 90, 2, RED);
        pintarNumero(mat3, num, 50, 12, RED);
        pintarNumero(mat4, num, 10, 22, RED);
        pintarNumero(mat5, num, 90, 22, RED);
    
        // Espera apenas um Enter
        cin.ignore();
    
        // Depois do Enter fica verde
        pintarNumero(mat1, num, 10, 2, GREEN);
        pintarNumero(mat2, num, 90, 2, GREEN);
        pintarNumero(mat3, num, 50, 12, GREEN);
        pintarNumero(mat4, num, 10, 22, GREEN);
        pintarNumero(mat5, num, 90, 22, GREEN);
    
        apagarSorteado();

        ganhadores = 0;
        
        
        cout << endl << endl << endl << endl;
        
        if (cartelaCompleta(mat1, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela 1 do jogador " << nome[0] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat2, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela 2 do jogador " << nome[1] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat3, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela 3 do jogador " << nome[2] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat4, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela 4 do jogador " << nome[3] << " ganhou!";
            ganhadores++;
        }
        
        if (cartelaCompleta(mat5, numSorteados, quantidade)) {
            textbackground(GREEN);
            cout << endl << "A cartela 5 do jogador " << nome[4] << " ganhou!";
            ganhadores++;
        }
        
        if (ganhadores == 1) {
            textbackground(GREEN);
            cout << "\nTem 1 ganhador!";
            cin.ignore();
            cin.ignore();
        } else if (ganhadores > 1) {
            textbackground(GREEN);
            cout << "\nTem " << ganhadores << " ganhadores!";
            cin.ignore();
            cin.ignore();
        }
        
    } while (ganhadores == 0 && quantidade < 75);
}

// ------------------------------------------------------------------------
// MENU - desenha Jogar/Sobre/Sair e devolve a opcao
// ------------------------------------------------------------------------

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

// ------------------------------------------------------------------------
// MAIN - a largada e o laco do menu
// ------------------------------------------------------------------------

//Equipe de desenvolvimento: Bernardo Sieno, Henrique Dorow, João Vitor Silva da Cruz, Julio Cesar Manabe Padilha e Nicolas do Vale Mezencio
// Objetivo: implementar um bingo com 5 cartelas 5x5
// Mecânica: São geradas 5 cartelas DIFERENTES com intervalos diferentes em cada linha, a cada enter é sorteado um número e é verificado se ele está em alguma cartelas

// main.cpp - a largada: liga o sorteador, reserva a memória das 5 cartelas e
// roda o laço do menu. Toda a lógica está nos outros arquivos.
// Equipe: ver comum.h



int main() {
	srand(time(NULL));
	int cartela1[TAM][TAM], cartela2[TAM][TAM], cartela3[TAM][TAM], cartela4[TAM][TAM], cartela5[TAM][TAM], numSorteados[(TAM*TAM)*3], opcao, quantidade=0;
	string nome[TAM];
    
	
	do {
	    opcao = chamarMenu();
    	switch(opcao) { // Executa a ação correspondente à opção
    		case 1:
    			jogar(cartela1, cartela2, cartela3, cartela4, cartela5, nome, numSorteados, quantidade);
    			break;
    		case 2:
    			sobre();
    			break;
    		case 3:
    			cout<<"\033c";
    			cout << "\n\tObrigado por jogar! :)"; // saída
    			break;
    		}
	} while(opcao != 3); // Continua exibindo o menu enquanto não escolher 3

	return 0;
}
