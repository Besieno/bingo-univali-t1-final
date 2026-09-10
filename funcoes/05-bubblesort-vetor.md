# bubblesort — ordena o vetor de números sorteados (sobrecarga de vetor)
**Onde:** `src/ordenacao.cpp:29`  ·  **Declarada em:** `src/ordenacao.h:18`  ·  **Modulo:** ordenacao

## O que faz
Ordena em ordem crescente as `n` primeiras posições de um vetor genérico. Mesma lógica de Bubble Sort da outra sobrecarga (troca vizinhos, flag `cond` pra parar cedo), mas sem depender de TAM — serve pra qualquer vetor, não só pra cartela.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| vet | int[] | vetor a ordenar | array, por referência (sem cópia) |
| n | int | quantas posições ordenar (nem sempre é o tamanho todo do array) | por valor |

## Devolve
nada (void) — o vetor é reordenado dentro dele mesmo (efeito colateral, sem retorno).

## Quem chama
`mostrarSorteado()` em `src/exibicao.cpp:20`, toda vez que um número novo é sorteado — reordena `numSorteados` antes de imprimir a lista na tela.

## Para a defesa
P: por que ordenar tudo de novo a cada sorteio, em vez de inserir o número novo já na posição certa? R: o enunciado (R22) só exige a lista final em ordem crescente na tela; o vetor tem no máximo 75 elementos, então reordenar tudo a cada rodada é simples e o custo extra não importa aqui.
