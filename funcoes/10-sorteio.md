# sorteio — sorteia o próximo número (1 a 75) sem repetir
**Onde:** `src/sorteio.cpp:18`  ·  **Declarada em:** `src/sorteio.h:10`  ·  **Modulo:** sorteio

## O que faz
Gera número aleatório entre 1 e 75, repetindo o sorteio enquanto o número já tiver saído antes.
Grava o número aceito na próxima posição livre do histórico de sorteados e devolve esse número.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| numSorteados[] | int[] | histórico de números já sorteados na partida | array, entra e sai (grava a posição `quantidade`) |
| quantidade | int | quantos números já saíram; também é o índice onde o novo entra | valor |

## Devolve
int — o número sorteado (1 a 75), já garantido inédito na partida.

## Quem chama
`jogar` (src/jogo.cpp:55) — uma vez a cada Enter, dentro do laço de rodadas.

## Para a defesa
P: "e se os 75 números saíssem todos sem ninguém dar bingo?"
R: não acontece — as 5 cartelas juntas só têm números de 1 a 75, então ao sortear o 75º número
alguma cartela já está completa; o vetor `numSorteados` tem exatamente 75 posições (main.cpp:21) e nunca estoura.
