# verificarSorteado — diz se um número já está numa lista de sorteados
**Onde:** `src/sorteio.cpp:9`  ·  **Declarada em:** `src/sorteio.h:9`  ·  **Modulo:** sorteio

## O que faz
Percorre um vetor e verifica se um número específico já aparece nele. Serve tanto pra não sortear
o mesmo número duas vezes quanto pra checar, número a número, se uma cartela inteira já saiu.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| vet[] | int[] | vetor onde procurar (sorteados até agora, ou os 25 números da cartela) | array, só leitura |
| num | int | número procurado | valor |
| quantidade | int | até que posição do vetor procurar | valor |

## Devolve
bool — true se `num` está em `vet[0..quantidade-1]`, false se não está.

## Quem chama
- `sorteio` (src/sorteio.cpp:23) — pra rejeitar número repetido
- `cartelaCompleta` (src/vitoria.cpp:13) — pra cada um dos 25 números da cartela, pergunta se já saiu

## Para a defesa
P: "por que ela serve pros dois casos, sorteio e vitória?"
R: a pergunta de fundo é a mesma — "esse número está nesta lista?" — só muda qual vetor é passado.
