# textbackground — liga (ou desliga) a cor de fundo do texto no terminal
**Onde:** `src/tela.cpp:26`  ·  **Declarada em:** `src/tela.h:15`  ·  **Modulo:** tela

## O que faz
Muda a cor de fundo de tudo que for impresso depois dela, até a próxima chamada. Com BLACK
volta o fundo ao padrão; com qualquer outra cor, chama ansiBG pra montar o comando certo. É o
que pinta o número da vez na lista de sorteados e o número certo dentro da cartela.

## Parametros
| nome | tipo | pra que serve | entra/sai |
|---|---|---|---|
| newcolor | int | cor de fundo a ligar (BLACK desliga) | entra, por valor |

## Devolve
nada (void) — efeito colateral: muda a cor de fundo até a próxima chamada

## Quem chama
`exibicao.cpp:30,34` (número da vez, na lista) · `exibicao.cpp:59,61` (em `pintarNumero`,
chamada por `jogo.cpp:62-76` com RED/GREEN) · `jogo.cpp:86-121` (mensagem de vitória em verde)

## Para a defesa
P: "Por que desligar com BLACK depois?" R: a cor de fundo ANSI fica ligada até alguém mandar
desligar — sem o `textbackground(BLACK)` logo depois, a cor vazaria pro resto da linha.
⚠️ A cor funciona certo, mas a mensagem que ela destaca em `jogo.cpp:87,93,99,105,111` diz só
"A cartela do jogador X ganhou!" — sem o número da cartela vencedora.
