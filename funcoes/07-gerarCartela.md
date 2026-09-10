# gerarCartela — preenche uma cartela 5x5 respeitando a faixa de cada linha
**Onde:** `src/cartela.cpp:21`  ·  **Declarada em:** `src/cartela.h:12`  ·  **Modulo:** cartela

## O que faz
Para cada uma das 5 linhas, calcula a faixa de valores da linha (1-15, 16-30, 31-45, 46-60, 61-75) e sorteia 5 números dentro dela, redesenhando quando `verifyNum` acusa repetição. Depois de preencher a linha inteira, chama `bubblesort` pra deixá-la em ordem crescente.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `mat` | `int[][TAM]` | a cartela a ser preenchida | entra vazia e sai preenchida — array 2D passa por referência em C++, por isso não precisa retornar nada |

## Devolve
nada (`void`) — o efeito é a matriz `mat` preenchida quando a função termina.

## Quem chama
`jogar`, em `src/jogo.cpp:25,28,31,34,37` — uma vez pra cada uma das 5 cartelas (`mat1` a `mat5`).

## Para a defesa
**P: por que sortear número a número e checar repetição, em vez de embaralhar uma lista dos 15 valores da faixa?**
R: com só 15 valores possíveis pra 5 posições, a chance de repetir é baixa e o "sorteia e redesenha se repetir" (rejection sampling) fica mais simples de escrever e explicar que montar e embaralhar uma lista auxiliar.
