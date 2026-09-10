# nomearCartelas — pergunta o nome dos 5 jogadores, um por vez
**Onde:** `src/jogadores.cpp:13`  ·  **Declarada em:** `src/jogadores.h:11`  ·  **Modulo:** jogadores

## O que faz
Roda um laço de `TAM` (5) voltas. Em cada volta imprime "Digite o nome do Nº jogador"
e lê a resposta com `cin >>` direto na posição `nome[i]`. No fim, limpa a tela
(`"\033c"`). É o único lugar do programa que preenche o vetor de nomes.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `nome` | `string[]` (array, tamanho `TAM`=5) | vetor onde cada nome digitado é guardado | sai preenchido (passagem por array = por referência) |

## Devolve
nada (void) — o resultado é o efeito colateral: `nome[0..4]` preenchidos ao voltar.

## Quem chama
`jogo.cpp:40`, dentro de `jogar(...)`, logo depois de gerar as 5 cartelas e antes de exibi-las na tela.

## Para a defesa
P: "Por que só um `for` de `TAM` e não `TAM*algumaCoisa`?" R: porque o jogo tem regra fixa
de 5 cartelas para 5 jogadores (mesma constante `TAM` do comum.h, reaproveitada para as duas coisas).

> **Corrigido em 10/09** (item C2): lia com `cin >> nome[i]`, que para no primeiro espaço — "Joao Vitor"
> virava dois jogadores e o quinto nunca era perguntado. Agora usa `getline`, e um
> `cin.ignore(1000, '
')` antes do laço descarta o Enter que sobrou do menu (`src/jogadores.cpp:14-18`).
> **Medido depois:** os cinco nomes compostos da equipe ficaram inteiros, um por cartela.
