# Tela — a camada que fala com o terminal: onde escrever e de que cor, sem saber nenhuma regra de bingo

## O que esta parte resolve

O resto do jogo (cartela, sorteio, exibição, vitória) só sabe *o quê* mostrar: qual número, em qual linha da cartela, se já saiu ou não. Nenhuma dessas partes sabe *como* fazer o cursor do terminal pular para uma posição, nem como pintar o fundo de um caractere. `tela.h`/`tela.cpp` isolam esse "como": três funções (`gotoxy`, `ansiBG`, `textbackground`) que traduzem "linha X, coluna Y" e "cor tal" em sequências de escape ANSI que o próprio terminal interpreta. É a única parte do projeto que emite esses códigos — todo o resto chama `gotoxy`/`textbackground`, nunca escreve `\033[` na mão (exceto o `"\033c"` de limpar a tela inteira, que fica de fora daqui — ver Limites).

## Arquivos e funções

| Função | Arquivo:linha | O que faz em uma linha |
|---|---|---|
| `gotoxy` | `tela.h:14` (declaração) / `tela.cpp:13-17` (implementação) | Move o cursor do terminal para a coluna `x`, linha `y`. |
| `ansiBG` | `tela.h:15` / `tela.cpp:19-23` | Traduz a cor no padrão do professor (0 a 15) no código ANSI de fundo correspondente. |
| `textbackground` | `tela.h:16` / `tela.cpp:25-32` | Liga a cor de fundo do terminal, ou devolve o fundo padrão quando a cor é `BLACK`. |

Constantes usadas no projeto inteiro: `BLACK 0`, `GREEN 2`, `RED 4` (`tela.h:10-12`) — só essas três, porque é só disso que o jogo precisa (verde = já saiu, vermelho = acabou de sair, preto = "sem cor", usado pra apagar o destaque).

## Como funciona, passo a passo

**`gotoxy(x, y)` — `tela.cpp:13-17`**
Monta a string `"\033[" + (y+1) + ";" + (x+1) + "H"` e manda pro terminal. `\033` é o caractere ESC; seguido de `[linha;colunaH` é o comando ANSI "posicione o cursor" (CSI). O `+1` em cada eixo existe porque o protocolo ANSI conta linha e coluna a partir de **1**, enquanto o resto do jogo (laços `for`, índices de matriz) conta a partir de **0** — a soma é a tradução entre as duas contagens, feita num único lugar para não se repetir em cada chamador. Depois do `cout`, `cout.flush()` força a saída a sair do buffer imediatamente.

**`ansiBG(cor)` — `tela.cpp:19-23`**
Recebe um inteiro 0-15 (no padrão de cor do professor: `BLACK=0, BLUE=1, GREEN=2, AQUA=3, RED=4, ...`) e devolve, por uma tabela fixa (`int bg[16] = {40,44,42,46,41,45,43,47,100,104,102,106,101,105,103,107}`), o código ANSI de fundo equivalente. O acesso é `bg[cor % 16]`, então qualquer inteiro cai dentro da tabela (módulo evita estourar o array) — embora o jogo só chame essa função, indiretamente, com `BLACK`, `GREEN` e `RED` (confirmado varrendo todo o `src/`: nenhuma chamada com outro valor).

**`textbackground(newcolor)` — `tela.cpp:25-32`**
Se a cor pedida é `BLACK`, manda `"\033[49m"` — o código ANSI que restaura o fundo **padrão** do terminal. Caso contrário, monta `"\033[" + ansiBG(newcolor) + "m"`. `m` fecha o comando SGR (Select Graphic Rendition), que é o comando ANSI de estilo/cor. De novo, `cout.flush()` no final.

**Como o resto do jogo usa essas três funções (contexto em `jogo.cpp` e `exibicao.cpp`):**
`jogar()` guarda as coordenadas de cada cartela e do painel de avisos logo no topo da função (`jogo.cpp:29-35`), justamente para toda chamada de `gotoxy` no resto do código apontar pra essas mesmas variáveis — mudar o layout é mudar 7 linhas, não caçar número mágico espalhado. A cada rodada de sorteio, `jogo.cpp:78-82` chama `pintarNumero(..., RED)` para as 5 cartelas, espera um Enter (`jogo.cpp:85`) e então `jogo.cpp:88-92` repinta o mesmo número de `GREEN`. Quem de fato acerta a célula na tela é `pintarNumero` (`exibicao.cpp:22-50`): ela percorre a matriz procurando o número sorteado e, a cada célula que **não** é o número, recalcula `posicaoX` reproduzindo a mesma tabulação que `exibir` usa pra desenhar a cartela (`exibicao.cpp:42-47`, espelhando o `\t` de `exibicao.cpp:66`) — só quando acha o número é que chama `gotoxy` (`exibicao.cpp:33`) e o par `textbackground(cor)` / `cout << mat[i][j]` / `textbackground(BLACK)` (`exibicao.cpp:35-37`). O `textbackground(BLACK)` depois de cada número é o motivo de o `49` (fundo padrão) importar: sem ele, cada caractere impresso depois herdaria a cor do número anterior.

## Por que foi feito assim

**Por que sequência ANSI (`\033[...`) e não `windows.h`/`SetConsoleCursorPosition`?**
O professor entregou dois headers: `codefun.h` (versão Windows, via API do console) e `codefun_GDB.h` (versão ANSI). `tela.cpp` reimplementa exatamente o corpo das funções `gotoxy` e `textbackground` de `codefun_GDB.h` (comparar `tela.cpp:13-17` com `codefun_GDB.h:31-35`, e `tela.cpp:25-32` com `codefun_GDB.h:78-84` — o código é idêntico, só sem o `static` em `ansiBG`). A alternativa (`windows.h`) prende o programa ao Windows e exige mais código (handle do console, struct `COORD`); a versão ANSI funciona em qualquer terminal que entenda VT, incluindo o Windows Terminal padrão, com bem menos código.

**Por que reimplementar em vez de `#include "codefun_GDB.h"` direto?**
O header do professor traz `gotoxy`, `clrscr`, `delay`, `random`, `randomize`, `pausar`, `textcolor`, `textbackground` e `movimenta` — mais funções do que o Bingo usa — e inclui `<unistd.h>` (POSIX) por causa do `delay`. Copiar só as três funções necessárias (`gotoxy`, `ansiBG`, `textbackground`) evita arrastar uma dependência POSIX e uma função de teclado (`movimenta`) que o jogo não usa, e mantém a entrega em um arquivo único sem depender de anexar o header do professor.

**Por que `ansiBG` é uma tabela fixa e não uma conta tipo `40 + cor`?**
A ordem de cores do professor é `BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, WHITE` (0 a 7); a ordem dos códigos ANSI de fundo é `40=preto, 41=vermelho, 42=verde, 43=amarelo, 44=azul, ...`. As duas ordens **não batem** — `RED` é 4 no padrão do professor mas precisa virar `41`, não `44`. Uma fórmula linear erraria a cor; só um mapeamento posição-a-posição (a tabela) acerta as 16 combinações. É por isso que o valor de `tela.cpp:21` é copiado literalmente da tabela do professor (`codefun_GDB.h:66`), não inventado.

**Por que `textbackground(BLACK)` manda `\033[49m` em vez de `\033[40m`?**
`40` pintaria o fundo de preto **literal**. `49` é o código ANSI "volte ao fundo padrão do terminal" — o mesmo efeito de nunca ter mudado a cor. Como `textbackground(BLACK)` é usado pra "apagar" o destaque depois de pintar um número (`exibicao.cpp:37`, `94`), usar `49` é o que garante que o fundo volta a ser o do terminal do usuário, e não um preto forçado que destoaria em terminais com tema diferente.

**Por que três funções soltas e não uma classe/objeto "Tela"?**
O enunciado proíbe recursos não vistos em aula (`enunciado.txt:72`) e pede sub-rotinas com passagem de parâmetro (`enunciado.txt:70`) — não pede (nem sugere) orientação a objetos. Três funções livres, cada uma recebendo só o que precisa por parâmetro, cumprem exatamente o que foi pedido sem introduzir um recurso de linguagem que teria que ser explicado e defendido à parte.

## Requisitos do enunciado que esta parte atende

- **R18** — `enunciado.txt:53` "Marcar os valores já sorteados nas cartelas com cores diferenciadas": só existe porque `textbackground`/`ansiBG` sabem pintar o fundo de um número.
- **R23** — `enunciado.txt:57-58` "destacando o atual valor sorteado com uma cor diferente": é o `RED` vs `GREEN` que passa por `textbackground` a cada chamada de `pintarNumero`/`exibirSorteados`.
- **R27** — `enunciado.txt:63-64` "deve-se destacar o valor sorteado na cartela": depende de `gotoxy` achar a célula certa antes de `textbackground` pintar.
- **R32** — `enunciado.txt:72` "Não utilize variáveis globais": as três constantes de cor (`tela.h:10-12`) são `#define` — macro de pré-processador, substituída em tempo de compilação, não uma variável que existe em tempo de execução.
- **R33** — `enunciado.txt:72` "nenhum comando ainda não abordado na disciplina": o `\033[...` não é um comando "de fora" — é literalmente o corpo de `gotoxy`/`textbackground` do `codefun_GDB.h` que o próprio professor forneceu.

## Perguntas de defesa sobre esta parte

1. **O que exatamente `\033[` faz o terminal fazer?** — `\033` é o caractere ESC; a sequência `\033[linha;colunaH` é um comando ANSI (CSI) que o terminal reconhece como "mova o cursor", não como texto pra imprimir.
2. **Por que `gotoxy` soma 1 a `x` e a `y`?** — O protocolo ANSI conta linha/coluna a partir de 1; o resto do jogo (matrizes, laços) conta a partir de 0. A soma faz essa conversão num lugar só.
3. **Isso não é um comando fora do que foi visto em aula?** — Não: é o mesmo código de `gotoxy` e `textbackground` do `codefun_GDB.h` que o professor distribuiu (`codefun_GDB.h:31-35` e `78-84`); só não incluímos o header porque ele traz funções que o Bingo não usa.
4. **Por que `ansiBG` usa uma tabela de 16 posições em vez de uma conta?** — Porque a ordem de cores do professor não é a mesma ordem dos códigos ANSI de fundo (`RED` é 4 pra ele, mas precisa virar 41, não 44); só uma tabela posição-a-posição acerta as 16 cores.
5. **Por que `textbackground(BLACK)` manda `49` e não `40`?** — `49` restaura o fundo padrão do terminal; `40` forçaria preto literal, o que destoaria em terminal com outro tema. É usado pra "apagar" a cor depois de pintar um número.
6. **Por que `cout.flush()` depois de cada escape, se `cin` já é amarrado a `cout` por padrão?** — É verdade que ler de `cin` já força o flush de `cout`; o `flush()` explícito é redundante nesse fluxo específico, mas garante a atualização visual mesmo se algum trecho futuro usar `cin` de outra forma. Defensivo, não estritamente necessário hoje.

## Limites conhecidos

- `ansiBG`/`textbackground` não validam o intervalo de `cor`/`newcolor`. Hoje nenhuma chamada real usa algo além de `BLACK`, `GREEN` ou `RED` (conferido varrendo todo `src/`), mas nada no tipo (`int`) impede alguém passar um valor negativo — `cor % 16` de um número negativo em C++ pode indexar `bg[]` fora dos limites (comportamento indefinido). Risco teórico, não observado na prática.
- `tela.cpp` não faz nada para garantir que o terminal interpreta ANSI (não há chamada equivalente a `SetConsoleMode`/`ENABLE_VIRTUAL_TERMINAL_PROCESSING`) — ele confia que o ambiente já suporta VT, o que é o padrão no Windows Terminal moderno mas não é garantido em todo `cmd.exe`. Se o terminal não interpretar, os códigos aparecem como texto literal na tela em vez de mover o cursor.
- A limpeza de tela inteira (`"\033c"`) **não é uma função deste arquivo** — aparece como string literal direto em `jogo.cpp:37`, `menu.cpp:44,58,74` e `jogadores.cpp:22`. Uma função `limparTela()` dentro de `tela.h` teria centralizado isso, do mesmo jeito que `gotoxy`/`textbackground` centralizam o resto.
- `gotoxy` não valida `x`/`y` contra o tamanho real do terminal — se algum chamador (`jogo.cpp`/`exibicao.cpp`) passar coordenada fora da janela aberta por `system("mode con: cols=120 lines=30 ...")` em `main.cpp:30`, o texto simplesmente não aparece onde deveria. Manter as coordenadas dentro da tela é responsabilidade de quem chama, não desta camada.
