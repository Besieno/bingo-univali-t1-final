# O que cada função faz

Uma ficha por função do jogo — 21 no total. Cada uma é curta de propósito: o que faz,
que parâmetros recebe, o que devolve, quem a chama, e **uma pergunta de defesa com a resposta**.

Serve para duas coisas: achar rápido onde mexer, e treinar a defesa função por função.
Para o andar de cima (em que ordem as coisas acontecem, quem depende de quem), veja
[`docs/00-mapa-do-codigo.md`](../docs/00-mapa-do-codigo.md).

> **✅ = corrigido em 10/09** (os quatro itens vermelhos do `91`). **⚠️ = problema conhecido que
> ficou** — o que fazer está em [`docs/91-o-que-pode-ser-contestado.md`](../docs/91-o-que-pode-ser-contestado.md).


## Tela — cursor e cor

| Função | Ficha | Onde |
|---|---|---|
| **gotoxy** ⚠️ — move o cursor do terminal pra coluna x, linha y | [01-gotoxy.md](01-gotoxy.md) | `src/tela.cpp:13` |
| **ansiBG** — converte um código de cor (0-15) no código ANSI de fundo | [02-ansiBG.md](02-ansiBG.md) | `src/tela.cpp:19` |
| **textbackground** ⚠️ — liga (ou desliga) a cor de fundo do texto no terminal | [03-textbackground.md](03-textbackground.md) | `src/tela.cpp:26` |

## Ordenação — Bubble Sort (as duas sobrecargas)

| Função | Ficha | Onde |
|---|---|---|
| **bubblesort** — ordena uma linha da cartela (sobrecarga de matriz) | [04-bubblesort-matriz.md](04-bubblesort-matriz.md) | `src/ordenacao.cpp:10` |
| **bubblesort** — ordena o vetor de números sorteados (sobrecarga de vetor) | [05-bubblesort-vetor.md](05-bubblesort-vetor.md) | `src/ordenacao.cpp:29` |

## Cartela — gerar e validar uma cartela

| Função | Ficha | Onde |
|---|---|---|
| **verifyNum** — checa se um número já saiu antes na mesma linha da cartela | [06-verifyNum.md](06-verifyNum.md) | `src/cartela.cpp:12` |
| **gerarCartela** — preenche uma cartela 5x5 respeitando a faixa de cada linha | [07-gerarCartela.md](07-gerarCartela.md) | `src/cartela.cpp:21` |
| **verifyCartela** — diz se duas cartelas são idênticas, posição por posição | [08-verifyCartela.md](08-verifyCartela.md) | `src/cartela.cpp:41` |

## Sorteio — a urna

| Função | Ficha | Onde |
|---|---|---|
| **verificarSorteado** — diz se um número já está numa lista de sorteados | [09-verificarSorteado.md](09-verificarSorteado.md) | `src/sorteio.cpp:9` |
| **sorteio** — sorteia o próximo número (1 a 75) sem repetir | [10-sorteio.md](10-sorteio.md) | `src/sorteio.cpp:18` |

## Vitória — a cartela está cheia?

| Função | Ficha | Onde |
|---|---|---|
| **cartelaCompleta** — checa se todos os 25 números da cartela já foram sorteados | [11-cartelaCompleta.md](11-cartelaCompleta.md) | `src/vitoria.cpp:10` |

## Exibição — o que aparece na tela

| Função | Ficha | Onde |
|---|---|---|
| **mostrarSorteado** ⚠️ — imprime a lista de sorteados, em ordem crescente, destacando o da vez | [12-mostrarSorteado.md](12-mostrarSorteado.md) | `src/exibicao.cpp:17` |
| **apagarSorteado** — tenta limpar a mensagem da lista de sorteados | [13-apagarSorteado.md](13-apagarSorteado.md) | `src/exibicao.cpp:45` |
| **pintarNumero** — encontra um número dentro da cartela e pinta o fundo dele | [14-pintarNumero.md](14-pintarNumero.md) | `src/exibicao.cpp:50` |
| **exibir** ⚠️ — desenha uma cartela completa (cabeçalho + 25 números) na posição recebida | [15-exibir.md](15-exibir.md) | `src/exibicao.cpp:77` |

## Jogadores — o dono de cada cartela

| Função | Ficha | Onde |
|---|---|---|
| **nomearCartelas** ✅ — pergunta o nome dos 5 jogadores, um por vez | [16-nomearCartelas.md](16-nomearCartelas.md) | `src/jogadores.cpp:13` |

## Jogo — a partida

| Função | Ficha | Onde |
|---|---|---|
| **jogar** ✅ — o maestro: gera as cartelas, pede nomes, sorteia e checa vencedor até o fim | [17-jogar.md](17-jogar.md) | `src/jogo.cpp:21` |

## Menu — Jogar / Sobre / Sair

| Função | Ficha | Onde |
|---|---|---|
| **lerOpcao** ✅ — lê e valida a opção do menu (só aceita 1, 2 ou 3) | [18-lerOpcao.md](18-lerOpcao.md) | `src/menu.cpp:11` |
| **sobre** ✅ — mostra a tela de créditos (equipe, professor, mês/ano) | [19-sobre.md](19-sobre.md) | `src/menu.cpp:29` |
| **chamarMenu** ⚠️ — desenha o menu principal e devolve a opção escolhida | [20-chamarMenu.md](20-chamarMenu.md) | `src/menu.cpp:31` |

## Main — a largada

| Função | Ficha | Onde |
|---|---|---|
| **main** — ponto de entrada: prepara os dados e roda o laço do menu | [21-main.md](21-main.md) | `src/main.cpp:19` |

---

**21 funções** · 4 corrigidas em 10/09 · 5 ainda com alerta.
