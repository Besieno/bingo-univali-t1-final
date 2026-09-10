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

⚠️ Vitória (linhas 87,93,99,105,111) só diz "ganhou!" — sem número da cartela (`enunciado.txt:67` pede os dois).
