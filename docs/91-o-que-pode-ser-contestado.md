# O que ainda pode ser contestado — e o que responder

O jogo atende o enunciado. Isto aqui é a lista honesta do que **sobrou**: pontos que o
professor pode cutucar na defesa, ou que quebram se alguém usar o programa de um jeito
inesperado. Cada item tem a resposta pronta e o conserto, com o custo.

**Nada aqui foi alterado no código.** A decisão de aplicar ou não é do grupo — a lista
existe para que a escolha seja consciente, não para virar tarefa automática.

Legenda de prioridade: 🔴 aplicar antes de entregar · 🟡 aplicar se sobrar tempo · ⚪ só saber responder.

---

## 🔴 C1 — `or` e `and` misturados com `||` e `&&` no mesmo arquivo

**Onde:** `or`/`and` em `src/menu.cpp` (`lerOpcao`), `src/sorteio.cpp` (`inserirEmOrdem`)
e `src/jogo.cpp` (as cascatas de `cartelasIguais`); `&&` em `src/cartela.cpp` (`bubblesort`)
e no `while` final de `src/jogo.cpp`.

**Por que incomoda:** os dois são padrão do C++ e significam a mesma coisa — mas o arquivo
se contradiz sozinho, e a primeira pergunta vira "por que aqui é `and` e ali é `&&`?".
Não há resposta boa além de "foi escrito em momentos diferentes".

**Conserto:** troca mecânica de todos para `&&` e `||`. **5 min, risco zero.**

**Resposta se ficar como está:** "`and` e `or` são palavras-chave alternativas do C++, sinônimos
exatos de `&&` e `||`; o compilador trata igual."

---

## 🔴 C2 — Duas execuções no mesmo segundo geram o jogo idêntico

**Onde:** `src/main.cpp` → `srand(time(NULL))`.

**Por que incomoda:** o enunciado pede, com todas as letras, *"a cada nova execução deverá
haver um novo sorteio com valores diferenciados"*. `time(NULL)` conta em **segundos**: abrir
o programa duas vezes rápido dá a mesma semente e o mesmo jogo. É fácil do professor
reproduzir sem querer — basta fechar e abrir.

**Medido:** dez partidas rodadas em sequência, sem intervalo, saíram em **dois grupos de
jogos idênticos** (cinco e cinco) — cartelas iguais, mesma ordem de sorteio. Com 1 segundo
de intervalo entre execuções, seis partidas saíram todas diferentes. Não é teoria.

**Conserto:** `srand((unsigned int)time(NULL) + (unsigned int)clock());` — `clock()` já vem
de `<time.h>`, que o arquivo inclui. **1 min.**

**Resposta se ficar como está:** "a semente é o relógio em segundos; duas execuções no mesmo
segundo caem na mesma sequência."

---

## 🔴 C3 — Nome vazio ou muito longo estraga a tela final

**Onde:** `src/jogadores.cpp` → `getline(cin, nome[i])`; o efeito aparece em
`src/exibicao.cpp` (`exibir`) e na mensagem de vitória em `src/jogo.cpp`.

**Por que incomoda:** apertar Enter sem digitar deixa o nome vazio, e a mensagem final vira
`" ganhou com a cartela 1!"` — bem no requisito que pede nome **e** número da cartela.
Quanto ao tamanho: os nomes da equipe foram testados e cabem (o maior, "Joao Vitor Silva da
Cruz", fica inteiro na cartela 3). Um nome acima de ~29 letras é que passaria da coluna 120.

**Conserto:** repetir a pergunta enquanto o nome vier vazio, e cortar o que passar de 25 letras.
**5 min.** (`while (nome[i].empty())` em volta do `getline`.)

---

## 🟡 C4 — Enter em fila faz uma rodada passar sem pausa

**Onde:** `src/jogo.cpp` — o `cin.ignore(1000, '\n')` antes do laço e o de dentro dele.

**O que acontece:** o `cin.ignore` espera uma linha, mas não esvazia o que já estava no
buffer antes. Se o jogador apertar Enter duas vezes seguidas, a segunda linha fica guardada;
na rodada seguinte o `cin.ignore` não bloqueia, e o número é sorteado, pintado de vermelho e
**vira verde no mesmo instante**, sem ninguém pedir. O sorteio não some — mas o "cada sorteio
só ao apertar Enter" foi furado.

> **Honestidade sobre a prova:** isto vem da leitura do código, não de um teste reproduzido.
> Não dá para automatizar "apertar Enter duas vezes rápido" — quem quiser confirmar, é só
> abrir o jogo e martelar o Enter.

**Conserto com respaldo:** o próprio `codefun_GDB.h` do professor tem a função `pausar()`,
que esvazia o buffer antes de esperar:
```cpp
void pausar() {
    if (cin.rdbuf()->in_avail() > 0)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
```
Dá para usar a mesma ideia. **10 min** — mas exige `<limits>` e alguém do grupo saber
explicar `in_avail()`. Ver `referencia/codefun_GDB.h`.

---

## 🟡 C5 — Nada avisa que o jogo espera o próximo Enter

**Onde:** `src/jogo.cpp` — o aviso *"Pressione Enter para comecar o sorteio..."* aparece
**uma vez só**, antes da primeira rodada. Depois disso, `mostrarSorteado` escreve
"Numero sorteado: N" **em cima** dessa mesma linha e o aviso some.

**Por que incomoda:** quem não sabe do jogo olha a tela parada e acha que travou.

**Conserto:** uma linha antes do `cin.ignore` do laço, escrevendo o aviso numa linha abaixo
do número sorteado. **2 min.**

---

## 🟡 C6 — A janela precisa de 120×30 e o erro é escondido de propósito

**Onde:** `src/main.cpp` → `system("mode con: cols=120 lines=30 > nul 2>&1")`.

**Medido:** renderizando a saída real de uma partida inteira, com os nomes da equipe (os mais
longos que vão aparecer), o tabuleiro ocupa **115 colunas × 26 linhas** — cabe em 120×30 com
folga. O risco não é o tamanho: é o comando falhar. Num terminal que não aceita `mode`, o `> nul`
engole o erro e as cartelas da direita cortam sem nenhum aviso (um `cmd` padrão tem 80 colunas).

**Conserto:** testar o retorno do `system` e, se não for 0, pedir para o usuário redimensionar
a janela antes de continuar. **5 min.**

**Resposta na defesa:** "o layout está centralizado nas variáveis do começo de `jogar()`;
mudar de lugar é mexer num lugar só."

---

## 🟡 C7 — Cinco matrizes soltas em vez de um vetor de cartelas

**Onde:** `src/jogo.cpp` (`jogar` recebe `mat1..mat5` e repete o mesmo bloco 5 vezes em
quatro pontos diferentes), `src/menu.cpp` (`chamarMenu` repassa as 5) e `src/main.cpp`
(declara as 5).

**Por que incomoda:** é o item de "organização e clareza" (20% da nota) mais visível.
`int cartelas[5][TAM][TAM]` com um `for` cortaria umas 50 linhas repetidas — e usa só
array e `for`, nada fora do que foi visto em aula.

**Por que não foi feito:** a defesa é individual e os cinco precisam explicar o trabalho
inteiro; um array de três dimensões é mais uma coisa para alguém travar explicando.
**É uma decisão consciente, não um esquecimento** — e essa é a resposta na defesa.

---

## ⚪ C8 — "De onde veio esse `\033[`?"

**Onde:** `src/tela.cpp` (`gotoxy`, `textbackground`, `ansiBG`) e os `\033c` que limpam a tela.

**Resposta com prova:** veio do **`codefun_GDB.h` que o próprio professor entregou** — a
implementação é a mesma, e o arquivo está em `referencia/` para conferir. `\033[` é a
sequência de escape ANSI: `ESC` seguido de `linha;colunaH` move o cursor, e de um número
entre 40 e 47 troca a cor de fundo (`49` volta ao padrão).

**O que ainda vale testar:** rodar o `.exe` num `cmd.exe` puro antes da entrega. O Windows
Terminal interpreta ANSI por padrão; um `cmd` antigo pode imprimir os códigos como texto.
Se isso acontecer, o mínimo é trocar os `\033c` por `system("cls")`.

---

## ⚪ C9 — "Isso não é variável global?"

**Onde:** `#define TAM 5` em `src/comum.h` e `#define BLACK/GREEN/RED` em `src/tela.h`.

**Resposta:** não. `#define` é macro de pré-processador: o compilador troca o texto antes de
compilar, e não existe nenhuma variável em tempo de execução. Global seria `int total;`
escrito fora de qualquer função — não há nenhuma no projeto.

---

## ⚪ C10 — Duas fórmulas para o mesmo layout

**Onde:** `src/exibicao.cpp` — `exibir` desenha com `\t`, e `pintarNumero` refaz a conta da
tabulação (`posicaoX = ((posicaoX / 8) + 1) * 8`) para achar a mesma célula.

**Por que é frágil:** são dois lugares que precisam concordar. Se um mudar, a cor cai ao lado
do número, e em silêncio.

**Resposta:** "`exibir` usa tabulação, e o terminal para a cada 8 colunas; `pintarNumero`
reproduz essa conta para achar a coluna exata do número." Está correto hoje — foi conferido
rodando, com as cores caindo em cima dos números certos.

---

## ⚪ C11 — Número de um dígito fica com uma "mordida" sem cor

**Onde:** `src/exibicao.cpp` → `exibirSorteados`: o espaço de alinhamento é impresso **antes**
de ligar a cor de fundo, então ele sai sem cor.

**Efeito:** puramente visual — na lista, os números de 1 a 9 têm um pedaço apagado à esquerda.

**Conserto:** ligar a cor primeiro, imprimir o espaço depois. **2 min.**

---

## ⚪ C12 — "41 espaços" é número mágico

**Onde:** `src/exibicao.cpp` → `mostrarSorteado` limpa a linha escrevendo 41 espaços.

**Resposta:** cobre `"Numero sorteado: NN"` com folga. Se quiser blindar, vira uma constante
com nome. **1 min.**

---

## ⚪ C13 — "Por que o código de vocês é parecido com o do João?"

**A pergunta vai vir**, porque o repositório do rascunho tem o arquivo dele e as funções são
parentes.

**Resposta:** é o mesmo trabalho do mesmo grupo. O João escreveu a base — cartelas por faixa,
sorteio sem repetir, pintura, vitória — e esta versão é a evolução dela: os nomes das funções
foram corrigidos (`verifyNum` → `numeroRepetidoNaLinha`), os comentários que descreviam outra
coisa foram reescritos, a leitura dos nomes passou para `getline`, o layout foi refeito para
caber na tela e foi acrescentada a lista ordenada dos sorteados, que faltava.

**Ação prática:** postar **um único arquivo** (`entrega/bingo.cpp`) e conferir depois do upload
que foi o certo — o enunciado diz que essa conferência é responsabilidade do grupo.

---

## Se sobrar só uma hora antes da entrega

1. **C1** — padronizar `&&`/`||` (5 min)
2. **C2** — semente com `clock()` (1 min)
3. **C3** — nome vazio (5 min)
4. **C5** — aviso do Enter (2 min)
5. **C8** — testar num `cmd.exe` de verdade (10 min)

Os quatro primeiros somam 13 minutos e tiram da mesa os pontos mais fáceis de perder.
