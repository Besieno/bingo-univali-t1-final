# verifyCartela — diz se duas cartelas são idênticas, posição por posição
**Onde:** `src/cartela.cpp:41`  ·  **Declarada em:** `src/cartela.h:13`  ·  **Modulo:** cartela

## O que faz
Compara duas cartelas 5x5 número a número. Se todas as 25 posições forem iguais entre `a` e `b`, considera as cartelas iguais. Usada pra impedir que o jogo gere duas cartelas idênticas na mesma partida.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `a` | `int[][TAM]` | primeira cartela a comparar | entra, por referência |
| `b` | `int[][TAM]` | segunda cartela a comparar | entra, por referência |

## Devolve
`bool` — `true` quando as duas cartelas são idênticas (posição a posição); `false` na primeira diferença encontrada.

## Quem chama
`jogar`, em `src/jogo.cpp:29,32,35,38` — depois de gerar cada cartela nova (2ª a 5ª), compara com todas as anteriores dentro de um `do...while` que regenera enquanto houver igualdade.

## Para a defesa
**P: por que comparar as 25 posições em vez de só o total dos números (soma)?**
R: porque duas cartelas diferentes podem ter a mesma soma por coincidência; comparar posição a posição é a única forma de garantir que são de fato a mesma cartela, não só parecidas.
