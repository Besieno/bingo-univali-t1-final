# apagarSorteado — tenta limpar a mensagem da lista de sorteados
**Onde:** `src/exibicao.cpp:45`  ·  **Declarada em:** `src/exibicao.h:14`  ·  **Modulo:** exibicao

## O que faz
Reposiciona o cursor no mesmo ponto onde `mostrarSorteado` escreve (coluna 0, linha 35) e imprime uma string vazia, antes de seguir para a checagem de vencedor da rodada.

## Parametros
Nenhum — a posição que ela mexe é sempre a mesma, fixa no código.

## Devolve
nada (void).

## Quem chama
`jogo.cpp:78`, depois de pintar o número de verde e antes de conferir se alguém ganhou.

## Para a defesa
P: "por que essa função não recebe nenhum parâmetro, se ela mexe na tela?"
R: porque ela só reposiciona o cursor numa linha fixa (35) — a mesma usada por `mostrarSorteado` — não precisa saber quantidade nem qual número, só "chega lá e escreve vazio".
