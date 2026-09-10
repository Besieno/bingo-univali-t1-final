# exibir — desenha uma cartela completa (cabeçalho + 25 números) na posição recebida
**Onde:** `src/exibicao.cpp:77`  ·  **Declarada em:** `src/exibicao.h:16`  ·  **Modulo:** exibicao

## O que faz
Escreve o número da cartela e o nome do jogador dono dela, depois imprime as 5 linhas x 5 colunas da matriz, separando os números por tab. Não decide onde a cartela entra na tela — só desenha no `x,y` que recebe.

## Parametros
| nome | tipo | para que serve | entra/sai |
|---|---|---|---|
| `mat[][TAM]` | matriz `int` | a cartela a desenhar | entra, por referência |
| `contador` | `int` | número/ID da cartela (1 a 5) | valor — também indexa `nome[contador-1]` |
| `nome[]` | `string[]` | nome de todos os jogadores | entra, por referência — só lê a posição desta cartela |
| `x`, `y` | `int` | canto superior esquerdo onde desenhar | valor |

## Devolve
nada (void) — desenha direto na tela.

## Quem chama
`jogo.cpp:45-49` — uma vez pra cada uma das 5 cartelas, antes do sorteio começar.

## Para a defesa
P: "por que separar os números com `\t` em vez de espaço fixo?"
R: `\t` pula pra próxima parada de tabulação (múltiplo de 8) sozinho, então a coluna fica alinhada sem contar quantos dígitos tem cada número (1 ou 2).

⚠️ Essa parada de tabulação é absoluta na tela, não relativa a `x`; com as cartelas 2 e 5 desenhadas em `x=90` (`jogo.cpp:46,49`) a última coluna cai bem mais longe — é parte do motivo de o tabuleiro medir 122 colunas e não caber em 80×25 nem 120×30.
