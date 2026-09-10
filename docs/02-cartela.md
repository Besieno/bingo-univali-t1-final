# Cartela — fábrica de cartelas válidas: gera, ordena e compara os cartões que os jogadores vão marcar

## O que esta parte resolve

`cartela.h`/`cartela.cpp` respondem duas perguntas do enunciado, e só essas: como fabricar um cartão de 25 números que respeita a faixa de cada linha e não repete número dentro da própria linha (`enunciado.txt:39-46`, `50`), e como saber se dois cartões são idênticos, para a partida nunca sortear duas cartelas iguais (`enunciado.txt:51`). Não decide nada de tela, nome de jogador ou sorteio de bingo — essas partes vivem em `exibicao`, `jogadores` e `sorteio`. Aqui só existe a matriz `int[TAM][TAM]` (`TAM` = 5, `comum.h:23`) e as quatro funções que a criam e a validam; `jogo.cpp` é quem usa esse material para montar a partida.

## Arquivos e funções

| Função | Arquivo:linha | O que faz em uma linha |
|---|---|---|
| `bubblesort` | `cartela.cpp:11-32` (declarada em `cartela.h:11`) | Ordena uma linha da cartela em ordem crescente, trocando vizinhos fora de ordem |
| `numeroRepetidoNaLinha` | `cartela.cpp:34-44` (`cartela.h:12`) | Diz se um número já apareceu nas colunas já preenchidas daquela linha |
| `gerarCartela` | `cartela.cpp:46-66` (`cartela.h:13`) | Preenche as 5 linhas da cartela, cada uma dentro da sua faixa (1-15, 16-30, ..., 61-75), sem repetir e já ordenada |
| `cartelasIguais` | `cartela.cpp:68-81` (`cartela.h:14`) | Compara duas cartelas posição a posição e diz se são idênticas |

## Como funciona, passo a passo

**Gerar uma linha sem repetir (`gerarCartela`, `cartela.cpp:46-66`).** O laço externo (`52`) percorre as 5 linhas. Para a linha `i`, `limiteInicial = 15*i+1` e `limiteFinal = 15*(1+i)` (`54-55`) — isso dá exatamente 1-15 na linha 0, 16-30 na linha 1, e assim até 61-75 na linha 4, seguindo a tabela do enunciado (`enunciado.txt:42-46`). Para cada uma das 5 colunas dessa linha, um `do-while` (`58-62`) sorteia um número dentro da faixa e só aceita se `numeroRepetidoNaLinha` disser que ele ainda não saiu; se saiu, sorteia de novo. É "gerar e testar", não "embaralhar e pegar": mais simples de escrever, ao custo de eventualmente sortear um número repetido e descartar — o que sai barato aqui (ver seção de limites). Depois que a linha inteira está preenchida, `bubblesort(mat, i)` (`64`) ordena só aquela linha antes de passar para a próxima.

**Só olhar o que já existe (`numeroRepetidoNaLinha`, `cartela.cpp:34-44`).** O parâmetro `coluna` é a posição que `gerarCartela` está tentando preencher agora; o laço (`36`) varre só `0` até `coluna-1` — as posições que já têm um número de verdade. As colunas depois de `coluna` ainda não foram decididas nesta chamada de `gerarCartela` (a matriz `mat` é reaproveitada entre as 5 cartelas da partida, então comparar com elas seria comparar com o lixo de uma geração anterior).

**Ordenar sem `std::sort` (`bubblesort`, `cartela.cpp:11-32`).** `cond` (`14`) começa em 1 e o `for` externo (`16`) só continua enquanto `cond == 1`. Cada passagem interna (`21-30`) compara vizinhos e troca quando estão fora de ordem, marcando `cond = 1` (`28`) sempre que troca alguma coisa. Se uma passagem inteira não trocar nada, `cond` permanece 0 e o `for` externo para — para uma linha de 5 elementos quase ordenada, isso evita rodar as 4 passagens completas à toa.

**Duas cartelas são a mesma? (`cartelasIguais`, `cartela.cpp:68-81`).** Percorre as 25 posições (`70-79`) e devolve `false` no primeiro par diferente (`74-77`); só devolve `true` se todas as 25 baterem (`80`). Compara só duas matrizes por vez — quem decide *quantas* comparações fazer é quem chama.

**Quem garante que as 5 cartelas da partida são diferentes entre si.** Isso não acontece dentro de `cartela.cpp` — é `jogo.cpp` que orquestra: gera `mat1` direto (`jogo.cpp:39`) e, para cada cartela seguinte, envolve `gerarCartela` num `do-while` que testa `cartelasIguais` contra *todas* as anteriores antes de aceitar (`mat2` contra `mat1` em `41-44`; `mat3` contra `mat1` e `mat2` em `45-48`; e assim em cascata até `mat5` contra as outras quatro em `53-56`). Cada cartela nova continua sendo regerada do zero enquanto bater com alguma já aceita.

## Por que foi feito assim

- **"Sortear e testar" em vez de embaralhar um vetor com os 15 valores da faixa e pegar os 5 primeiros.** O embaralhamento (tipo Fisher-Yates) exigiria um vetor auxiliar de 15 posições e uma lógica de remoção/swap que não estava no material da disciplina. `numeroRepetidoNaLinha` + `do-while` usa só o que já é conhecido — laço, sorteio, comparação — e sai mais curto.
- **Ordenar por bubblesort, depois de gerar a linha, em vez de já nascer ordenada.** Gerar já em ordem cresceria a lógica de geração (teria que sortear e inserir na posição certa ao mesmo tempo em que checa repetição). Separar em "gera sem se importar com ordem" + "ordena no fim" mantém cada função com uma responsabilidade só. O algoritmo escolhido é o bubblesort porque é o que o material do professor cobre (PDF "Métodos de Pesquisa e ordenação", citado em `docs/99-o-que-foi-medido.md:27`); usar `std::sort` puxaria `<algorithm>` e entraria no risco do enunciado de "comando ainda não abordado" (`enunciado.txt:72`, R33). Para 5 elementos o custo de bubblesort (no máximo 10 comparações) não pesa.
- **`numeroRepetidoNaLinha` olhar só até `coluna`, não a linha inteira.** Comparar com posições ainda não preenchidas na mesma chamada de `gerarCartela` compararia com valor indefinido — não é proteção extra, é a definição correta do problema: só o que já foi decidido pode estar repetido.
- **`cartelasIguais` comparar só um par, sem embutir a checagem das 5 cartelas.** Uma função com uma responsabilidade (comparar A com B) é reaproveitável e fácil de testar isoladamente; o preço é `jogo.cpp` precisar encadear as chamadas certas (`41-56`) para cobrir as 5 — mais código ali, não aqui.
- **Parâmetro `int mat[][TAM]` em vez de struct ou alocação dinâmica.** É a forma de passar uma matriz por referência que a disciplina ensina: um array 2D como parâmetro decai para ponteiro, então `bubblesort` e `gerarCartela` alteram a cartela de quem chamou sem precisar devolver nada e sem usar `&` (diferente de `sorteio`/`inserirEmOrdem`, que usam referência explícita porque o parâmetro ali é um `int` sozinho, não um array).

## Requisitos do enunciado que esta parte atende

- **R09** — cartela com 25 números, 5 colunas × 5 linhas: `gerarCartela` preenche `TAM × TAM` posições (`cartela.cpp:52-63`). Enunciado: `enunciado.txt:13-14,39` "cada cartela possui cinco colunas com cinco linhas... cada cartela possui 25 números".
- **R10** — faixa de valores por linha: `limiteInicial`/`limiteFinal` (`cartela.cpp:54-55`). Enunciado: `enunciado.txt:42-46` (1-15, 16-30, 31-45, 46-60, 61-75).
- **R15** — sem valores repetidos na mesma linha: `numeroRepetidoNaLinha` (`cartela.cpp:34-44`) + retentativa em `gerarCartela` (`58-62`). Enunciado: `enunciado.txt:50` "Não pode haver valores repetidos nas linhas".
- **R16** — sem cartelas repetidas entre as 5: `cartelasIguais` (`cartela.cpp:68-81`), usada por `jogo.cpp:41-56`. Enunciado: `enunciado.txt:51` "Não pode haver cartelas repetidas".
- **R19** — linhas em ordem crescente: `bubblesort` chamado ao fim de cada linha (`cartela.cpp:64`). Enunciado: `enunciado.txt:54` "As linhas devem estar em ordem crescente".
- **R30** — sub-rotinas com passagem de parâmetro: as 4 funções recebem a matriz (ou parte dela) como parâmetro; nenhuma mexe em variável fora do próprio escopo. Enunciado: `enunciado.txt:70` "Utilize sub-rotinas e passagem de parâmetros".
- **R32** — sem variável global: `TAM` é macro de pré-processador (`comum.h:23`), não variável; todo `int`/`mat` usado aqui é local a alguma função. Enunciado: `enunciado.txt:72` "Não utilize variáveis globais".
- **R41** — recursos da linguagem: array bidimensional como parâmetro, algoritmo de ordenação implementado à mão (bubblesort) em vez de biblioteca pronta. Enunciado: `enunciado.txt:93` "Recursos da linguagem utilizados = 20% da nota".

(R11 — "gerar 5 cartelas" — depende desta parte mas não é decidido aqui: `gerarCartela` fabrica uma cartela por chamada; quem chama 5 vezes e decide repetir em caso de empate é `jogo.cpp:39-56`.)

## Perguntas de defesa sobre esta parte

1. **Como vocês calculam a faixa de cada linha?** `limiteInicial = 15*i+1` e `limiteFinal = 15*(1+i)` (`cartela.cpp:54-55`) — para `i` de 0 a 4 isso dá 1-15, 16-30, 31-45, 46-60, 61-75, exatamente a tabela do enunciado.
2. **Como se evita número repetido na mesma linha?** `numeroRepetidoNaLinha` (`cartela.cpp:34-44`) varre as colunas já preenchidas daquela linha; o `do-while` de `gerarCartela` (`58-62`) sorteia de novo enquanto o número vier repetido.
3. **Por que `numeroRepetidoNaLinha` só olha até a coluna atual, e não a linha inteira?** Porque as colunas depois da atual ainda não foram preenchidas nesta chamada — comparar com elas seria comparar com valor indefinido ou com a geração anterior daquela mesma matriz.
4. **Como se garante que as 5 cartelas não se repetem?** `cartelasIguais` (`cartela.cpp:68-81`) compara duas cartelas posição a posição; `jogo.cpp` (`41-56`) regenera a cartela nova em `do-while` enquanto ela bater com qualquer uma das já aceitas.
5. **Por que ordenar a linha só no fim (bubblesort), e não já gerar em ordem?** Separa duas responsabilidades simples (gerar sem repetir; depois ordenar) em vez de uma lógica só complicada. Bubblesort é o algoritmo do material do professor; para 5 elementos custa no máximo 10 comparações.
6. **O que faz a variável `cond` no bubblesort?** É o sinalizador de troca (`cartela.cpp:14,19,28`): fica 1 enquanto alguma troca acontece numa passagem; se uma passagem inteira não troca nada, `cond` continua 0 e o `for` externo (`16`) para, sem rodar as passagens restantes à toa.

## Limites conhecidos

- **A faixa por linha está com o número `15` escrito direto no código (`cartela.cpp:54-55`), não derivado de `75 / TAM`.** Funciona porque o enunciado fixa 75 números em 5 linhas, mas se alguém mudasse `TAM` em `comum.h:23` sem mexer aqui, a faixa por linha pararia de fazer sentido e ninguém seria avisado pelo compilador.
- **O `do-while` de retentativa (`cartela.cpp:58-62`) não tem limite de tentativas.** Isso nunca trava na prática porque cada linha sempre tem mais valores possíveis (15) do que posições a preencher (5) — sempre sobra número livre —, mas essa garantia está na matemática do enunciado, não escrita em nenhum comentário ou checagem do código.
- **`cartelasIguais` compara só um par de cartelas por vez.** A garantia de que as 5 são todas diferentes entre si não mora aqui: depende inteiramente de `jogo.cpp:41-56` encadear a chamada certa contra todas as cartelas anteriores. Uma 6ª cartela exigiria lembrar de adicionar mais uma comparação à mão nesse encadeamento.
