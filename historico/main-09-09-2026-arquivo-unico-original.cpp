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
*/

#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define TAM 5

// Códigos de cor no mesmo padrão do codefun.h que o professor passou
#define BLACK 0
#define GREEN 2
#define RED 4

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

void gotoxy(int x, int y) // Inicia a função responsável por posicionar o cursor em uma coordenada específica do terminal.
{
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H"; // Envia o comando ANSI para mover o cursor para a posição indicada.
    cout.flush();
}

void nomearCartelas(string nome[])
{ // Inicia a função responsável por armazenar os nomes dos jogadores.
    for (int i = 0; i < TAM; i++)
    {
        cout << "Digite o nome do jogador da cartela " << (i + 1) << ": ";
        getline(cin, nome[i]); // getline lê a linha inteira, então nome composto não ocupa a vaga do jogador seguinte
    }
    cout << "\033c";
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

void mostrarSorteado(int num, int x, int y)
{ // Exibe o número que acabou de ser sorteado.
    gotoxy(x, y);
    cout << "                                         "; // Limpa o que já estava escrito na linha, senão sobra pedaço do texto anterior
    gotoxy(x, y);
    cout << "Numero sorteado: " << num;
}

void pintarNumero(int mat[][TAM], int num, int x, int y, int cor)
{ // Inicia a função responsável por destacar um número específico dentro da cartela.
    for (int i = 0; i < TAM; i++)
    {
        int posicaoX = x; // Reinicia a posição horizontal para o início da linha.

        for (int j = 0; j < TAM; j++)
        {

            if (mat[i][j] == num)
            { // Verifica se o número da cartela corresponde ao número sorteado.
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

void exibir(int mat[][TAM], int contador, string nome[], int x, int y)
{ // Inicia a função responsável por exibir uma cartela e seu jogador.
    gotoxy(x, y);
    cout << "Cartela: " << contador; // Mostra o número de identificação da cartela.

    gotoxy(x, y + 1);
    cout << "Jogador: " << nome[contador - 1]; // Mostra o nome do jogador dono dessa cartela.

    for (int i = 0; i < TAM; i++)
    {
        gotoxy(x, y + 3 + i);

        for (int j = 0; j < TAM; j++)
        {
            cout << mat[i][j] << "\t";
        }
    }
}

void exibirSorteados(int lista[], int quantidade, int atual, int x, int y)
{ // Mostra todos os números já sorteados, em ordem crescente
    int coluna = 0;
    int linha = 0;

    gotoxy(x, y);
    cout << "Numeros sorteados (" << quantidade << "/75):";

    for (int i = 0; i < quantidade; i++)
    {
        gotoxy(x + coluna * 3, y + 1 + linha); // Cada número ocupa 3 colunas da tela

        if (lista[i] < 10)
        { // Alinha os números de um dígito com os de dois
            cout << " ";
        }

        if (lista[i] == atual) // O número que acabou de sair fica em vermelho
            textbackground(RED);
        else // Os que já tinham saído ficam em verde
            textbackground(GREEN);

        cout << lista[i];
        textbackground(BLACK);

        coluna++;
        if (coluna == 25)
        { // Quebra a linha a cada 25 números
            coluna = 0;
            linha++;
        }
    }
}

void bubblesort(int mat[][TAM], int linha)
{ // Inicia o algoritmo Bubble Sort para ordenar uma linha da cartela.
    int i, j, cond, temp;
    cond = 1; // Indica que ainda pode ser necessário realizar trocas

    for (i = TAM - 1; (i >= 1) && (cond == 1); i--)
    { // Percorre a linha do final para o início (TAM-1 representa o último índice da linha)

        cond = 0; // Assume que não haverá mais trocas nesta passagem

        for (j = 0; j < i; j++)
        { // Compara elementos vizinhos da linha
            if (mat[linha][j + 1] < mat[linha][j])
            { // Verifica se os elementos estão fora de ordem
                temp = mat[linha][j];
                mat[linha][j] = mat[linha][j + 1];
                mat[linha][j + 1] = temp;
                cond = 1; // Indica que houve uma troca
            }
        }
    }
}

bool numeroRepetidoNaLinha(int mat[][TAM], int linha, int num, int coluna)
{ // Verifica se um número já existe nas posições anteriores da linha atual
    for (int j = 0; j < coluna; j++)
    { // Percorre somente as colunas que já foram preenchidas
        if (mat[linha][j] == num)
        {                // Verifica se o número gerado já apareceu na linha
            return true; // Número repetido encontrado
        }
    }
    return false; // Número não foi encontrado
}

bool verificarSorteado(int vet[], int num, int quantidade)
{ // Verifica se um número já saiu em algum sorteio anterior
    for (int i = 0; i < quantidade; i++)
    { // Percorre somente as posições já preenchidas do vetor de sorteados
        if (vet[i] == num)
        {                // Verifica se o número gerado já foi cantado
            return true; // Número repetido encontrado
        }
    }
    return false; // Número não foi encontrado
}

void inserirEmOrdem(int lista[], int &quantidade, int valor)
{ // Guarda o número novo já na posição certa, deixando a lista sempre em ordem crescente
    int pos = quantidade;

    while (pos > 0 and lista[pos - 1] > valor)
    { // Empurra pra direita todos os que forem maiores que o valor novo
        lista[pos] = lista[pos - 1];
        pos--;
    }

    lista[pos] = valor;
    quantidade++; // quantidade vem por referência, então o contador também sobe para quem chamou
}

bool cartelaCompleta(int mat[][TAM], int numSorteados[], int quantidade)
{ // Informa que todos os números da cartela foram sorteados.
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            if (!verificarSorteado(numSorteados, mat[i][j], quantidade))
            {
                return false;
            }
        }
    }

    return true;
}

void gerarCartela(int mat[][TAM])
{ // Função responsável por gerar uma cartela de Bingo
    int limiteFinal;
    int limiteInicial;
    int num;

    for (int i = 0; i < TAM; i++)
    {                               // Percorre cada linha da cartela
        limiteInicial = 15 * i + 1; // Define o primeiro número possível da linha - Linha 0: 1 - Linha 1: 16 - Linha 2: 31 - Linha 3: 46 - Linha 4: 61
        limiteFinal = 15 * (1 + i); // Define o último número possível da linha - Linha 0: 15 - Linha 1: 30 - Linha 2: 45 - Linha 3: 60 - Linha 4: 75
        for (int j = 0; j < TAM; j++)
        {
            do
            {
                mat[i][j] = limiteInicial + rand() % (limiteFinal - limiteInicial + 1); // Gera um número aleatório dentro do intervalo definido para a linha atual
                num = mat[i][j];
            } while (numeroRepetidoNaLinha(mat, i, num, j)); // Gera outro número caso o número já tenha aparecido
        }
        bubblesort(mat, i); // Ordena a linha depois que seus 5 números foram gerados
    }
}

bool cartelasIguais(int a[][TAM], int b[][TAM])
{ // Retorna verdadeiro quando todas as posições das cartelas são iguais.
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            if (a[i][j] != b[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

int sorteio(int numSorteados[], int &quantidade)
{
    int num;

    do
    {
        num = rand() % 75 + 1; // Gera um número aleatório entre 1 e 75.
    } while (verificarSorteado(numSorteados, num, quantidade)); // Verifica se o número gerado já foi sorteado.

    inserirEmOrdem(numSorteados, quantidade, num); // Guarda o número na lista ordenada e soma 1 na quantidade

    return num;
}

void jogar(int mat1[][TAM], int mat2[][TAM], int mat3[][TAM], int mat4[][TAM], int mat5[][TAM], string nome[])
{                         // Função responsável por iniciar o jogo
    int numSorteados[75]; // Todos os números já cantados, guardados em ordem crescente
    int quantidade = 0;   // Quantos números já saíram nesta partida
    int ganhadores = 0;

    // Posição de cada parte do tabuleiro na tela, reunidas aqui pra ajustar o layout em um lugar só
    int x1 = 2, y1 = 1;
    int x2 = 42, y2 = 1;
    int x3 = 82, y3 = 1;
    int x4 = 2, y4 = 10;
    int x5 = 42, y5 = 10;
    int xSorteados = 2, ySorteados = 18;
    int xAviso = 2, yAviso = 23;

    cout << "\033c"; // Limpa a tela

    gerarCartela(mat1); // Gera cartela 1

    do
    {
        gerarCartela(mat2); // Gera cartela 2
    } while (cartelasIguais(mat1, mat2)); // verificar se as cartelas 1 e 2 são iguais
    do
    {
        gerarCartela(mat3); // Gera cartela 3
    } while (cartelasIguais(mat1, mat3) or cartelasIguais(mat2, mat3));
    do
    {
        gerarCartela(mat4); // Gera cartela 4
    } while (cartelasIguais(mat1, mat4) or cartelasIguais(mat2, mat4) or cartelasIguais(mat3, mat4));
    do
    {
        gerarCartela(mat5); // Gera cartela 5
    } while (cartelasIguais(mat1, mat5) or cartelasIguais(mat2, mat5) or cartelasIguais(mat3, mat5) or cartelasIguais(mat4, mat5));

    nomearCartelas(nome);

    exibir(mat1, 1, nome, x1, y1);
    exibir(mat2, 2, nome, x2, y2);
    exibir(mat3, 3, nome, x3, y3);
    exibir(mat4, 4, nome, x4, y4);
    exibir(mat5, 5, nome, x5, y5);

    gotoxy(xAviso, yAviso);
    cout << "Pressione Enter para comecar o sorteio...";
    cin.ignore(1000, '\n'); // Aguarda o Enter antes de cantar o primeiro número

    do
    {
        int num = sorteio(numSorteados, quantidade);

        mostrarSorteado(num, xAviso, yAviso);
        exibirSorteados(numSorteados, quantidade, num, xSorteados, ySorteados); // Lista em ordem crescente, com o atual destacado

        // Vermelho durante o sorteio
        pintarNumero(mat1, num, x1, y1, RED);
        pintarNumero(mat2, num, x2, y2, RED);
        pintarNumero(mat3, num, x3, y3, RED);
        pintarNumero(mat4, num, x4, y4, RED);
        pintarNumero(mat5, num, x5, y5, RED);

        // Espera apenas um Enter
        cin.ignore(1000, '\n');

        // Depois do Enter fica verde
        pintarNumero(mat1, num, x1, y1, GREEN);
        pintarNumero(mat2, num, x2, y2, GREEN);
        pintarNumero(mat3, num, x3, y3, GREEN);
        pintarNumero(mat4, num, x4, y4, GREEN);
        pintarNumero(mat5, num, x5, y5, GREEN);

        exibirSorteados(numSorteados, quantidade, 0, xSorteados, ySorteados); // 0 = nenhum número em destaque

        ganhadores = 0;

        if (cartelaCompleta(mat1, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[0] << " ganhou com a cartela 1!";
            ganhadores++;
        }

        if (cartelaCompleta(mat2, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[1] << " ganhou com a cartela 2!";
            ganhadores++;
        }

        if (cartelaCompleta(mat3, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[2] << " ganhou com a cartela 3!";
            ganhadores++;
        }

        if (cartelaCompleta(mat4, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[3] << " ganhou com a cartela 4!";
            ganhadores++;
        }

        if (cartelaCompleta(mat5, numSorteados, quantidade))
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << nome[4] << " ganhou com a cartela 5!";
            ganhadores++;
        }

        if (ganhadores > 0)
        {
            gotoxy(xAviso, yAviso + 1 + ganhadores);
            cout << "Pressione Enter para voltar ao menu...";
            cin.ignore(1000, '\n');
        }

    } while (ganhadores == 0 && quantidade < 75);
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

int main()
{
    srand(time(NULL));
    system("mode con: cols=120 lines=30 > nul 2>&1"); // Abre a janela no tamanho que o tabuleiro precisa (o > nul evita mensagem de erro se o terminal não aceitar)

    int cartela1[TAM][TAM], cartela2[TAM][TAM], cartela3[TAM][TAM], cartela4[TAM][TAM], cartela5[TAM][TAM];
    string nome[TAM];

    chamarMenu(cartela1, cartela2, cartela3, cartela4, cartela5, nome);

    return 0;
}
