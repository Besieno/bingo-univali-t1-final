# ansiBG — converte um código de cor (0-15) no código ANSI de fundo
**Onde:** `src/tela.cpp:19`  ·  **Declarada em:** `src/tela.h:14`  ·  **Modulo:** tela

## O que faz
Recebe um número de cor e devolve o código que o terminal ANSI entende como "fundo dessa
cor". Cores 0-7 (básicas) viram 40-47; cores 8-15 (claras) viram 100-107. É auxiliar interna
de textbackground — nenhum outro arquivo do jogo chama ansiBG direto.

## Parametros
| nome | tipo | pra que serve | entra/sai |
|---|---|---|---|
| color | int | código a converter (BLACK=0, RED=1, GREEN=2, ...) | entra, por valor |

## Devolve
int — o código ANSI de fundo, pronto pra ir dentro de `"\033[" << codigo << "m"`

## Quem chama
`tela.cpp:30`, dentro da própria `textbackground` — função auxiliar interna do módulo

## Para a defesa
P: "Isso veio do material do professor?" R: gotoxy e textbackground são idênticos ao
`codefun_GDB.h` que ele entregou; ansiBG não — o dele usa uma tabela de 16 posições, este usa
uma fórmula, porque os códigos 0-7 do jogo já seguem a ordem real do ANSI (compare
`referencia/codefun_GDB.h:65-68` com `src/tela.h:9-11`).
