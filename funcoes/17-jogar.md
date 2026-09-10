# jogar — o maestro: gera as cartelas, pede nomes, sorteia e checa vencedor até o fim
**Onde:** `src/jogo.cpp:21`  ·  **Declarada em:** `src/jogo.h:12`  ·  **Modulo:** jogo

## O que faz
Gera as 5 cartelas garantindo que nenhuma repita as anteriores (`do-while` + `verifyCartela`),
pede os 5 nomes e as desenha. Depois sorteia, pinta de vermelho, espera Enter, pinta de verde
e confere vencedor — repetindo até ganhar ou até os 75 números acabarem.

## Parametros
| nome | tipo | serve pra | entra/sai |
|---|---|---|---|
| `mat1..mat5` | `int[][TAM]` | cartela de cada jogador | vazias → preenchidas (por referência) |
| `nome` | `string[TAM]` | nome de cada jogador | vazio → preenchido por `nomearCartelas` |
| `numSorteados` | `int[]` | histórico do sorteio | preenchido pelas chamadas a `sorteio` |
| `quantidade` | `int` | contador de sorteados | **por valor** — não volta pro chamador |

## Devolve
nada (`void`) — efeito colateral: tela desenhada + arrays acima preenchidos.
## Quem chama
`main()` em `src/main.cpp:29`, `case 1` do menu ("Jogar").

## Para a defesa
P: "Por que o contador não continua entre jogos?" R: `quantidade` é por valor (`jogo.h:12`).

> **Corrigido em 10/09** (item C3): a mensagem agora sai como **"A cartela 1 do jogador X ganhou!"**
> (`src/jogo.cpp:86,92,98,104,110`) — o enunciado pede o nome **e** o número da cartela.
> No mesmo item, os dois `cin.ignore()` seguidos antes do sorteio viraram um `cin.ignore(1000, '
')`
> (`src/jogo.cpp:51`): com `getline` nos nomes não sobra mais `
` para descartar, então dois
> ignores pediriam dois Enters.
