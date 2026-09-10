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
