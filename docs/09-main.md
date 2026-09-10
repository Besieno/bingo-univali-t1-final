# Main — ponto de entrada: semeia o sorteio, redimensiona o console, reserva a memória do jogo e entrega tudo pro menu

## O que esta parte resolve

`main()` é a única função do arquivo `main.cpp`. Ela não joga bingo, não desenha nada e não lê nenhuma entrada do
jogador diretamente — o arquivo nem inclui `<iostream>` (`main.cpp:19-23`), e a função inteira (`main.cpp:27-38`)
não tem um único `cout` ou `cin`. O papel dela é só três coisas, nesta ordem: preparar o gerador de números
aleatórios pra a partida, preparar o terminal pra caber o tabuleiro, e reservar a memória (5 cartelas + os nomes)
que vai circular por todo o resto do programa. Feito isso, ela entrega o controle pra `chamarMenu` (`menu.cpp`) e
só volta a rodar quando o usuário escolhe Sair. Quem realmente joga é `jogo.cpp` (chamado a partir do menu); este
documento cobre só o que acontece antes disso, em `main.cpp`.

Não existe `main.h` neste projeto — confirmado olhando `src/` (17 arquivos, nenhum `main.h`). `main.cpp` usa
`comum.h` (a constante `TAM`) e `menu.h` (a assinatura de `chamarMenu`) direto.

## Arquivos e funções

| função | arquivo:linha | o que faz em uma linha |
|---|---|---|
| `main` | `main.cpp:27-38` | Semeia o sorteio, ajusta o console pra 120×30, declara as 5 cartelas e o vetor de nomes, e chama `chamarMenu` com tudo isso. |

## Como funciona, passo a passo

1. **`main.cpp:29` — `srand(time(NULL));`** Define a semente do gerador de números pseudoaleatórios pra esta
   execução do processo. `time(NULL)` devolve o relógio do sistema em segundos; usar isso como semente faz cada
   execução do `.exe` começar de um ponto diferente da sequência de `rand()`, que é usada mais tarde tanto pra
   gerar as cartelas (`cartela.cpp`) quanto pra sortear os números (`sorteio.cpp`). Roda uma única vez, no início
   do programa — não dentro do laço de `jogar` — porque a semente é uma propriedade do processo, não da partida.

2. **`main.cpp:30` — `system("mode con: cols=120 lines=30 > nul 2>&1");`** Redimensiona a janela do terminal
   antes de qualquer cartela existir. O tamanho 120×30 não foi chutado: é o valor medido rodando a versão final
   até o fim de uma partida real (72 números sorteados) — o layout das 5 cartelas lado a lado (`jogo.cpp:29-34`)
   precisa exatamente disso, sem sobra (ver `fatos-verificados.md`). O `> nul 2>&1` no fim manda a saída e o erro
   do comando `mode` pro "lugar nenhum", pra não aparecer uma mensagem de erro do Windows na tela caso o terminal
   não aceite o redimensionamento.

3. **`main.cpp:32-33` — as declarações de memória.**
   ```cpp
   int cartela1[TAM][TAM], cartela2[TAM][TAM], cartela3[TAM][TAM], cartela4[TAM][TAM], cartela5[TAM][TAM];
   string nome[TAM];
   ```
   `TAM` vem de `comum.h:23` (`#define TAM 5`) e nasceu pra descrever a cartela — 5 colunas por 5 linhas. As
   cinco matrizes `TAM×TAM` são as 5 cartelas do jogo, todas vazias neste ponto; `gerarCartela` (chamada dentro de
   `jogar`, `jogo.cpp:39-56`) é quem as preenche depois. `nome[TAM]` é o vetor com o nome de cada jogador — aqui
   `TAM` (5) está sendo reaproveitado pra significar "5 jogadores", que é um número diferente do que `TAM`
   originalmente descreve (ver Limites).

4. **`main.cpp:35` — `chamarMenu(cartela1, cartela2, cartela3, cartela4, cartela5, nome);`** Passa as 5 cartelas e
   o vetor de nomes pro menu. Arrays em C++ não são copiados na passagem por parâmetro — o nome da variável já
   decai pro endereço da memória — então `chamarMenu` (e, através dele, `jogar`) enxerga e escreve nas mesmas
   posições de memória que `main` reservou. Não é preciso `&` explícito aqui (diferente de `int &opcao` em
   `menu.cpp:15`, que é um `int` sozinho, não um array). A partir desta linha, `main` não faz mais nada até essa
   chamada retornar — e ela só retorna quando o usuário escolhe Sair no menu (`menu.cpp:73-78`).

5. **`main.cpp:37` — `return 0;`** Fecha o programa depois que `chamarMenu` devolveu o controle. É o único
   ponto de saída do `main`; a decisão de quando isso acontece pertence inteiramente ao laço do menu, não a esta
   função.

## Por que foi feito assim

- **Arrays de tamanho fixo (`TAM`), não `std::vector`.** O enunciado proíbe `template` (`enunciado.txt:71`), e
  `vector` é um template da biblioteca padrão — usar `vector<int>` já seria instanciar um. Como o tamanho da
  cartela é conhecido em tempo de compilação (sempre 5×5), um array comum resolve sem precisar de nada proibido.
  A alternativa "mais moderna" (`vector`/`array<T,N>`) simplesmente não está disponível dentro da regra do
  trabalho.

- **As 5 cartelas e os nomes nascem em `main`, não dentro de `jogar`.** O enunciado proíbe variável global
  (`enunciado.txt:72`, "Não utilize variáveis globais"). Só que esse dado precisa sobreviver e ser compartilhado
  por toda a cadeia `main → chamarMenu → jogar → gerarCartela/pintarNumero/...`. A saída sem variável global é
  declarar a memória uma vez, no escopo de `main`, e ir passando por parâmetro pra baixo — que é exatamente o que
  `enunciado.txt:70` pede ("Utilize sub-rotinas e passagem de parâmetros"). Se fosse declarada dentro de `jogar`,
  o efeito prático seria o mesmo (o enunciado permite variável local), mas colocar em `main` deixa explícito, no
  ponto de entrada do programa, que aquela é *toda* a memória de estado do jogo — não sobra nada escondido em
  outro lugar.

- **`srand` uma vez só, em `main`, e não em `sorteio.cpp`.** `sorteio.cpp` é quem chama `rand()` de fato, mas
  semear ali dentro rodaria de novo cada vez que o jogador escolhe "Jogar" no mesmo processo (o menu permite
  jogar várias partidas sem fechar o programa — `menu.cpp:56-78`). Rodar `srand(time(NULL))` mais de uma vez no
  mesmo segundo não muda nada (a semente já seria a mesma), então semear uma única vez, na abertura do programa,
  é mais simples e evita a falsa sensação de "resortear" a cada partida.

- **`system("mode con...")` fica em `main`, não em `jogo.cpp`/`exibicao.cpp`.** É configuração do terminal, não
  do jogo — precisa rodar antes de qualquer cartela ser desenhada, e só precisa rodar uma vez por execução do
  programa (não uma vez por partida). O ponto de entrada é o lugar natural pra isso.

## Requisitos do enunciado que esta parte atende

- **R12** — `enunciado.txt:48` "A cada nova execução deverá haver um novo sorteio com valores diferenciados" →
  `main.cpp:29`, `srand(time(NULL))`, semeando o sorteio a cada execução do `.exe`.
- **R30** — `enunciado.txt:70` "Utilize sub-rotinas e passagem de parâmetros." → `main.cpp:35` é o primeiro elo
  visível dessa cadeia: entrega as 5 cartelas e os nomes por parâmetro pra `chamarMenu`.
- **R31** — `enunciado.txt:71` "Não utilize templates" → `main.cpp:32-33` usa arrays de tamanho fixo (`TAM`,
  `comum.h:23`), não `vector`/`array<T,N>`.
- **R32** — `enunciado.txt:72` "Não utilize variáveis globais" → toda a memória do jogo (5 cartelas + nomes) é
  local a `main()` (`main.cpp:32-33`), não há nenhuma variável fora de função no arquivo.
- **R02 / R39** — `enunciado.txt:9` "Modalidade: Cinco integrantes." e `enunciado.txt:91` "Identificação dos
  autores [...] no código" → cabeçalho do arquivo (`main.cpp:8-13`) lista os 5 nomes da equipe.
- **R17 (precondição, não a exibição em si)** — `enunciado.txt:52` "Exibir na tela as cartelas geradas durante
  toda a execução." → `main.cpp:30` garante que a janela tem 120×30, o tamanho medido como necessário pra as 5
  cartelas caberem sem cortar; quem de fato mantém as cartelas na tela é `jogo.cpp`/`exibicao.cpp`, não `main`.

## Perguntas de defesa sobre esta parte

1. **Por que `srand(time(NULL))` está em `main.cpp` e não em `sorteio.cpp`, que é quem sorteia de verdade?**
   Porque a semente é uma propriedade da execução do processo, não da partida. Semear uma vez, no início, evita
   resemear (sem efeito, já que seria o mesmo segundo) toda vez que o jogador escolhe "Jogar" de novo no mesmo
   processo.

2. **O trabalho proíbe variável global. Como `main` garante isso, se 5 cartelas e os nomes precisam existir
   durante o jogo inteiro?** Elas são locais a `main()` (`main.cpp:32-33`), não globais, e viajam por parâmetro
   por toda a cadeia de chamadas (`main.cpp:35` → `chamarMenu` → `jogar`). Arrays em C++ passam o endereço, não
   uma cópia, então todo mundo na cadeia enxerga a mesma memória sem ela precisar ser global.

3. **Por que não usar `std::vector` pras cartelas?** `vector` é um template da biblioteca padrão, e o enunciado
   proíbe template (`enunciado.txt:71`). Como o tamanho é fixo e sabido em tempo de compilação (`TAM=5`), o array
   comum resolve sem esbarrar na regra.

4. **Rodando o `.exe` duas vezes bem rápido, o sorteio muda?** Nem sempre. `srand(time(NULL))` (`main.cpp:29`)
   usa o relógio em segundos como semente; se as duas execuções caírem no mesmo segundo, a semente é igual e o
   jogo sai idêntico. É uma limitação real, não uma pegadinha (ver Limites).

5. **O que faz o `mode con: cols=120 lines=30` em `main.cpp:30`, e por que o `> nul 2>&1` no final?** Redimensiona
   o terminal pra 120 colunas por 30 linhas — o tamanho medido como necessário pra as 5 cartelas caberem lado a
   lado sem cortar. O `> nul 2>&1` esconde qualquer mensagem de erro do Windows caso o terminal não aceite o
   comando, pra não sujar a tela antes mesmo do menu aparecer.

6. **Por que `nome` é declarado com tamanho `TAM`, se `TAM` é a medida da cartela (5×5), não o número de
   jogadores?** Porque neste trabalho os dois números coincidem — 5 cartelas, 5 jogadores, cartela 5×5 — então
   reaproveitar a mesma constante funciona. Não é logicamente a mesma coisa; é uma coincidência de valores (ver
   Limites).

## Limites conhecidos

- **`srand(time(NULL))` (`main.cpp:29`) tem resolução de 1 segundo.** Duas execuções do `.exe` dentro do mesmo
  segundo semeiam com o mesmo valor e produzem exatamente as mesmas cartelas e o mesmo sorteio, o que enfraquece
  R12 num caso específico (fácil de reproduzir rodando o programa duas vezes em sequência rápida). Conserto de
  uma linha, se perguntado: somar `clock()` (já disponível via `<time.h>`, incluído em `main.cpp:20`) à semente.

- **O retorno de `system(...)` em `main.cpp:30` não é verificado.** Se o terminal não aceitar o redimensionamento
  pra 120×30, o `> nul 2>&1` garante que nenhum erro aparece — mas também garante que ninguém percebe. O jogo
  seguiria rodando com o layout de `jogo.cpp` pensado pra uma tela que pode não existir de verdade.

- **`TAM` (`comum.h:23`) está sendo usado em `main.cpp:33` pra dimensionar `nome[]`, mas `TAM` foi definido pra
  descrever a cartela (5 linhas × 5 colunas), não a quantidade de jogadores.** Os dois valores são 5 só porque o
  enunciado fixa os dois em 5 (`enunciado.txt:11-14` e `:39`). Se um dia a cartela mudasse de tamanho sem que o
  número de jogadores mudasse junto, `nome[TAM]` quebraria por acidente — não é um bug hoje, mas é um
  acoplamento que não devia existir entre dois conceitos diferentes.

- **5 variáveis de cartela declaradas à mão (`main.cpp:32`), em vez de uma matriz 3D (`int[5][TAM][TAM]`).** Isso
  é uma decisão de projeto que se sustenta na defesa (ver "Por que foi feito assim"), mas tem um custo visível:
  toda função que mexe nas cartelas — `chamarMenu` (`menu.h:14`), `jogar` (`jogo.h:12`) — precisa de 5 parâmetros
  de cartela mais o vetor de nomes, em vez de 1.
