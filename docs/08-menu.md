# Menu — o laço que decide se o jogo continua rodando ou termina, e a única porta de saída do programa

## O que esta parte resolve

O enunciado exige três coisas específicas do menu: exatamente três opções (Jogar/Sobre/Sair,
`enunciado.txt:20-23`), que o programa só encerre pela opção Sair (`enunciado.txt:28-29`), e que
qualquer outra opção volte ao menu depois de terminar (`enunciado.txt:29-31`). `menu.h`/`menu.cpp`
são inteiramente isso: não geram cartela, não sorteiam número, não desenham tabuleiro — só leem o
que o jogador digitou, validam, e despacham para `jogar()` ou `sobre()`.

`chamarMenu` é chamada uma única vez, por `main.cpp:35`, e é a função que o programa nunca larga:
enquanto ela está rodando, o processo está vivo; quando ela retorna (só depois de `opcao == 3`), o
`main` executa o `return 0` (`main.cpp:37`) e o programa acaba. Por isso o menu não é "mais uma
tela" — é a espinha dorsal que faz o resto do jogo (`jogar`, `sobre`) parecer uma sequência de
sub-rotinas chamadas e devolvidas, em vez de um script linear que roda uma vez e fecha.

## Arquivos e funções

| Função | Arquivo:linha | O que faz em uma linha |
|---|---|---|
| `lerOpcao` (declaração) | `menu.h:12` | Assinatura: recebe a opção por referência (`int &opcao`) |
| `lerOpcao` (implementação) | `menu.cpp:15-40` | Lê a opção digitada e só devolve quando ela for um número válido entre 1 e 3 (ou a entrada tiver acabado) |
| `sobre` (declaração) | `menu.h:13` | Assinatura: sem parâmetros, sem retorno |
| `sobre` (implementação) | `menu.cpp:42-50` | Mostra equipe, professor/disciplina e mês/ano; espera Enter para voltar |
| `chamarMenu` (declaração) | `menu.h:14` | Assinatura: recebe as 5 matrizes de cartela e o vetor de nomes por parâmetro |
| `chamarMenu` (implementação) | `menu.cpp:52-79` | Laço do menu: imprime as opções, chama `lerOpcao`, despacha `jogar`/`sobre`/sai |

## Como funciona, passo a passo

1. `main.cpp:35` chama `chamarMenu(cartela1, cartela2, cartela3, cartela4, cartela5, nome)`, passando
   as 5 matrizes 5×5 declaradas em `main.cpp:32` (ainda sem nenhum número sorteado) e o vetor de
   `string` `nome[TAM]`. `chamarMenu` não aloca nada — só recebe o espaço que o `main` reservou e o
   repassa adiante para `jogar()` quando for a vez.

2. Dentro de `chamarMenu`, a variável local `opcao` nasce zerada (`menu.cpp:54`) e o corpo roda dentro
   de um `do-while` (`menu.cpp:56-78`) cuja condição de parada é `opcao != 3` (linha 78) — ou seja, o
   laço só para quando a opção escolhida for exatamente Sair. A cada volta: limpa a tela (58), imprime
   as três opções (59-63), chama `lerOpcao(opcao)` (64) e usa um `switch` para decidir o que fazer
   (65-77).

3. `lerOpcao` (`menu.cpp:15-40`) tem seu próprio laço, separado do laço do menu: ele só devolve o
   controle quando a variável referenciada `opcao` (recebida por `&`, `menu.h:12`) contiver 1, 2 ou 3.
   - `cin >> opcao` (linha 19) tenta ler um inteiro.
   - Se a leitura falhar — o usuário digitou letra ou símbolo — `cin.fail()` vira `true` (21). Dentro
     desse bloco, se além de falhar a entrada tiver **acabado** (`cin.eof()`, linha 23 — acontece com
     entrada redirecionada de arquivo ou Ctrl+Z no teclado), a função força `opcao = 3` e retorna na
     hora (24-27): trata isso como se o jogador tivesse escolhido Sair, porque insistir em ler de um
     `cin` que não tem mais nada geraria um laço que nunca teria como progredir.
   - Se falhou mas **não** é fim de entrada, `cin.clear()` (28) tira o `cin` do estado de erro — sem
     isso, toda leitura seguinte falharia do mesmo jeito, sempre — e `opcao = 0` (29) força a repetição
     do laço, já que 0 está fora do intervalo válido.
   - `cin.ignore(1000, '\n')` (32) roda **sempre**, tenha a leitura falhado ou não: descarta o resto da
     linha digitada, inclusive o caractere de Enter. Isso é o que garante que a próxima leitura comece
     limpa — seja o próximo `cin >> opcao` da repetição, seja o `getline` de `nomearCartelas`
     (`jogadores.cpp:20`) quando o jogador escolhe Jogar.
   - Só depois disso o laço confere se `opcao` está fora de 1-3 (34-38) para decidir se repete a
     pergunta, e a condição do `do-while` (39) fecha o ciclo.

4. De volta em `chamarMenu`, o `switch(opcao)` (65-77) despacha:
   - **1** → chama `jogar(mat1..mat5, nome)` (68), passando as 5 matrizes e o vetor de nomes por
     parâmetro de array. Quando `jogar` retorna — a partida terminou porque alguém fez bingo ou os 75
     números acabaram (`jogo.cpp:140`) — a execução simplesmente volta para o topo do `do-while` do
     menu, e a tela de opções aparece de novo. Não existe nenhuma chamada extra nem `goto`: é a
     estrutura do laço, e só ela, que implementa "volta ao menu ao final da execução" (R05).
   - **2** → chama `sobre()` (71), que imprime as informações e trava esperando Enter
     (`menu.cpp:49`) antes de devolver o controle ao mesmo laço.
   - **3** → imprime a despedida (74-75) e não chama mais nenhuma função (76). A função `chamarMenu`
     não tem `return` explícito aqui — ela simplesmente chega ao fim do `switch`, a condição do
     `do-while` (78) já é falsa (`opcao == 3`), o laço termina, `chamarMenu` retorna para `main.cpp:35`,
     e a linha seguinte (`main.cpp:37`, `return 0`) é o que de fato encerra o processo.

## Por que foi feito assim

- **Checar `cin.fail()` e `cin.eof()`, em vez de confiar direto em `cin >> opcao`**: não checar é
  exatamente o bug medido na versão anterior do trabalho — digitar uma letra deixa o `cin` em estado de
  erro permanente, `opcao` nunca muda de valor, e o `do-while` gira para sempre. Isso foi reproduzido
  rodando o executável antigo: mais de 4 MB de "Valor inválido" cuspidos em 4 segundos, sem parar
  (`docs/99-o-que-foi-medido.md`, seção "Bugs reproduzidos rodando"). `cin.clear()` é a única forma de tirar o
  fluxo desse estado; sem ele a checagem de intervalo (`menu.cpp:34`) nunca teria uma nova leitura para
  avaliar.

- **`cin.ignore(1000, '\n')` com um limite explícito, em vez de `cin.ignore()` sem argumento**: a versão
  sem argumento descarta só **um** caractere. Se sobrar mais de um caractere de lixo antes do Enter (por
  exemplo, o usuário digitar "ab" e depois Enter), um caractere continua no buffer e é consumido pela
  leitura seguinte — o mesmo tipo de falha catalogado no relatório de auditoria para a versão anterior
  do grupo (no repositório do rascunho esse item é o J3 de `falta/01-sorteio/`). Passar `1000` como limite garante que a linha inteira até o `\n`
  (ou até 1000 caracteres, o que vier primeiro) seja descartada de uma vez.

- **Tratar `cin.eof()` como "sair" em vez de deixar o laço tentar de novo**: sem essa saída, uma entrada
  que termina no meio (arquivo redirecionado que acaba, ou Ctrl+Z no teclado) faz `cin >> opcao` falhar
  para sempre, e `cin.clear()` sozinho não resolve — não há mais nada para ler. Forçar `opcao = 3`
  fecha o programa de forma controlada em vez de girar num laço sem chance de progredir.

- **`lerOpcao(int &opcao)` por referência, em vez de `int lerOpcao()` com `return`**: o valor
  problemático (uma leitura inválida) precisa ser corrigido dentro do mesmo laço que a função já está
  controlando, sem precisar de uma segunda variável nem de um `while` externo repetindo a atribuição.
  Além disso, é a demonstração direta do que o enunciado pede em "sub-rotinas e passagem de parâmetros"
  (`enunciado.txt:70`): a função recebe o endereço da variável do chamador e escreve nela diretamente,
  sem retorno e sem variável global.

- **O menu ser um `do-while` estrutural, e não recursão ou um `if` dentro de outro laço maior**: repetir
  a exibição do menu chamando a própria `chamarMenu` de novo a cada partida encerrada empilharia uma
  chamada de função por partida jogada — numa sessão longa isso cresce sem necessidade e complica
  explicar o fluxo. Um laço `do-while` expressa "repete até a condição de saída" sem empilhar nada:
  cada partida é uma chamada normal de `jogar()` que sempre retorna para o mesmo ponto.

- **`"\033c"` para limpar a tela, e não `system("cls")`**: o resto do projeto já fala em códigos de
  escape ANSI o tempo todo — `gotoxy` (`tela.cpp:15`) e `textbackground` (`tela.cpp:28,30`) escrevem
  sequências de escape direto no `cout` — e usar o mesmo mecanismo evita misturar duas formas diferentes
  de controlar o terminal dentro do mesmo programa. `"\033c"` é um reset completo do terminal (ESC + `c`,
  "Reset to Initial State"), diferente do `clrscr()` que o material do professor define em
  `referencia/codefun_GDB.h:37-40` (que usa `"\033[2J\033[H"` — limpa o conteúdo da tela e reposiciona o
  cursor, mas não reseta atributos de cor). Isso importa aqui porque o jogo troca a cor de fundo o tempo
  inteiro durante o sorteio: `jogo.cpp:78-92` chama `pintarNumero(..., RED)` e depois
  `pintarNumero(..., GREEN)` a cada número sorteado, e é `pintarNumero` quem liga a cor de fundo de
  fato (`textbackground(cor)`, `exibicao.cpp:35,37`); um reset completo evita que uma cor de fundo antiga sobre quando a tela
  do menu ou do "Sobre" aparece na sequência. A alternativa mais simples, `system("cls")`, também
  funcionaria, mas dependeria do sistema operacional (não existe em Linux) e trocaria de mecanismo no
  meio do código sem necessidade.

## Requisitos do enunciado que esta parte atende

- **R03** — "O jogo deverá possuir um menu onde será possível escolher: • Jogar • Sobre • Sair"
  (`enunciado.txt:20-23`) → as três opções impressas em `menu.cpp:60-62` e validadas para o intervalo
  1-3 em `lerOpcao` (`menu.cpp:34`).
- **R04** — "seu jogo só deve ser encerrado ao selecionar essa opção" (`enunciado.txt:28-29`) →
  `menu.cpp:73-76` (case 3) não chama nenhuma outra função; o `return 0` de `main.cpp:37` só é
  alcançado depois que `chamarMenu` retorna, o que só acontece quando o `do-while` (`menu.cpp:78`)
  encontra `opcao == 3`.
- **R05** — "caso qualquer outra opção seja escolhida ela deve retornar ao menu no fim de sua execução"
  (`enunciado.txt:29-31`) → estrutura do `do-while` (`menu.cpp:56-78`): tanto `jogar()` (case 1,
  linha 68) quanto `sobre()` (case 2, linha 71) são chamadas normais que retornam para o mesmo laço,
  não fecham nem saem do programa.
- **R06** — "deverão ser exibidas a equipe de desenvolvimento (o nome de cada membro da equipe)"
  (`enunciado.txt:34-36`) → `menu.cpp:45`, os 5 nomes da equipe.
- **R07** — "o mês/ano" (`enunciado.txt:36`) → `menu.cpp:47`, `"Setembro de 2026."`.
- **R08** — "e o nome do professor/disciplina" (`enunciado.txt:36`) → `menu.cpp:46`, nome do
  professor e da disciplina.
- **R30** (contribuição desta parte, não exclusiva dela) — "Utilize sub-rotinas e passagem de
  parâmetros" (`enunciado.txt:70`) → `lerOpcao(int &opcao)` por referência (`menu.h:12`) e
  `chamarMenu` recebendo as 5 matrizes e o vetor de nomes por parâmetro de array (`menu.h:14`),
  repassando tudo para `jogar` sem depender de variável compartilhada fora de escopo.
- **R32** (contribuição desta parte) — "Não utilize variáveis globais" (`enunciado.txt:72`) →
  `opcao` é local a `chamarMenu` (`menu.cpp:54`); nada em `menu.cpp`/`menu.h` existe fora do corpo de
  uma função (o único `#define` do projeto que este arquivo enxerga é `TAM`, em `comum.h:23`, que é
  macro de pré-processador — vira texto substituído na compilação, não uma variável em tempo de
  execução).

## Perguntas de defesa sobre esta parte

1. **O que acontece se eu digitar uma letra no menu?**
   `cin.fail()` detecta que não veio número (`menu.cpp:21`); como não é fim de entrada,
   `cin.clear()` tira o `cin` do estado de erro (28) e `opcao` volta a 0 (29), fazendo o `do-while`
   perguntar de novo. Sem isso o programa entraria num laço de erro permanente — é o bug reproduzido
   na versão anterior do trabalho (`docs/99-o-que-foi-medido.md`).

2. **Por que o programa não fecha sozinho depois de uma partida?**
   `chamarMenu` é um `do-while` (`menu.cpp:56-78`) que só sai quando `opcao == 3`; `jogar()` e
   `sobre()` são chamadas normais dentro do `switch` (68, 71) e, quando retornam, a execução volta
   para o topo do laço — não existe `return` nem `exit` escondido em nenhum outro caso.

3. **Por que `lerOpcao` recebe `int &opcao` em vez de devolver o valor com `return int`?**
   Passagem por referência (`menu.h:12`): a função escreve direto na variável do chamador. Isso evita
   precisar de variável global (R32) para compartilhar o valor e é a demonstração pedida de "passagem
   de parâmetros" (R30) num caso em que o próprio valor pode precisar ser corrigido (a opção inválida
   vira 0) dentro da mesma chamada.

4. **O que faz o `cin.ignore(1000, '\n')` logo depois da leitura, e por que com esse número?**
   Descarta tudo que sobrou na linha digitada, inclusive o Enter (`menu.cpp:32`). O limite de 1000
   garante que a linha inteira seja descartada, não só um caractere; sem isso o `\n` (ou um caractere
   de lixo) ficaria na entrada e o próximo `getline()` — usado em `nomearCartelas` quando o jogador
   escolhe Jogar — leria uma linha vazia em vez de esperar o nome do jogador.

5. **Por que existe uma checagem separada para `cin.eof()`?**
   Cobre o caso de a entrada ter acabado de vez (arquivo redirecionado que termina, ou Ctrl+Z no
   teclado): sem essa saída, `cin >> opcao` falharia para sempre e o laço nunca teria como progredir
   sozinho. O código força `opcao = 3` e trata isso como Sair (`menu.cpp:23-27`).

6. **Por que `"\033c"` para limpar a tela, em vez de `system("cls")` ou o `clrscr()` do material do
   professor?**
   Consistência: o projeto inteiro já usa códigos de escape ANSI para cursor e cor
   (`tela.cpp:15,28,30`). `"\033c"` é um reset completo do terminal, feito para apagar também
   qualquer cor de fundo que tenha ficado ligada da tela anterior — a cada número sorteado
   (`jogo.cpp:78-92`) `pintarNumero` liga `textbackground` (`exibicao.cpp:35,37`). Não é idêntico ao `clrscr()` do professor
   (`referencia/codefun_GDB.h:38`, que usa `"\033[2J\033[H"` e não reseta cor), mas é a mesma família
   de recurso (escape ANSI), só um comando de reset mais abrangente.

## Limites conhecidos

- **A limpeza de tela não virou função em `tela.h`.** `gotoxy` e `textbackground` ganharam função
  própria (`tela.h:14,16`), mas `"\033c"` é repetido como string literal 3 vezes dentro de
  `menu.cpp` (44, 58, 74) e mais 2 vezes em outros arquivos (`jogo.cpp:37`, `jogadores.cpp:22`). Se um
  dia precisasse trocar o mecanismo de limpar tela, seriam 5 lugares para editar, não 1. Pesa um pouco
  em organização (R38), não em funcionamento.
- **`lerOpcao` não avisa nada quando `cin.eof()` acontece** (`menu.cpp:23-27`) — o programa volta ao
  comportamento de Sair em silêncio, sem nenhuma mensagem. Correto para entrada automatizada
  (arquivo/pipe), mas se perguntarem "e se a entrada acabar no meio de uma leitura?", a resposta é
  essa: fecha sem avisar por quê.
- **O `"\033c"` usado aqui não é o `clrscr()` que está em `referencia/codefun_GDB.h:37-40`** (esse usa
  `"\033[2J\033[H"`, que limpa o conteúdo mas não reseta atributos de cor). O projeto optou por uma
  sequência diferente e mais forte, de propósito — não é um recurso "de fora da disciplina" (é ANSI
  padrão, a mesma família dos outros escapes usados no material do professor), mas não é literalmente
  o mesmo comando; vale saber explicar a diferença se perguntarem por que não é idêntico.
- **`sobre()` e o menu dependem do terminal interpretar sequências ANSI** (`\033[`, `\033c`) para
  posicionar cursor, colorir e limpar a tela. A versão final compila e roda sem erro no ambiente
  testado (`docs/99-o-que-foi-medido.md`, seção "Compilação"), mas não conferi a saída visual num `cmd.exe`
  puro sem VT100 habilitado — vale testar nesse terminal específico antes da defesa, caso o professor
  use um.
