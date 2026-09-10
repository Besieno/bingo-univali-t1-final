# lerOpcao — lê e valida a opção do menu (só aceita 1, 2 ou 3)
**Onde:** `src/menu.cpp:11`  ·  **Declarada em:** `src/menu.h:11`  ·  **Módulo:** menu

## O que faz
Lê um número digitado pelo usuário e repete a leitura enquanto o valor estiver
fora de 1 a 3, avisando "Valor inválido" a cada tentativa errada. Devolve a
primeira opção válida digitada.

## Parâmetros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| opcao | int | valor de entrada da função | por valor — mas é sobrescrito por `cin >> opcao` antes de qualquer uso, o que chega nunca é lido |

## Devolve
`int` — a opção validada (sempre 1, 2 ou 3).

## Quem chama
`chamarMenu()`, em `src/menu.cpp:40`.

## Para a defesa
P: "Por que a função recebe um parâmetro `opcao` se ele nunca é usado?"
R: É resíduo de um design anterior; hoje `cin >> opcao` sobrescreve o parâmetro antes de qualquer leitura, então o valor recebido não influencia o resultado.

> **Corrigido em 10/09** (item C1): ganhou `cin.clear()` + `cin.ignore(1000, '
')` quando a leitura
> falha, e `return 3` quando a entrada termina (`src/menu.cpp:14-21`). Antes, digitar uma letra
> travava o programa num laço infinito — 86 MB de "Valor inválido" em 2 segundos. **Medido depois
> da correção:** `a`, `b`, `9`, `3` → três avisos e sai limpo, 321 bytes.
