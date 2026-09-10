# pintarNumero — encontra um número dentro da cartela e pinta o fundo dele
**Onde:** `src/exibicao.cpp:50`  ·  **Declarada em:** `src/exibicao.h:15`  ·  **Modulo:** exibicao

## O que faz
Varre a matriz da cartela célula por célula, recalculando a coluna de tela de cada uma (mesma conta de tabulação usada por `exibir`). Ao achar o número procurado, posiciona o cursor ali, pinta o fundo com a cor recebida e para — se o número não está na cartela, a função não faz nada.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `mat[][TAM]` | matriz `int` | a cartela onde procurar | entra, por referência |
| `num` | `int` | número a destacar | valor |
| `x`, `y` | `int` | canto onde essa cartela foi desenhada por `exibir` | valor — precisam ser os mesmos usados na chamada de `exibir` |
| `cor` | `int` | cor de fundo (`RED` ou `GREEN`, de `tela.h`) | valor |

## Devolve
nada (void) — muda a cor de fundo daquela célula na tela.

## Quem chama
`jogo.cpp:62-66` (vermelho, logo após o sorteio) e `jogo.cpp:72-76` (verde, depois do Enter) — uma chamada por cartela, mat1 a mat5, nas duas cores.

## Para a defesa
P: "por que recalcular a posição em vez de guardar onde cada número caiu na tela?"
R: porque `exibir` alinha com `\t`, que salta pra parada de tabulação em vez de coluna fixa; `pintarNumero` repete o mesmo cálculo (linhas 67-72) pra achar onde aquele número ficou, sem guardar coordenada nenhuma.
