# main — ponto de entrada: prepara os dados e roda o laço do menu
**Onde:** `src/main.cpp:19`  ·  **Declarada em:** só no .cpp  ·  **Módulo:** main

## O que faz
Semeia o gerador de aleatórios com `srand(time(NULL))`, declara as 5 matrizes de
cartela, o vetor de nomes, o vetor de números sorteados e o contador `quantidade`.
Depois roda um `do-while`: mostra o menu, executa a opção escolhida (Jogar, Sobre
ou Sair) e só encerra quando a opção for Sair.

## Parâmetros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| — | — | `main()` não recebe parâmetro (sem argc/argv) | — |

## Devolve
`int` — código de saída do processo; sempre `0`, devolvido só depois do laço do
menu terminar (opção 3).

## Quem chama
Ninguém no código: é o ponto de entrada, chamado pelo sistema operacional/runtime
do C++ ao iniciar o `.exe`.

## Para a defesa
"Por que `numSorteados` tem tamanho `(TAM*TAM)*3`?" — `TAM*TAM` = 25 (números por
cartela) × 3 = 75, exatamente a faixa de sorteio de 1 a 75 do enunciado.
