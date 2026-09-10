# gotoxy — move o cursor do terminal pra coluna x, linha y
**Onde:** `src/tela.cpp:13`  ·  **Declarada em:** `src/tela.h:13`  ·  **Modulo:** tela

## O que faz
Manda um comando ANSI que reposiciona o cursor sem apagar o que já foi impresso. É a base de
toda a exibição: cartelas, lista de sorteados e avisos usam gotoxy antes de imprimir, pra
desenhar em coordenadas fixas em vez de rolar a tela.

## Parametros
| nome | tipo | pra que serve | entra/sai |
|---|---|---|---|
| x | int | coluna de destino (0 = primeira) | entra, por valor |
| y | int | linha de destino (0 = primeira) | entra, por valor |

## Devolve
nada (void) — efeito colateral: move o cursor e dá `flush` no `cout`

## Quem chama
`exibicao.cpp:19,46,57,78,81,85` (mensagem do sorteado e cartela) · `jogo.cpp:42` (aviso "Enter para continuar...")

## Para a defesa
P: "Por que +1 em x e y?" R: o terminal ANSI numera linha/coluna a partir de 1; o resto do código conta cartela a partir de 0 — a soma converte.
⚠️ As coordenadas usadas (até x=90, y=35) são o motivo de o tabuleiro precisar de 122 colunas × 37 linhas — não cabe em 80×25 nem 120×30.
