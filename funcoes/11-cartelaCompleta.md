# cartelaCompleta — checa se todos os 25 números da cartela já foram sorteados
**Onde:** `src/vitoria.cpp:10`  ·  **Declarada em:** `src/vitoria.h:11`  ·  **Modulo:** vitoria

## O que faz
Varre as 5 linhas e 5 colunas da cartela (25 números). Para cada número, pergunta a `verificarSorteado` se ele já saiu na urna. Basta um número que ainda não saiu para retornar falso na hora, sem terminar de varrer o resto.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `mat` | `int[][TAM]` | a cartela do jogador (matriz 5x5) | entra, por referência (array) |
| `numSorteados` | `int[]` | lista dos números já sorteados | entra, por referência (array) |
| `quantidade` | `int` | quantos números já foram sorteados até agora | entra, por valor |

## Devolve
`bool` — `true` se os 25 números da cartela já saíram todos; `false` assim que encontra o primeiro que falta.

## Quem chama
`src/jogo.cpp:85, 91, 97, 103, 109` — uma vez para cada uma das 5 cartelas (mat1 a mat5), a cada rodada de sorteio.

## Para a defesa
P: "por que parar no primeiro número que falta, em vez de contar quantos faltam?" R: não precisa contar — a pergunta é só sim/não (bingo ou não), então o `return false` imediato economiza checagens sem mudar o resultado.
