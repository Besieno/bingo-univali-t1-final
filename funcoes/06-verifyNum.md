# verifyNum — checa se um número já saiu antes na mesma linha da cartela
**Onde:** `src/cartela.cpp:12`  ·  **Declarada em:** `src/cartela.h:11`  ·  **Modulo:** cartela

## O que faz
Percorre as colunas já preenchidas de uma linha da cartela (de 0 até `coluna-1`) e verifica se `num` já apareceu ali. É o freio que impede repetição dentro da mesma linha durante a geração.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `mat` | `int[][TAM]` | a cartela inteira (matriz 5x5) | entra, por referência (array 2D sempre passa por referência em C++) |
| `linha` | `int` | qual linha checar | entra, por valor |
| `num` | `int` | o número recém-sorteado a conferir | entra, por valor |
| `coluna` | `int` | até onde a linha já foi preenchida (limite do laço) | entra, por valor |

## Devolve
`bool` — `true` se `num` já existe nas colunas `0..coluna-1` da `linha`; `false` se não encontrou repetição.

## Quem chama
`gerarCartela`, em `src/cartela.cpp:33` — dentro do `do...while` que gera cada número da cartela.

## Para a defesa
**P: por que a função só olha até `coluna` e não a linha toda?**
R: porque as posições depois de `coluna` ainda não foram preenchidas nesta rodada de geração — comparar com elas seria ler lixo de memória, não um número já sorteado.
