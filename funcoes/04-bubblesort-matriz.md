# bubblesort — ordena uma linha da cartela (sobrecarga de matriz)
**Onde:** `src/ordenacao.cpp:10`  ·  **Declarada em:** `src/ordenacao.h:17`  ·  **Modulo:** ordenacao

## O que faz
Ordena em ordem crescente os TAM (5) números de UMA linha de uma cartela (matriz 5x5). É Bubble Sort com flag de parada antecipada (`cond`): se uma passagem inteira não troca nada, a linha já está ordenada e o laço externo para antes da hora.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| mat | int[][TAM] | a cartela inteira | array, por referência (sem cópia) |
| linha | int | índice da linha a ordenar | por valor |

## Devolve
nada (void) — a linha é reordenada dentro do próprio array `mat` (efeito colateral, sem retorno).

## Quem chama
`gerarCartela()` em `src/cartela.cpp:35`, uma vez por linha, logo depois de sortear os 5 números daquela linha.

## Para a defesa
P: por que ordenar linha por linha e não a cartela toda de uma vez? R: cada linha tem sua própria faixa de 15 números (1-15, 16-30, ...), então ordenar cada linha isoladamente já entrega a ordem certa sem misturar faixas de linhas diferentes.
