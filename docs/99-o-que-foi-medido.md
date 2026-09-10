> **O que é este arquivo:** a lista do que foi conferido **rodando de verdade** — compilação,
> bugs reproduzidos, tamanho de tela medido, e o que o material do professor diz. Nada aqui é
> opinião: cada linha veio de um comando executado nesta máquina em 10/09/2026.

# Fatos verificados na máquina (medidos, não inferidos) — 10/09/2026

Tudo aqui foi conferido rodando/compilando de verdade. Pode citar como fato.

## Compilação
- Compilador real: `g++ (MinGW-W64 x86_64-ucrt-posix-seh) 16.1.0`.
- VERSÃO FINAL (arquivo único e módulos): compila com `-std=c++17 -Wall -Wextra` **sem nenhum aviso**.
- VERSÃO DO JOÃO: compila, mas com **1 aviso**: `'opcao' is used uninitialized` (`main (5).cpp:335`, declarada em `:332`).
- A versão do João usa `string` sem `#include <string>`; compila só porque a libstdc++ arrasta `<string>` por dentro de `<iostream>`. Não é garantido em outro compilador.

## Bugs reproduzidos rodando (versão do João)
- **Letra no menu = laço infinito**: `printf 'a\n' | main5.exe` cuspiu **mais de 4 MB** de "Valor inválido" em 4 segundos, sem parar. A mesma entrada na versão FINAL produz 270 bytes e sai limpo.
- **Nome com espaço**: digitando `Joao Vitor` para o jogador 1, a saída real mostra `Jogador: Joao` na cartela 1 e `Jogador: Vitor` na cartela 2 — e o 5º jogador nunca é perguntado (o nome dele é consumido pela sobra).
- **Vencedor sem o número da cartela**: a saída real do João diz só `ganhou!`; a FINAL diz `ganhou com a cartela 3!`.
- **Lista de sorteados**: a saída do João não contém nenhuma ocorrência de lista de números sorteados; a FINAL exibe `Numeros sorteados (N/75)` e a lista crescente.

## Tamanho de tela exigido (medido RENDERIZANDO a saída real num grid, como o terminal faria)
- VERSÃO DO JOÃO: **122 colunas × 36 linhas**. Não cabe nem em `cmd` 80×25 nem em Windows Terminal 120×30.
- VERSÃO FINAL: **115 colunas × 26 linhas** com os nomes reais da equipe (os mais longos) e 65 números já sorteados. Cabe em 120×30 com folga.
- (Uma medição anterior, mais grosseira, dizia 128×36 e 120×30: ela expandia cada tab em 8 colunas fixas em vez de ir até a próxima parada de tabulação. Valem os números de cima.)
- Renderizando o tabuleiro inteiro: na versão final as 5 cartelas não se sobrepõem e a cor cai exatamente em cima do número certo nas três colunas iniciais diferentes (2, 42 e 82). Na versão do João, as cartelas 2 e 5 começam na coluna 90 e a mensagem do número sorteado vai para a linha 36.

## Material do professor (existe no disco, em `Desktop\Algoritmos e programação 2\`)
- `codefun.h` — versão Windows: `gotoxy` por `SetConsoleCursorPosition`, `clrscr()` por `system("cls")`.
- `codefun_GDB.h` — versão ANSI: **`gotoxy` é exatamente `cout << "\033[" << (y+1) << ";" << (x+1) << "H"`**, e `textbackground` e a tabela `ansiBG` de 16 posições são **idênticos** aos usados na versão FINAL do trabalho.
  Consequência para a defesa: usar `\033[` NÃO é "comando de fora da disciplina" — é o código que o próprio professor entregou. Só é preciso saber dizer isso.
- Os dois headers definem `BLACK 0`, `BLUE 1`, `GREEN 2`, `AQUA 3`, **`RED 4`**.
  A versão FINAL usa esses valores. A versão do João define **`RED 1`**, que no padrão do professor é AZUL — funciona por acaso porque o `ansiBG` dele também foi simplificado para `40 + cor`.
- O professor também entregou o PDF *Métodos de Pesquisa e ordenação* (é de onde vem o bubblesort e a inserção ordenada) e *Subrotinas - Exercícios* / *Escopo de Variáveis*.

## Equivalência provada por script
- Cada uma das 21 funções da VERSÃO FINAL foi comparada, ignorando espaço e comentário, entre `main.cpp` original, os módulos em `src/` e o arquivo único gerado em `entrega/bingo.cpp`: **todas batem, e nenhuma linha de código do original ficou de fora**.
- O mesmo teste rodou para as 20 funções da versão do João: os módulos de `src/` são o código dele recortado, sem alteração de lógica.

## Detalhe de teste que engana
- Rodar o jogo com a entrada vinda de **arquivo** (`programa.exe < entrada.txt`) faz o `system("mode con: ...")` do `main` consumir a entrada, e o programa sai na hora. Com **pipe** (`cat entrada.txt | programa.exe`) funciona normal. Isso é artefato do teste, não bug do jogo — quem digita no teclado nunca vê isso.

## Tela de vitória da versão do João — renderizada da saída real

```
36|Numero sorteado: 62
37|Caio ganhou!
38|Tem 1 ganhador!
```

Três coisas provadas de uma vez:
1. A mensagem sai **sem o número da cartela** — só `Caio ganhou!` (o enunciado pede nome **e** número).
2. Ela cai nas linhas **37 e 38**, porque o `cout << endl` escreve na posição em que o cursor estava
   (linha 36, onde `apagarSorteado` deixou o cursor). Num terminal de 30 linhas isso **rola a tela**:
   as cartelas sobem e saem de vista, quebrando o "exibir as cartelas durante toda a execução".
3. `Numero sorteado: 62` **continua na linha 36** depois de `apagarSorteado()` — a prova de que
   imprimir `""` não apaga nada.

Com a vitória na tela, a versão do João ocupa **122 colunas × 38 linhas**.

## Semente repetida dentro do mesmo segundo (versão final)

Dez partidas rodadas em sequência, sem intervalo, saíram em **dois grupos de cinco jogos
idênticos** — mesmas cartelas, mesma ordem de sorteio. Repetindo com 1 segundo de intervalo
entre execuções, seis partidas saíram todas diferentes.

É o efeito de `srand(time(NULL))`: `time(NULL)` conta em segundos, então execuções no mesmo
segundo recebem a mesma semente. Sustenta o item C2 de `91-o-que-pode-ser-contestado.md`.

## Laço infinito da versão do João, medido com precisão

`printf 'a\n' | bingo.exe` (versão do João), cortado em 2 segundos:

| Para onde vai a saída | Bytes em 2 s | Repetições de "Valor inválido" |
|---|---|---|
| arquivo (`> saida.txt`) | 59.662.897 (~57 MB) | 1.084.778 |
| pipe (`\| wc -c`) | 125.347.142 (~120 MB) | — |

A diferença entre os dois é só a velocidade de escrita do destino. O laço não para sozinho:
só com Ctrl+C, ou quando o disco enche. A mesma entrada na versão final produz 270 bytes e sai limpo.

## Teste de aceitação automático (versão final)

`ferramentas/conferir-partida.mjs` rodado em 16 partidas: **16 passaram**. Ele confere 5 cartelas
na tela, faixa por linha, linhas crescentes e sem repetido, as 5 cartelas diferentes entre si,
lista de sorteados crescente e sem repetir dentro de 1–75, e se o vencedor anunciado tem mesmo
os 25 números já sorteados. São 400 linhas de cartela conferidas.

---

# Versão do grupo de 10/09/2026 (a que está no ar)

Tudo abaixo foi medido nesta versão, depois da troca. O que está acima vale para a versão
anterior — não misture os números.

## Compilação
- `src/*.cpp` e `entrega/bingo.cpp` compilam com `-Wall -Wextra` com **2 avisos**, os mesmos
  do arquivo único que o grupo mandou:
  - `'linha' set but not used` (`exibicao.cpp`, dentro de `mostrarSorteado`)
  - `'opcao' is used uninitialized` (`menu.cpp`, dentro de `chamarMenu`)
- A separação em 10 módulos não mudou nada: **as 21 funções batem byte-a-byte** (ignorando
  espaço e comentário) com o `main (6).cpp` original, e nenhuma linha de código ficou de fora.

## O que esta versão faz certo (conferido rodando, 6 partidas)
`ferramentas/conferir-partida.mjs` em 6 partidas: **6 passaram**.
- 5 cartelas na tela, cada linha na faixa certa, crescente, sem repetido (150 linhas conferidas)
- as 5 cartelas diferentes entre si
- **a lista de números sorteados aparece na tela, em ordem crescente, sem repetir, dentro de 1–75**
  — com o número da vez em fundo vermelho
- o vencedor anunciado tem mesmo os 25 números já sorteados

## O que foi medido de errado
- **Letra no menu = laço infinito.** `printf 'a\n' | bingo.exe` cortado em 2 s: **86.106.591 bytes**
  de "Valor inválido", sem parar. `lerOpcao` não chama `cin.clear()`.
- **Entrada que termina também trava.** Num teste em que a entrada acabou sem escolher "Sair",
  o programa encheu **9,3 GB** de arquivo em segundos (mesma causa: `cin` em estado de erro
  nunca mais bloqueia). Com teclado humano isso não acontece — mas Ctrl+Z reproduz.
- **Tabuleiro de 122 colunas × 37 linhas** (medido renderizando a saída real). Não cabe em
  `cmd` 80×25 nem em Windows Terminal 120×30: as cartelas 2 e 5 começam na coluna 90, e a
  lista de sorteados é impressa a partir da linha 36.
- **O vencedor sai sem o número da cartela:** `A cartela do jogador Henrique ganhou!`.
  O enunciado pede nome **e** número (R29).
- **Três partidas seguidas saíram idênticas** (mesma semente, `srand(time(NULL))` conta em
  segundos). Com 1 s de intervalo, as três seguintes saíram diferentes.

## Nota de método
Testar esta versão alimentando o jogo por arquivo é perigoso: se a entrada acabar sem um `3`
(Sair), o laço infinito do menu enche o disco. Use sempre um teto:
`cat entrada.txt | ./bingo.exe | head -c 8000000 > saida.txt`.

## Depois das 4 correções (10/09/2026, ainda na versão do grupo)

17 linhas tocadas em 3 arquivos. Medido depois:

| Teste | Antes | Depois |
|---|---|---|
| `printf 'a\nb\n9\n3\n' \| bingo.exe` | 86.106.591 bytes em 2 s, sem parar | **321 bytes**, três avisos e sai limpo |
| nome "Joao Vitor Silva da Cruz" | virava 2 jogadores; o 5º nunca era perguntado | fica inteiro na cartela 1; os 5 nomes compostos entram certos |
| anúncio do vencedor | `A cartela do jogador X ganhou!` | `A cartela 1 do jogador X ganhou!` |
| tela Sobre | `Maio de 2026` | `Setembro de 2026` |
| `conferir-partida.mjs` | 6 de 6 passaram (com aviso do número da cartela) | **6 de 6 passaram, sem aviso nenhum** |
| avisos de `-Wall -Wextra` | 2 | 2 (os mesmos: `linha` e `opcao` — itens C7 e C8, não tocados) |

O que **não** mudou e continua valendo: o tabuleiro ainda precisa de 122 colunas × 37 linhas
(item C5), e duas execuções no mesmo segundo ainda geram a mesma partida (item C9).

Detalhe do conserto do nome: com `getline` não sobra mais `\n` no buffer depois dos nomes,
então os dois `cin.ignore()` seguidos que vinham antes do sorteio viraram **um**
`cin.ignore(1000, '\n')` (`src/jogo.cpp:51`). Sem essa troca o jogo pediria dois Enters ali.

## Segunda rodada: os 2 avisos do compilador zerados (10/09/2026)

Duas mudanças, nenhuma de comportamento:
- `src/menu.cpp:46` — `int opcao;` → `int opcao = 0;`
- `src/exibicao.cpp:18` e `:39` — a variável `linha` (criada e incrementada, nunca lida) foi removida

Resultado:

| | Antes | Depois |
|---|---|---|
| `g++ -std=c++17 -Wall -Wextra` em `src/*.cpp` | 2 avisos | **0** |
| o mesmo na `entrega/bingo.cpp` | 2 avisos | **0** |
| com `-Wpedantic` também | — | **0** |
| 3 partidas no `conferir-partida.mjs` | — | **3 passaram** |
| letra no menu | 321 bytes e sai limpo | 321 bytes e sai limpo (igual) |

A lista de sorteados continua quebrando a cada 25 números: quem faz isso é o `cout << endl`,
não o contador que foi removido — conferido nas 3 partidas (66, 70 e 68 números lidos em
3 linhas, batendo com o contador `(N/75)` da tela).
