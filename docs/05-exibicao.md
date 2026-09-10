# Exibicao — desenha as cartelas, o número sorteado e a lista de sorteados na tela, sem decidir nenhuma regra do jogo

## O que esta parte resolve

O Bingo mostra tudo ao mesmo tempo no console: 5 cartelas fixas, uma linha de aviso com o número que acabou de sair, e uma lista que cresce a cada rodada com todos os números já cantados — e nada disso pode sumir ou embaralhar durante a partida (`enunciado.txt:52`, `60-61`). `exibicao.h`/`exibicao.cpp` são o único lugar do projeto que escreve caractere na tela e escolhe cor de fundo. Este módulo não sorteia (`sorteio.cpp`), não gera cartela (`cartela.cpp`) e não decide vencedor (`vitoria.cpp`) — ele recebe dado pronto (matriz, nome, número, cor) e só posiciona.

Duas tarefas diferentes moram aqui: desenhar uma cartela **do zero** (`exibir`) e depois **atualizar só um pedacinho** dela sem redesenhar tudo (`pintarNumero`), mais manter uma segunda tela, a lista de sorteados, que cresce de 0 até 75 itens (`mostrarSorteado` + `exibirSorteados`).

## Arquivos e funcoes

| Função | arquivo:linha | O que faz |
|---|---|---|
| `exibir` | `exibicao.cpp:52-69` (protótipo `exibicao.h:13`) | Desenha o cabeçalho (ID + nome do jogador) e a grade de 25 números de uma cartela. |
| `pintarNumero` | `exibicao.cpp:22-50` (protótipo `exibicao.h:14`) | Acha um número dentro de uma cartela já desenhada e repinta só aquela célula, com a cor recebida por parâmetro. |
| `mostrarSorteado` | `exibicao.cpp:14-20` (protótipo `exibicao.h:15`) | Apaga e reescreve a linha "Numero sorteado: N" na área de aviso. |
| `exibirSorteados` | `exibicao.cpp:71-103` (protótipo `exibicao.h:16`) | Redesenha do zero a lista inteira de números já sorteados, em ordem crescente, com o atual em vermelho. |

Quem chama as quatro é `jogo.cpp`; as coordenadas de tela de cada cartela ficam reunidas em `jogo.cpp:29-35` (x1/y1..x5/y5, xSorteados/ySorteados, xAviso/yAviso), de propósito, para mexer no layout num lugar só.

## Como funciona, passo a passo

**1. Desenho inicial — uma vez só, antes do sorteio começar.**
`jogo.cpp:60-64` chama `exibir` cinco vezes, uma por cartela, cada uma com um par (x,y) diferente (`jogo.cpp:29-33`), para as 5 caberem lado a lado sem se sobrepor. Dentro de `exibir` (`exibicao.cpp:52-69`): `gotoxy(x,y)` posiciona o cursor e imprime `"Cartela: " << contador` (linha 55); `gotoxy(x,y+1)` imprime `"Jogador: " << nome[contador-1]` (linha 58) — `contador` é o número de 1 a 5 da cartela, usado ao mesmo tempo como rótulo e (menos 1) como índice do vetor de nomes. Depois, para cada linha `i` de 0 a 4, um único `gotoxy(x, y+3+i)` (linha 62) posiciona o início da linha, e o laço interno em `j` imprime `mat[i][j] << "\t"` (linha 66). Repare: `gotoxy` só é chamado no **início** de cada linha — o espaçamento horizontal entre os 5 números de uma linha é feito inteiramente pelo caractere de tabulação, não por conta nenhuma dentro de `exibir`.

**2. Cada rodada de sorteio** (laço em `jogo.cpp:70-140`):
- `sorteio()` devolve um número novo (`jogo.cpp:72` — não é desta parte).
- `mostrarSorteado(num, xAviso, yAviso)` (`jogo.cpp:74`) reescreve a linha de aviso: `gotoxy` no ponto, imprime 41 espaços para apagar o que estava escrito antes (`exibicao.cpp:17`), `gotoxy` de novo no **mesmo** ponto (porque imprimir os espaços moveu o cursor para a direita) e só então escreve `"Numero sorteado: " << num` (linhas 16-19).
- `exibirSorteados(numSorteados, quantidade, num, xSorteados, ySorteados)` (`jogo.cpp:75`, com `atual = num`) redesenha a lista inteira do zero: escreve o cabeçalho `"Numeros sorteados (N/75):"` (linhas 76-77) e depois, para cada `i` de 0 a `quantidade-1`, calcula sua própria posição com `coluna` e `linha` locais (`x + coluna*3`, `y + 1 + linha`, linha 81) — sem usar tabulação nenhuma — decide a cor (vermelho se `lista[i] == atual`, verde caso contrário, linhas 88-91) e quebra para uma nova linha de tela a cada 25 números (linhas 96-101).
- `pintarNumero` é chamado 5 vezes, uma por cartela, todas em `RED` (`jogo.cpp:78-82`). Dentro dele (`exibicao.cpp:22-50`), duas variáveis de laço (`i` linha, `j` coluna) percorrem a matriz enquanto uma posição `posicaoX` (iniciada em `x`, linha 26) refaz a mesma conta que o `\t` de `exibir` faria: soma 1 se o número da célula tem 1 dígito, ou 2 se tem 2 (linhas 42-45), e arredonda para o próximo múltiplo de 8 (linha 47) — porque é isso que uma tabulação faz num terminal. Quando `mat[i][j] == num` (linha 31), o código pula direto pra lá com `gotoxy(posicaoX, y+3+i)` (linha 33), pinta o fundo, reimprime só aquele número, volta o fundo para `BLACK` e **retorna na hora** (linha 39) — nunca continua procurando depois de achar. Se o número sorteado não estiver naquela cartela, os dois laços terminam sem pintar nada; não é tratado como erro.
- O jogo espera o Enter (`jogo.cpp:85` — não é desta parte).
- `pintarNumero` é chamado de novo, dessa vez em `GREEN` (`jogo.cpp:88-92`), repintando a mesma célula — troca o destaque de "acabou de sair" (vermelho) para "já saiu" (verde).
- `exibirSorteados` é chamada outra vez (`jogo.cpp:94`), agora com `atual = 0` — como o sorteio nunca produz 0 (`sorteio.cpp:43`, `rand() % 75 + 1`), nenhum item bate com `atual` e a lista inteira sai verde, espelhando o que `pintarNumero` acabou de fazer nas cartelas.
- Conferir quem ganhou é `vitoria.cpp` (`cartelaCompleta`, chamada em `jogo.cpp:98` etc.) — fora desta parte.

## Por que foi feito assim

- **`\033[...]` (ANSI) em vez de `<windows.h>`**: `gotoxy`/`textbackground`/`ansiBG` (`tela.cpp`) são a reimplementação do `codefun_GDB.h` que o próprio professor entregou (confirmado em `fatos-verificados.md`, comparação byte a byte dos valores de cor). A alternativa — `SetConsoleCursorPosition`/`SetConsoleTextAttribute` do `<windows.h>` — prende o programa ao Windows e ninguém do grupo saberia explicar a API na defesa; o ANSI, além de mais simples, é literalmente material da disciplina.
- **`exibir` usa `\t` em vez de calcular a coluna de cada uma das 25 células**: menos conta dentro de `exibir` (um `cout` só por linha), mas o preço é que `pintarNumero` precisa **refazer** essa mesma conta de tabulação sozinho (`exibicao.cpp:42-47`) para saber aonde apontar o `gotoxy`. Foi uma escolha consciente de "desenhar simples, relocalizar complicado" — o ponto fraco disso está listado em Limites conhecidos.
- **`cor` como parâmetro de `pintarNumero`, em vez de duas funções (uma vermelha, uma verde)**: a mesma busca é repetida com cores diferentes duas vezes por rodada (`jogo.cpp:78-82` e `88-92`); um parâmetro evita duplicar a função inteira e é a aplicação direta da exigência "sub-rotinas e passagem de parâmetros" (`enunciado.txt:70`).
- **`exibirSorteados` redesenha a lista inteira a cada chamada, em vez de só acrescentar o número novo**: uma versão "só acrescenta" teria que lembrar, de uma chamada para a outra, em que coluna/linha parou — e como a passagem é por valor (`exibicao.h:16`, sem `&`), essa memória teria que morar em alguma variável que sobrevive entre chamadas, ou seja, uma global — proibida pelo enunciado (`enunciado.txt:72`). Recalcular tudo a partir de `lista[]` e `quantidade` (no máximo 75 números) custa pouco e não depende de nada guardado fora da função.
- **Ordenar por inserção (`inserirEmOrdem`, `sorteio.cpp:23-35`) em vez de ordenar dentro de `exibirSorteados`**: quem ordena é o sorteio, não a exibição — `exibirSorteados` só confia na ordem que recebe (`exibicao.cpp:79`). Inserir 1 número novo numa lista de até 74 já ordenados é mais barato que reordenar os 75 a cada rodada, e evita que este módulo precise de um segundo algoritmo de ordenação além do `bubblesort` já usado em `cartela.cpp:11-64`.
- **Quebra de 25 em 25 na lista de sorteados** (`exibicao.cpp:96-101`): cada número ocupa 3 colunas de tela (linha 81); 75 números × 3 = 225 colunas, mais do que os 120 do terminal usado pelo jogo (medido em `fatos-verificados.md`, `mode con: cols=120 lines=30`). 25 × 3 = 75 cabe numa linha só a partir de `xSorteados = 2` (`jogo.cpp:34`).

## Requisitos do enunciado que esta parte atende

| Req | Frase do enunciado | Como esta parte atende |
|---|---|---|
| R13 | `enunciado.txt:49` "Cada cartela terá um número de identificação e um nome" | `exibir` mostra `"Cartela: " << contador` (`exibicao.cpp:55`); quem atribui o número (1 a 5) é `jogo.cpp:60-64`, esta parte só exibe. |
| R14 | `enunciado.txt:49` + `enunciado.txt:66` | `exibir` mostra `"Jogador: " << nome[contador-1]` (`exibicao.cpp:58`), atrelando o nome digitado à cartela certa. |
| R17 | `enunciado.txt:52` "Exibir na tela as cartelas geradas durante toda a execução." | `exibir` roda uma vez por cartela antes do sorteio (`jogo.cpp:60-64`) e nunca mais é chamada nem apagada dentro do laço de sorteio (`jogo.cpp:70-140`); nada limpa a tela depois disso. |
| R18 | `enunciado.txt:53` "Marcar os valores já sorteados nas cartelas com cores diferenciadas" | `pintarNumero` em `GREEN` depois do Enter (`jogo.cpp:88-92`, cor definida em `tela.h:11`). |
| R22 | `enunciado.txt:57` "devem ser exibidos na tela em ordem crescente" | `exibirSorteados` imprime `lista[0..quantidade-1]` na ordem em que já chegam (`exibicao.cpp:79-93`); quem garante a ordem é `inserirEmOrdem` (`sorteio.cpp:23-35`), não esta parte. |
| R23 | `enunciado.txt:57-58` "destacando o atual valor sorteado com uma cor diferente" | `pintarNumero` em `RED` na primeira passada (`jogo.cpp:78-82`) e `exibirSorteados` pintando `lista[i] == atual` em `RED` (`exibicao.cpp:88-89`). |
| R24 | `enunciado.txt:60-61` "os valores sorteados também devem ser exibidos na tela durante todo o sorteio" | `exibirSorteados` é chamada duas vezes por rodada (`jogo.cpp:75` e `94`), sempre redesenhando a lista inteira acumulada. |
| R27 | `enunciado.txt:63-64` "deve-se destacar o valor sorteado na cartela" | `pintarNumero` localizando e pintando a célula certa dentro de cada cartela (`exibicao.cpp:31-39`). |
| R30 | `enunciado.txt:70` "Utilize sub-rotinas e passagem de parâmetros." | As 4 funções só existem como sub-rotinas parametrizadas (matriz, vetor de nomes, cor, quantidade/atual como parâmetros); nenhuma depende de estado fora da função. |

## Perguntas de defesa sobre esta parte

1. **Como `pintarNumero` sabe em que coluna da tela um número está, se `exibir` só usa `\t` pra espaçar?**
   Ele refaz a mesma conta que a tabulação faz: soma 1 (número de 1 dígito) ou 2 (2 dígitos) à posição atual e arredonda pro próximo múltiplo de 8 (`exibicao.cpp:42-47`), porque um `\t` sempre pula pro próximo múltiplo de 8 no terminal.

2. **Por que `pintarNumero` recebe `cor` como parâmetro em vez de duas funções separadas (uma vermelha, uma verde)?**
   Porque ela é chamada duas vezes por rodada fazendo a mesma busca, só muda a cor (`RED` em `jogo.cpp:78-82`, `GREEN` em `jogo.cpp:88-92`); parametrizar evita duplicar a função inteira e é a exigência de "sub-rotina com passagem de parâmetro" do enunciado.

3. **Quem garante que a lista de sorteados aparece em ordem crescente — a exibição ou o sorteio?**
   O sorteio. `inserirEmOrdem` (`sorteio.cpp:23-35`) já insere cada número novo na posição certa de uma lista sempre ordenada; `exibirSorteados` só percorre e desenha o que recebe (`exibicao.cpp:79`), não ordena nada.

4. **Por que `mostrarSorteado` escreve 41 espaços antes de escrever o número de novo?**
   Para apagar o texto anterior. Se o sorteio anterior foi "75" (2 dígitos) e o de agora é "3" (1 dígito), escrever só `"Numero sorteado: 3"` por cima deixaria o "5" antigo sobrando na tela; os espaços (`exibicao.cpp:17`) limpam a linha inteira antes.

5. **Por que a lista de sorteados quebra de 25 em 25 números?**
   Cada número ocupa 3 colunas de tela (`exibicao.cpp:81`); 75 × 3 = 225 colunas, e o jogo roda num terminal de 120 colunas (medido em `fatos-verificados.md`). Quebrar a cada 25 (`exibicao.cpp:96-101`) mantém 75 colunas por linha, dentro do limite.

6. **`exibir` é chamada de novo a cada número sorteado, pra redesenhar a cartela?**
   Não. `exibir` roda uma única vez por cartela, antes do sorteio começar (`jogo.cpp:60-64`). Depois disso, quem atualiza a tela é `pintarNumero`, que pinta só a célula do número sorteado — é por isso que a cartela nunca "pisca" nem some durante o jogo (R17).

## Limites conhecidos

- **Duas contas de coluna independentes para o mesmo layout.** `exibir` posiciona os números por tabulação implícita (`exibicao.cpp:66`); `pintarNumero` recalcula a mesma posição na unha (`exibicao.cpp:42-47`). Se alguém mudar como `exibir` imprime (por exemplo, trocar o `\t` por um espaço fixo, ou mudar quantos dígitos cabem), `pintarNumero` desalinha e passa a pintar a célula errada — sem erro de compilação nem de execução, silenciosamente.
- **Nome de jogador sem limite de tamanho.** `jogadores.cpp:20` lê o nome inteiro com `getline`, sem checar tamanho, e `exibir` (`exibicao.cpp:58`) imprime esse nome inteiro depois de `"Jogador: "`. As cartelas 1 e 2 ficam em x=2 e x=42 (`jogo.cpp:29-30`) — um nome longo o bastante invadiria visualmente a cartela vizinha. Não rodei o jogo com um nome desse tamanho para confirmar o efeito exato na tela; **não conferi** na prática, só no código.
- **Número mágico "41 espaços" sem constante nomeada.** `exibicao.cpp:17` funciona porque 41 é bem maior que os até ~19 caracteres da mensagem mais longa, mas não há comentário nem constante explicando a conta — se a mensagem "Numero sorteado: " mudar de texto, o valor 41 pode ficar curto sem nenhum aviso.
- **`exibirSorteados` nunca foi testada com outro `x` inicial.** A quebra a cada 25 números só cabe na tela porque hoje `xSorteados` é sempre 2 (`jogo.cpp:34`); mudar essa constante sem refazer a conta de largura poderia estourar as 120 colunas do terminal.
