# Vitória — a pergunta que decide se o sorteio pode parar

## O que esta parte resolve

O enunciado define vitória assim: "a primeira pessoa que completar uma cartela é a
vencedora" (`enunciado.txt:16`). Em código isso vira uma pergunta que precisa ser feita
depois de cada número sorteado: **esta cartela específica já teve todos os seus 25
números cantados?**

`vitoria.h`/`vitoria.cpp` resolvem só essa pergunta, e nada além dela. A função não
imprime nada, não conhece nome de jogador, não sabe que existe tela — ela recebe uma
cartela e a lista do que já saiu, e devolve `true` ou `false`. Quem faz algo com essa
resposta (anunciar o vencedor, parar o sorteio) é `jogo.cpp`.

## Arquivos e funções

| função | arquivo:linha | o que faz |
|---|---|---|
| `cartelaCompleta` | declaração em `src/vitoria.h:11`; implementação em `src/vitoria.cpp:10-24` | recebe uma cartela e a lista de números sorteados; devolve `true` só se as 25 posições da cartela já saíram |

`cartelaCompleta` não sorteia nem guarda nada sozinha: pra cada posição ela pergunta pro
sorteio se aquele número já saiu, chamando `verificarSorteado` (`src/sorteio.cpp:11-21`).
É a única função de que `vitoria.cpp` depende — por isso o `#include "sorteio.h"` em
`vitoria.cpp:8`.

## Como funciona, passo a passo

1. A função recebe 3 parâmetros: a cartela (`mat[][TAM]`), o vetor com os números já
   sorteados (`numSorteados[]`) e quantos números já saíram (`quantidade`) —
   `vitoria.cpp:10`. `TAM` é 5 (`comum.h:23`), então a cartela tem 5×5 = 25 posições,
   o mesmo 25 que o enunciado usa pra descrever a cartela (`enunciado.txt:39`).

2. Dois `for` aninhados percorrem as 25 posições, linha por linha (`i`) e, dentro de
   cada linha, coluna por coluna (`j`) — `vitoria.cpp:12-14`.

3. Para cada posição, a função pergunta pro sorteio: "`mat[i][j]` já foi cantado?",
   chamando `verificarSorteado(numSorteados, mat[i][j], quantidade)` (`vitoria.cpp:16`).
   Essa chamada faz uma busca linear nas primeiras `quantidade` posições de
   `numSorteados` (`sorteio.cpp:11-21`) — é o mesmo vetor que `sorteio()` preenche a
   cada rodada (`sorteio.cpp:37-48`).

4. Se a resposta for "não" (`!verificarSorteado(...)`), a função já sabe que a cartela
   não está completa e devolve `false` **na hora**, sem terminar de olhar o resto da
   cartela (`vitoria.cpp:16-19`). Não interessa quantos números faltam — basta faltar
   um pra a resposta já estar decidida.

5. Se os dois `for` terminarem sem nenhum `false` ter sido disparado, é porque as 25
   posições bateram positivo, e a função devolve `true` (`vitoria.cpp:23`).

6. Quem chama: `jogo.cpp` roda essa checagem pras 5 cartelas, uma a uma, sempre depois
   de sortear o número (`jogo.cpp:72`), pintá-lo de vermelho, esperar o Enter e
   repintá-lo de verde (`jogo.cpp:74-92`). Ou seja, `cartelaCompleta` só examina o
   estado já "fechado" da rodada — nunca é chamada no meio dela.

7. Cada chamada (`jogo.cpp:98`, `105`, `112`, `119`, `126`) alimenta um `if`: se
   `true`, imprime o nome do jogador e o número da cartela, e soma 1 em `ganhadores`
   (por exemplo `jogo.cpp:100-102` pra cartela 1). As 5 cartelas são checadas mesmo
   depois de uma já ter dado positivo na mesma rodada — então dois jogadores podem
   ser anunciados juntos se o número que saiu completar duas cartelas ao mesmo tempo.

8. O laço principal do sorteio só para quando `ganhadores` fica maior que 0 (ou os 75
   números acabam) — `while (ganhadores == 0 && quantidade < 75)` em `jogo.cpp:140`.
   Esse `while` é o verdadeiro "fim de jogo"; `cartelaCompleta` só fornece a resposta
   que ele testa a cada rodada.

## Por que foi feito assim

- **Por que perguntar pro sorteio (`verificarSorteado`) em vez de marcar direto na
  cartela quando um número sai?** Porque já existe uma única fonte de verdade pra "o
  que já foi cantado": o vetor `numSorteados`, mantido por `sorteio.cpp`. Se
  `vitoria.cpp` guardasse seu próprio controle (uma matriz paralela de booleanos, por
  exemplo), esse controle teria que ser atualizado toda vez que um número saísse, em
  outro arquivo — mais um lugar pra esquecer de atualizar e as duas partes do
  programa discordarem entre si sobre o que já saiu. Reaproveitar `numSorteados`
  custa uma chamada de função e garante que sorteio e vitória nunca dessincronizam.

- **Por que percorrer as 25 posições de novo a cada rodada, em vez de guardar um
  contador de quantos números da cartela já bateram?** Um contador incremental seria
  mais rápido, mas exigiria decidir, a cada número sorteado, em quais das 5 cartelas
  ele aparece e somar ali — duplicando lógica que `sorteio.cpp` e `vitoria.cpp` já
  resolvem separadamente. Com 25 células e no máximo 75 números sorteados, o pior
  caso é 25×75 = 1875 comparações por cartela por rodada: irrelevante pra um jogo de
  console. A diferença de desempenho não aparece na prática, então a versão mais
  simples de entender venceu.

- **Por que `return false` assim que acha o primeiro número faltando, em vez de
  contar quantos faltam e decidir só no fim?** A pergunta que a função responde é
  sim/não. Assim que se sabe que falta pelo menos um número, a resposta já está
  decidida; continuar olhando o resto da cartela não muda o resultado, só gasta
  tempo à toa. É resolver o problema com o mínimo de trabalho necessário.

- **Por que `quantidade` é passado por valor aqui, e por referência (`&quantidade`)
  em `sorteio()` e `inserirEmOrdem()` (`sorteio.h:10-11`)?** A regra é: usa-se `&`
  quando a função precisa alterar a variável de quem chamou e essa mudança precisa
  ser vista de volta fora dela. `cartelaCompleta` só lê a lista pra comparar — nunca
  soma nem tira nada de `quantidade`. Em `sorteio()` e `inserirEmOrdem()`, o contador
  de números sorteados precisa subir e continuar valendo pra `jogo.cpp`, daí o `&`.
  Usar `&` sem necessidade em `cartelaCompleta` não daria erro de compilação, mas
  seria usar o recurso sem entender pra que ele serve — e é esse contraste que prova,
  na defesa, que a referência foi escolha e não decoreba.

- **Por que `cartelaCompleta` não sabe nada sobre nome de jogador nem sobre tela?**
  Pra atender R30 (sub-rotinas com passagem de parâmetro) sem misturar
  responsabilidade: `vitoria.cpp` enxerga só cartela e sorteio; quem decide o que
  fazer com a resposta — imprimir "Fulano ganhou com a cartela N!", contar
  `ganhadores`, parar o laço — é `jogo.cpp`. Isso deixa a função pequena, fácil de
  testar sozinha (dá pra chamar com qualquer cartela e qualquer lista de sorteados
  na mão, sem rodar o jogo inteiro) e fácil de explicar em uma frase na defesa.

## Requisitos do enunciado que esta parte atende

- **R25** — `enunciado.txt:59-60` "o sorteio para quando um dos jogadores fizer
  'bingo'": é exatamente o que `cartelaCompleta` detecta a cada rodada; `jogo.cpp`
  usa o retorno dela pra decidir a condição do `while` que encerra o sorteio
  (`jogo.cpp:140`).
- **R30** — `enunciado.txt:70` "Utilize sub-rotinas e passagem de parâmetros":
  `cartelaCompleta` é uma sub-rotina que recebe 3 parâmetros (cartela, lista de
  sorteados, quantidade) e não depende de nada fora deles.
- **R32** — `enunciado.txt:72` "Não utilize variáveis globais": os 3 dados que a
  função usa chegam todos por parâmetro; nada é lido de fora do escopo da função.
- **Contribui para R29** (não implementa sozinha) — `enunciado.txt:66-67` "Ao final,
  deve ser exibido o nome do jogador e o número da cartela vencedora": o `true`/`false`
  de `cartelaCompleta` é o gatilho que `jogo.cpp` usa pra saber QUANDO imprimir isso
  (`jogo.cpp:98-131`); quem de fato imprime nome e número é `jogo.cpp`, não
  `vitoria.cpp`.

## Perguntas de defesa sobre esta parte

1. **Como o programa sabe que uma cartela está completa?**
   `cartelaCompleta` (`vitoria.cpp:10-24`) passa pelas 25 posições da cartela e, pra
   cada uma, pergunta pro sorteio (`verificarSorteado`, `sorteio.cpp:11-21`) se aquele
   número já saiu; basta uma resposta "não" pra já devolver `false`.

2. **Por que checar contra o vetor de sorteados, e não marcar direto na cartela
   quando o número sai?**
   Porque `numSorteados` já é a única lista que registra "o que saiu"; duplicar esse
   controle na cartela criaria duas fontes de verdade que precisariam ficar sempre
   iguais, com risco de desincronizar.

3. **`cartelaCompleta` é chamada quantas vezes por rodada?**
   Cinco, uma por cartela, sempre depois do número já ter sido pintado de verde na
   tela (`jogo.cpp:98,105,112,119,126`) — nunca no meio da rodada.

4. **Duas cartelas podem ganhar no mesmo sorteio?**
   Sim: as 5 chamadas rodam sempre, mesmo que uma já tenha dado `true` antes na mesma
   rodada (`jogo.cpp:98-131`), então dois jogadores podem ser anunciados juntos se o
   mesmo número completar as duas cartelas.

5. **Onde exatamente o sorteio para?**
   No `while` do fim de `jogo.cpp` (`jogo.cpp:140`): `while (ganhadores == 0 &&
   quantidade < 75)`. Assim que uma chamada de `cartelaCompleta` faz `ganhadores`
   subir pra mais que 0, essa condição falha e nenhum número novo é sorteado.

6. **Por que `quantidade` não é `&quantidade` em `cartelaCompleta`, se em `sorteio()`
   é?**
   Porque `cartelaCompleta` só lê a lista pra comparar; ela nunca precisa alterar o
   contador de quem chamou. `&` só se usa quando a função tem que devolver uma
   mudança pra fora — não é o caso aqui.

## Limites conhecidos

- `verificarSorteado` (`sorteio.cpp:11-21`), usado dentro de `cartelaCompleta`, faz
  busca linear nas posições já preenchidas de `numSorteados`, mesmo a lista já
  estando ordenada por `inserirEmOrdem` (`sorteio.cpp:23-35`). Daria pra usar busca
  binária, mas com no máximo 75 números sorteados a diferença nunca aparece na
  prática — não é falha, é uma troca consciente de simplicidade por uma otimização
  que não faz diferença nesse tamanho de problema.
- `cartelaCompleta` só reconhece a cartela cheia (as 25 posições), exatamente como o
  enunciado define vitória (`enunciado.txt:16`, `59-60`). Não existe checagem de
  linha, coluna ou diagonal isolada — se um dia pedirem "bingo de linha", essa função
  precisaria ser reescrita, não só ajustada.
