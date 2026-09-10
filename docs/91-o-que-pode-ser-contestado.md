# O que pode ser contestado — versão do grupo (10/09/2026)

> ## ✅ Seis destes já foram aplicados (10/09/2026) — o compilador agora está limpo
>
> **C1, C2, C3 e C4 estão consertados no código** — 17 linhas tocadas em 3 arquivos
> (`src/menu.cpp`, `src/jogadores.cpp`, `src/jogo.cpp`), mudando o mínimo possível.
> Medido depois da correção:
>
> | | Antes | Depois |
> |---|---|---|
> | letra no menu | 86 MB de "Valor inválido" em 2 s | 3 avisos e sai limpo (321 bytes) |
> | "Joao Vitor Silva da Cruz" | virava 2 jogadores, o 5º não era perguntado | fica inteiro na cartela 1 |
> | vitória | "A cartela do jogador X ganhou!" | "A cartela **1** do jogador X ganhou!" |
> | tela Sobre | "Maio de 2026" | "Setembro de 2026" |
>
> 6 partidas pelo `conferir-partida.mjs` depois da mudança: **6 passaram**, e o aviso de
> "o anúncio não diz o número da cartela" desapareceu.
>
> Numa segunda rodada foram aplicados também **C7 e C8**, os dois que geravam aviso de
> compilação: `int opcao = 0;` em `chamarMenu` (`src/menu.cpp:46`) e a remoção da variável
> `linha`, que era criada e incrementada sem nunca ser lida (`src/exibicao.cpp:18` e `:39`).
> **O programa agora compila com `-Wall -Wextra -Wpedantic` sem um único aviso**, e 3 partidas
> novas passaram no conferidor.
>
> Do C7 ficou a metade cosmética: `lerOpcao` **continua recebendo um parâmetro que nunca lê**.
> Isso não gera aviso nenhum, mas é uma pergunta fácil do professor ("por que essa função
> recebe algo que ela ignora?"). Tirar o parâmetro mexe em 3 lugares (`menu.h`, a definição e
> a chamada) — 3 minutos, se quiserem.
>
> **C5 (a janela de 122×37) continua aberto de propósito:** o caminho de 0 minuto é abrir o
> terminal maximizado; mexer nas coordenadas é o item mais arriscado da lista.


Esta é a lista honesta do que sobrou **nesta** versão: a que o grupo mandou em 10/09, hoje em `src/` e no arquivo único `entrega/bingo.cpp`. Cada item abaixo foi conferido abrindo o arquivo, e o que diz "medido" foi medido rodando o `.exe` compilado com `g++ 16.1.0`. **Nada foi alterado no código** — a separação em 10 partes não mexeu em lógica nenhuma (as 21 funções batem byte-a-byte com o arquivo do grupo), e nenhum conserto daqui foi aplicado. A decisão de aplicar é do grupo.

Quem aplicar precisa mexer nos **dois** lugares: em `src/<arquivo>.cpp` e regerar `entrega/bingo.cpp` (ou editar os dois à mão), senão o arquivo postado fica diferente do que vocês estudaram.

**Legenda de prioridade**
- 🔴 aplicar antes de entregar
- 🟡 se sobrar tempo
- ⚪ só saber responder na defesa

---

## C1 ✅ APLICADO (era 🔴) — Digitar uma letra no menu = laço infinito

**Onde** — `src/menu.cpp:11-19` (`lerOpcao`) / `entrega/bingo.cpp:397-405`. Chamada em `src/menu.cpp:40` / `entrega/bingo.cpp:426`.

**Por que incomoda** — O laço é `do { cin >> opcao; if (opcao < 1 or opcao > 3) cout << "Valor inválido..."; } while (opcao < 1 or opcao > 3);`. Não existe `cin.clear()` nem `cin.ignore()` em lugar nenhum dela. Quando `cin >> opcao` recebe uma letra, o `cin` entra em estado de erro e **não consome** o caractere errado: toda leitura seguinte falha na hora, sem ler nada, e a condição do `while` nunca muda. **Medido agora:** `printf 'a\n' | bingo.exe` cuspiu **86 MB de "Valor inválido" em 2 segundos**, sem parar — só morre com Ctrl+C. É o primeiro campo que qualquer avaliador testa, e um dedo errado no teclado durante a defesa trava o programa na frente do professor. Mina R40 (funcionamento correto = 50% da nota) e R04 (só "Sair" encerra o programa).

**Conserto** (~5 min) — dentro do `if` de opção inválida, em `src/menu.cpp:14-16`:

```cpp
if (opcao < 1 or opcao > 3) {
    cin.clear();              // tira o cin do estado de erro
    cin.ignore(1000, '\n');   // joga fora o que não era número
    cout << endl << "\tValor inválido, escolha uma das opções válidas: ";
}
```

**Resposta na defesa se ficar como está** — "O `cin >> opcao` só aceita número. Se entrar letra, o `cin` trava em estado de erro e o laço repete o aviso — a correção é `cin.clear()` mais `cin.ignore()`; nós identificamos, não chegou a entrar nesta versão." Dito assim é honestidade; dizer que "não acontece" é pior, porque o professor reproduz em 2 segundos.

---

## C2 ✅ APLICADO (era 🔴) — Nome com espaço é cortado e vaza para a cartela seguinte

**Onde** — `src/jogadores.cpp:13-19`, a leitura está na linha `16` (`cin >> nome[i];`) / `entrega/bingo.cpp:272-278`, leitura em `:275`.

**Por que incomoda** — `cin >>` para no primeiro espaço em branco. **Medido rodando:** digitando `Joao Vitor` para o 1º jogador, a tela saiu com **Cartela 1 = "Jogador: Joao" e Cartela 2 = "Jogador: Vitor"**, empurrando todos os nomes seguintes e deixando o 5º jogador sem ser perguntado de verdade. A ironia: três dos cinco nomes da própria equipe são compostos (`João Vitor Silva da Cruz`, `Julio Cesar Manabe Padilha`, `Nicolas do Vale Mezencio`) — se o professor digitar o nome dele, quebra. Fere R14 e R28, e contamina R29 (o vencedor anunciado pode ser meio nome ou o nome de outro).

**Conserto** (~10 min) — em `src/jogadores.cpp`, antes do `for`, descartar o Enter que sobrou do menu e passar a ler a linha inteira:

```cpp
cin.ignore(1000, '\n');            // limpa o \n que sobrou da leitura da opção
for (int i = 0; i < TAM; i++) {
    cout << "\n\tDigite o nome do " << i+1 << "º jogador: ";
    getline(cin, nome[i]);
}
```

`<string>` já está incluído em `src/jogadores.cpp:7`, não precisa de include novo.

**Resposta na defesa se ficar como está** — "`cin >>` lê até o espaço; para nome composto o certo é `getline`. Nós sabemos a causa e o conserto (um `cin.ignore` antes do laço e `getline` no lugar do `>>`)." E, na hora de demonstrar, digitem nomes de uma palavra — mas sem esconder o motivo se perguntarem.

---

## C3 ✅ APLICADO (era 🔴) — A mensagem de vitória não diz o número da cartela

**Onde** — `src/jogo.cpp:87, 93, 99, 105, 111` / `entrega/bingo.cpp:350, 356, 362, 368, 374`.

**Por que incomoda** — As cinco mensagens são iguais: `cout << endl << "A cartela do jogador " << nome[0] << " ganhou!";`. Nenhuma imprime o número da cartela (1 a 5). **Medido:** a saída real sai como "A cartela do jogador X ganhou!". O enunciado é literal em `enunciado.txt:66-67`: *"Ao final, deve ser exibido o nome do jogador **e o número da cartela vencedora**"* — é R29, e é o tipo de frase que o professor consegue cravar como "não atendido" sem discussão. Acontece em 100% das partidas, não depende do sorteio.

**Conserto** (~5 min) — incluir o número fixo de cada bloco, em `src/jogo.cpp:87` e nos outros quatro:

```cpp
cout << endl << "A cartela 1 do jogador " << nome[0] << " ganhou!";
```

e repetir com `2` (`:93`), `3` (`:99`), `4` (`:105`), `5` (`:111`) — o número é o mesmo que já é passado para `exibir(mat1, 1, ...)` em `src/jogo.cpp:45-49`.

**Resposta na defesa se ficar como está** — "O índice da cartela existe no código (é o `contador` que a `exibir` imprime como `Cartela: N`), mas na frase de vitória só imprimimos o nome. É uma linha de `cout` por bloco." Não há como defender como "atendido" — reconheçam.

---

## C4 ✅ APLICADO (era 🔴) — A tela "Sobre" diz "Maio de 2026"

**Onde** — `src/menu.cpp:25` / `entrega/bingo.cpp:411`. Contradiz o cabeçalho do próprio trabalho, `src/comum.h:6` / `entrega/bingo.cpp:6`, que diz "Setembro de 2026".

**Por que incomoda** — `cout << "\n\tMaio de 2026.\n";` numa das três opções do menu, que o professor vai abrir. R07 (`enunciado.txt:36`) pede o mês/ano, e a entrega é 11/09/2026. É sobra de uma versão anterior do trabalho: a versão de 09/09 já tinha isso corrigido (`historico/main-09-09-2026-arquivo-unico-original.cpp`), e a de 10/09 voltou para "Maio". Custa 1 palavra e dá impressão de código não revisado.

**Conserto** (~2 min) — em `src/menu.cpp:25`:

```cpp
cout << "\n\tSetembro de 2026.\n";
```

**Resposta na defesa se ficar como está** — "Ficou de uma versão anterior do arquivo; o mês certo é setembro, está no cabeçalho." Simples, mas é uma pergunta evitável por 2 minutos de trabalho.

---

## C5 🔴 — O tabuleiro precisa de 122 colunas × 37 linhas e nada ajusta a janela

**Onde** — `src/jogo.cpp:45-49` (as cartelas 2 e 5 começam em **x = 90**) / `entrega/bingo.cpp:308-312`; as mesmas colunas em `pintarNumero` (`src/jogo.cpp:62-66` e `:72-76` / `entrega/bingo.cpp:325-329` e `:335-339`); a conta de tabulação em `src/exibicao.cpp:72` / `entrega/bingo.cpp:247` (`posicaoX = ((posicaoX / 8) + 1) * 8;`); a lista de sorteados fixada na linha 36 por `gotoxy(0, 35)` em `src/exibicao.cpp:19` e `:46` / `entrega/bingo.cpp:194` e `:221`. Não existe **nenhuma** chamada `system(...)` no projeto (conferido por busca no arquivo inteiro).

**Por que incomoda** — **Medido renderizando a saída real:** o tabuleiro completo ocupa **122 colunas × 37 linhas**. O `cmd` padrão do Windows é 80×25 e o Windows Terminal abre em 120×30 — nenhum dos dois cabe. Na janela do professor, as cartelas 2 e 5 cortam à direita e a lista de sorteados (linha 36) cai fora da área visível, o que rola a tela e empurra as cartelas para fora. Isso quebra ao vivo R17 (`enunciado.txt:52`, cartelas visíveis durante toda a execução) e R24 (`enunciado.txt:60-61`, sorteados visíveis durante todo o sorteio) mesmo com a lógica 100% correta.

**Conserto** — três caminhos, em ordem de risco:

1. **0 min, zero código:** abrir o terminal **maximizado** (ou com a janela esticada) antes de rodar, e conferir na máquina da defesa que as 5 cartelas e a lista aparecem juntas. É o único caminho que não mexe em nada.
2. ~5 min, uma linha: no começo do `main`, `system("mode con: cols=130 lines=40 > nul 2>&1");`. Funciona, mas `system()` é um comando fora do material da disciplina — pode levantar R33. Se usarem, saibam explicar.
3. ~20 min, sem comando novo: reduzir os X das cartelas em `src/jogo.cpp:45-49` (e os mesmos valores nas 10 chamadas de `pintarNumero`) para, por exemplo, 2 / 40 / 78, e trazer a lista de sorteados de `gotoxy(0, 35)` para algo como `gotoxy(0, 28)`. Mexe em mais lugares: se fizerem, **rodem uma partida inteira depois** para ver se nada se sobrepõe.

**Resposta na defesa se ficar como está** — "O layout usa coordenadas absolutas; ele foi desenhado para janela grande e precisa de ~122 colunas. Em janela pequena as cartelas da direita cortam." E rodar maximizado. Não tente argumentar que cabe em 80×25: não cabe.

---

## C6 🟡 — A mensagem de vitória é posicionada só por `endl`, e `apagarSorteado` não apaga nada

**Onde** — `src/jogo.cpp:83` (`cout << endl << endl << endl << endl;` antes dos blocos de vitória) / `entrega/bingo.cpp:346`; `apagarSorteado` em `src/exibicao.cpp:45-48` / `entrega/bingo.cpp:220-223`, chamada em `src/jogo.cpp:78` / `entrega/bingo.cpp:341`.

**Por que incomoda** — Todo o resto do desenho usa `gotoxy` (posição absoluta), mas o bloco final de vitória depende de onde o cursor parou mais quatro `endl`. Como a lista de sorteados cresce (quebra de linha a cada 25 números em `src/exibicao.cpp:37-41`), no fim da partida o cursor já está perto do fim da tela: os `endl` empurram o console para cima justamente no momento em que o professor está olhando. E `apagarSorteado()` é `gotoxy(0, 35); cout << "";` — string vazia **não apaga caractere nenhum**; a função só reposiciona o cursor, apesar do comentário dizer que apaga a mensagem do número sorteado. Hoje isso não quebra nada (a lista só cresce e é reescrita por cima, e o enunciado quer ela visível mesmo), mas o nome e o comentário não correspondem ao comportamento — cai em R38 e R39.

**Conserto** (~5 min) — escolher uma das duas, não as duas:
- trocar `cout << endl << endl << endl << endl;` (`src/jogo.cpp:83`) por um `gotoxy(0, 38);`, para a mensagem de vitória ter lugar fixo; e
- corrigir o comentário de `apagarSorteado` para dizer o que ela faz (só reposiciona o cursor) **ou** remover a função e a chamada de `src/jogo.cpp:78`.

Não implementem uma limpeza "de verdade" na `apagarSorteado`: apagar a lista contraria R24.

**Resposta na defesa se ficar como está** — "A `apagarSorteado` é resíduo de uma versão em que só o último número aparecia; hoje ela não apaga nada e a lista fica na tela, que é o que o enunciado pede. A mensagem final usa `endl` em vez de `gotoxy` — funciona em janela grande."

---

## C7 ✅ APLICADO EM PARTE (era 🟡) — `lerOpcao` recebe um parâmetro que nunca é lido (aviso do compilador)

**Onde** — `src/menu.cpp:11` (`int lerOpcao(int opcao)`), chamada em `src/menu.cpp:38` (`int opcao;` sem valor) e `:40` (`opcao = lerOpcao(opcao);`) / `entrega/bingo.cpp:397`, `:424`, `:426`. Protótipo em `src/menu.h`.

**Por que incomoda** — **Medido:** compilando com `-Wall -Wextra` sai `warning: 'opcao' is used uninitialized [-Wuninitialized]` apontando a chamada. O parâmetro é decorativo: a primeira linha do laço (`src/menu.cpp:13`) já faz `cin >> opcao`, sobrescrevendo o que entrou. Numa disciplina em que "sub-rotinas e passagem de parâmetros" é requisito explícito (R30, `enunciado.txt:70`) e "recursos da linguagem" vale 20% (R41), a pergunta "para que serve esse parâmetro?" tem como resposta honesta "para nada".

**Conserto** (~3 min) — tirar o parâmetro:

```cpp
int lerOpcao() {
    int opcao;
    do { ... } while (opcao < 1 or opcao > 3);
    return opcao;
}
```

e em `src/menu.cpp:40` chamar `opcao = lerOpcao();`, atualizando o protótipo em `src/menu.h`. Atalho de 30 segundos, se quiserem só matar o aviso: `int opcao = 0;` em `src/menu.cpp:38`.

**Resposta na defesa se ficar como está** — "O valor devolvido é pelo `return`; o parâmetro de entrada não é usado — é um resto. O correto seria `int lerOpcao()`." Tenham essa frase pronta: é o aviso que aparece na compilação, então é provável que seja visto.

---

## C8 ✅ APLICADO (era 🟡) — Variável `linha` criada, incrementada e nunca usada (aviso do compilador)

**Onde** — `src/exibicao.cpp:18` (`int coluna=0, linha=0;`) e `:40` (`linha++;`) / `entrega/bingo.cpp:193` e `:215`.

**Por que incomoda** — **Medido:** `warning: variable 'linha' set but not used [-Wunused-but-set-variable=]`. Quem controla a quebra de linha da lista de sorteados é só `if (coluna == 25)`; `linha` nunca é lida em nenhuma expressão. É código morto, e é um dos dois avisos que o projeto emite — cai direto em R38 (organização e clareza, 20%).

**Conserto** (~2 min) — em `src/exibicao.cpp:18` deixar `int coluna = 0;` e apagar o `linha++;` da linha 40. Não muda comportamento nenhum (a variável é só escrita).

**Resposta na defesa se ficar como está** — "É uma variável que sobrou de uma ideia de contar as linhas impressas; não entra em cálculo nenhum e pode sair."

---

## C9 🟡 — `srand(time(NULL))` repete a partida se o `.exe` rodar duas vezes no mesmo segundo

**Onde** — `src/main.cpp:20` / `entrega/bingo.cpp:445`.

**Por que incomoda** — `time(NULL)` muda de valor uma vez por segundo. **Medido:** três execuções seguidas sem intervalo saíram idênticas (mesmas 5 cartelas, mesma ordem de sorteio); com 1 segundo entre elas, saíram diferentes. R12 (`enunciado.txt:48`) é exatamente "a cada nova execução deverá haver um novo sorteio com valores diferenciados" — e o teste natural do professor é dar dois cliques no `.exe` e comparar.

**Conserto** (~5 min) — somar uma fonte que muda dentro do mesmo segundo:

```cpp
srand(time(NULL) + clock());
```

`clock()` vem do `<time.h>` que já está incluído em `src/main.cpp:11` — não precisa de include novo.

**Resposta na defesa se ficar como está** — "A semente é o relógio em segundos; duas execuções dentro do mesmo segundo pegam a mesma semente. Somar `clock()` resolve." Se for demonstrar diferença entre execuções, espere 1 segundo entre elas.

---

## C10 🟡 — `or` no lugar de `||`, misturado com `&&` no mesmo arquivo

**Onde** — `or` em `src/jogo.cpp:32, 35, 38` e `src/menu.cpp:14, 17` / `entrega/bingo.cpp:295, 298, 301, 400, 403` (8 ocorrências somadas). `&&` nos mesmos papéis em `src/ordenacao.cpp:14` e `src/jogo.cpp:127` / `entrega/bingo.cpp:72` e `:390`.

**Por que incomoda** — `or` e `and` são sinônimos padrão de `||` e `&&` desde o C++98: **não é erro e não é biblioteca externa**. O problema é de defesa: quase nenhum material introdutório usa essa forma, e o mesmo arquivo usa os dois estilos para a mesma coisa — o professor pode ler como "comando não abordado na disciplina" (R33, `enunciado.txt:72`) ou como código vindo de fonte externa, que é um risco sério por causa de R43.

**Conserto** (~5 min) — trocar as 8 ocorrências de `or` por `||`. Comportamento idêntico, sintaxe reconhecível, e o arquivo passa a ter um único estilo.

**Resposta na defesa se ficar como está** — "`or` é palavra-chave do próprio C++, equivalente exata de `||` (está no padrão, não é macro nem biblioteca). Usamos as duas formas no arquivo; o certo seria padronizar." Se não forem trocar, **todos os cinco** precisam saber dizer isso.

---

## C11 ⚪ — `\033c` para limpar a tela não é o `clrscr()` do material do professor

**Onde** — `cout << "\033c";` em `src/jogadores.cpp:18`, `src/jogo.cpp:23`, `src/menu.cpp:22` e `:32`, `src/main.cpp:35` / `entrega/bingo.cpp:277, 286, 408, 418, 460`. O material do professor está em `referencia/codefun_GDB.h:37-40`: `void clrscr(){ cout << "\033[2J\033[H"; cout.flush(); }`.

**Por que incomoda** — `gotoxy`, `textbackground` e `ansiBG` (em `src/tela.cpp:13-32`) **são** cópia do header do professor, e isso é defensável: é material dele. Mas `\033c` (reset total do terminal) não aparece em lugar nenhum do `codefun_GDB.h`. Se a defesa for "isso veio do header do professor", a frase é verdadeira para três funções e falsa para a limpeza de tela.

**Conserto** (~5 min, opcional) — trocar as 5 ocorrências por `cout << "\033[2J\033[H";`, que é letra por letra o que o professor entregou e tem o mesmo efeito.

**Resposta na defesa se ficar como está** — "`\033[...` são códigos ANSI de terminal, a mesma família do `gotoxy` do `codefun_GDB.h`. O `\033c` reinicia o terminal; o do professor é `\033[2J\033[H`, que limpa e volta o cursor ao canto. Escolhemos o reset porque zera também as cores."

---

## C12 ⚪ — `RED` vale 1 aqui e 4 no header do professor

**Onde** — `#define BLACK 0`, `#define RED 1`, `#define GREEN 2` em `src/tela.h:9-11` / `entrega/bingo.cpp:35-37`, com a fórmula `ansiBG` em `src/tela.cpp:19-24` / `entrega/bingo.cpp:49-54` (`40 + color`).

**Por que incomoda** — No `codefun_GDB.h` do professor a paleta é a clássica do DOS/Borland: `BLACK 0, BLUE 1, GREEN 2, AQUA 3, RED 4`. Lá, o valor 1 é **azul**. O código do grupo usa a numeração crua do ANSI, onde `40 + 1 = 41` é vermelho de verdade — ou seja, **a cor na tela está certa** (medido: o número da vez sai em fundo vermelho). Só não é a numeração do material dele, e "por que `RED` vale 1 e no meu header vale 4?" é pergunta provável.

**Conserto** — nenhum necessário; a cor exibida está correta. Quem quiser eliminar o risco: `#define RED 4` mais uma tabela de tradução em `ansiBG` igual à do professor — mas isso mexe em cor, o que exige rodar uma partida inteira depois para conferir.

**Resposta na defesa se ficar como está** — "Nós usamos os números na ordem do ANSI, onde o código de fundo é 40 + cor: 40 preto, 41 vermelho, 42 verde. Por isso `RED` vale 1 aqui. O header do professor usa a paleta do DOS, em que vermelho é 4 e a tradução é feita por tabela. As duas pintam vermelho na tela — muda só a convenção do número."

---

## C13 ⚪ — O fundo verde da vitória nunca volta para preto

**Onde** — `textbackground(GREEN);` em `src/jogo.cpp:86, 92, 98, 104, 110, 116, 121` / `entrega/bingo.cpp:349, 355, 361, 367, 373, 379, 384`, nenhum seguido de `textbackground(BLACK);`. Compare com `pintarNumero` (`src/exibicao.cpp:59-61` / `entrega/bingo.cpp:234-236`), que sempre restaura.

**Por que incomoda** — Puramente cosmético: o fundo verde fica ligado até a próxima limpeza de tela (o `\033c` do menu resolve sozinho ao voltar). Na tela de vitória, qualquer texto extra sai com fundo verde sem necessidade.

**Conserto** (~3 min) — um `textbackground(BLACK);` depois do `cout` de cada bloco de vitória e dos dois de "Tem N ganhador(es)".

**Resposta na defesa se ficar como está** — "A cor de fundo do terminal é um estado: ligamos verde para a mensagem e não desligamos, porque a tela é limpa ao voltar ao menu. O certo é restaurar depois de escrever, como a `pintarNumero` faz."

---

## Se sobrar só uma hora antes da entrega

Ordem de aplicação, do que mais protege nota para o que menos protege. Os tempos são de edição mais conferência rápida; **os dois lugares** (o `.cpp` em `src/` e `entrega/bingo.cpp`) contam juntos em cada item.

| # | Item | O que muda | Minutos | Acumulado |
|---|---|---|---|---|
| 1 | **C4** | `"Maio"` → `"Setembro"` em `src/menu.cpp:25` | 2 | 2 |
| 2 | **C3** | número da cartela nas 5 mensagens de vitória (R29) | 5 | 7 |
| 3 | **C1** | `cin.clear()` + `cin.ignore()` em `lerOpcao` (trava do menu) | 5 | 12 |
| 4 | **C2** | `cin.ignore()` + `getline` nos nomes | 10 | 22 |
| 5 | **C5** | abrir o terminal maximizado e conferir a tela inteira (opção 1, sem código) | 0 | 22 |
| 6 | **C9** | `srand(time(NULL) + clock());` | 5 | 27 |
| 7 | **C7** | `lerOpcao()` sem parâmetro (mata 1 dos 2 avisos) | 3 | 30 |
| 8 | **C8** | remover a variável `linha` (mata o 2º aviso) | 2 | 32 |
| 9 | **C10** | `or` → `||` nas 8 ocorrências | 5 | 37 |
| 10 | **C6** | comentário da `apagarSorteado` + `gotoxy` antes da vitória | 5 | 42 |

**Total dos 🔴: 22 minutos.** Com os 🟡 da lista: **42 minutos**. Sobram ~18 minutos da hora para o passo que não pode faltar — **compilar e jogar uma partida inteira do começo ao fim** depois de mexer (`g++ -std=c++17 -Wall -Wextra entrega/bingo.cpp -o bingo.exe`): cinco nomes, Enters até alguém ganhar, e voltar ao menu. C11, C12 e C13 ficam de fora: são resposta de defesa, não conserto.

E o item que não custa minuto nenhum e vale 50% junto com a defesa: **todos os cinco** precisam saber explicar o arquivo inteiro, não só a parte que escreveram (R34, R42 — sem defesa, nota zero).

---

## O que esta versão GANHOU em relação à anterior

Sendo justo: a versão de 10/09 não é só regressão. Comparada com a base do grupo (o `main.cpp` do João, de onde ela partiu), o próprio grupo fechou requisitos que faltavam.

**1. A lista de números sorteados aparece na tela, em ordem crescente** — `src/exibicao.cpp:17-43` (`mostrarSorteado`) / `entrega/bingo.cpp:192-218`. **Medido agora:** a lista aparece de fato, em ordem crescente, com o número da vez em **fundo vermelho** (`src/exibicao.cpp:29-31`), acumulando e mostrando o contador `(N/75)`. A base mostrava só o número da vez e tentava apagá-lo em seguida. Isso fecha **R22** (sorteados exibidos em ordem crescente), **R23** (valor recém-sorteado em cor diferente) e **R24** (lista visível durante todo o sorteio) — três requisitos, e R22/R24 são cobrados duas vezes no enunciado (`enunciado.txt:56-61`).

**2. Sobrecarga de `bubblesort` — e é um recurso do PDF da disciplina** — `src/ordenacao.cpp:10` (`bubblesort(int mat[][TAM], int linha)`, chamada em `src/cartela.cpp:35`) e `src/ordenacao.cpp:29` (`bubblesort(int vet[], int n)`, chamada em `src/exibicao.cpp:20`) / `entrega/bingo.cpp:68` e `:87`, chamadas em `:130` e `:195`. Mesmo nome, duas assinaturas, **sem template** (R31 respeitado), e as duas são realmente usadas — não é código morto. "Sobrecarga de função" é tema de um PDF da própria disciplina: é o argumento mais forte que vocês têm para **R41** (recursos da linguagem, 20% da nota), e a resposta pronta para "por que existem dois `bubblesort`?". Como bônus, a segunda sobrecarga é o que mantém a lista de sorteados ordenada a cada rodada, ligando este ganho ao item 1.

**3. O menu pede a opção com um prompt de verdade** — `src/menu.cpp:39` (`cout << "\n\tEscolha uma opção: ";`) / `entrega/bingo.cpp:425`, junto das três opções em `src/menu.cpp:33-36`. O usuário não fica olhando uma tela que não pede nada. Sustenta **R03** (três opções exatas) e ajuda em **R38**.

**4. Um nome por jogador, perguntado um por um, com a cartela identificada** — `src/jogadores.cpp:13-19` pergunta "Digite o nome do Nº jogador" nos cinco; `src/exibicao.cpp:79` e `:82` imprimem `Cartela: N` e `Jogador: <nome>` juntos em cada cartela / `entrega/bingo.cpp:254` e `:257`. Isso fecha **R13** (cartela tem número de identificação), **R14** (cartela tem nome) e **R28** (solicitar o nome do jogador de cada cartela) — com a ressalva do C2: a leitura por `cin >>` quebra em nome composto. O requisito está atendido; a robustez da leitura é que não está.

Junto com isso, continuam de pé desta versão as partes que nunca foram problema e que o professor vai testar primeiro: faixa de valores por linha (R10), sem repetição na linha (R15), sem cartelas repetidas (R16, pelos `do/while` com `verifyCartela` em `src/jogo.cpp:27-38`), linhas em ordem crescente (R19, pelo `bubblesort` em `src/cartela.cpp:35`), sorteio 1-75 sem repetir (R20/R21, `src/sorteio.cpp:18-28`), um sorteio por Enter (R26, `src/jogo.cpp:69`) e o número destacado na cartela em vermelho e depois verde (R18/R27, `src/jogo.cpp:62-76`).
