# mostrarSorteado — imprime a lista de sorteados, em ordem crescente, destacando o da vez
**Onde:** `src/exibicao.cpp:17`  ·  **Declarada em:** `src/exibicao.h:13`  ·  **Modulo:** exibicao

## O que faz
Reordena o próprio vetor de sorteados (chama `bubblesort`, não trabalha em cópia) e imprime todos, um a um, quebrando linha a cada 25 números. O número que acabou de sair nesta rodada aparece com fundo vermelho; os demais, fundo padrão.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `num` | `int` | o número sorteado nesta rodada | valor — só usado pra decidir qual entra em vermelho |
| `numSorteados[]` | `int[]` | histórico de todos os sorteados até agora | array por referência — é reordenado de verdade dentro da função |
| `quantidade` | `int` | quantos números tem no vetor até agora | valor — limita o laço e o `bubblesort` |

## Devolve
nada (void) — imprime direto na tela.

## Quem chama
`jogo.cpp:58`, dentro do laço de rodadas, logo após cada `sorteio()`.

## Para a defesa
P: "por que ordenar o vetor aqui dentro, e não guardar já ordenado?"
R: `sorteio` grava na ordem cronológica de saída; ordenar só na hora de exibir cumpre o R22 (lista sempre crescente) sem mexer em quem usa o vetor pra outra coisa (ex.: `cartelaCompleta`), que não depende de ordem.

⚠️ A posição fixa `gotoxy(0, 35)` somada à quebra a cada 25 números empurra a lista pra perto da linha 38 quando há muitos sorteados, parte do motivo de o tabuleiro medir 122×37 e não caber em 80×25 nem 120×30.

> **Corrigido em 10/09** (item C8): a variável `linha` era criada e incrementada sem nunca ser
> lida — gerava o aviso `'linha' set but not used`. Foi removida; a quebra de linha a cada 25
> números continua sendo feita pelo `cout << endl`, como antes.
