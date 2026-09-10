# Bingo — Trabalho M1 (versão final)

**Algoritmos e Programação II (22817) — UNIVALI · Escola Politécnica**
Professor: Rafael Ballotin Martins · Entrega: **11/09/2026 até 08:00**

Equipe: Bernardo Sieno · Henrique Dorow · João Vitor Silva da Cruz · Julio Cesar Manabe Padilha · Nicolas do Vale Mezencio

> Este é o repositório da **versão que vai ser entregue**: a versão do grupo de **10/09/2026**
> (`main (6).cpp`), separada em partes. O rascunho — o código do João de 09/09 com a lista
> do que falta — vive em `bingo-univali-t1`. As versões anteriores estão em `historico/`.

---

## O que é cada pasta

```
src/          o jogo, um arquivo por parte  ← é aqui que se mexe
entrega/      o MESMO código, num arquivo só ← é isto que vai pro AVA
funcoes/      uma ficha por função: o que faz, parâmetros, quem chama, defesa
docs/         mapa do código, o que pode ser contestado, requisitos, o que foi medido
ferramentas/  gera entrega/bingo.cpp a partir de src/ e confere uma partida
historico/    as versões anteriores, com o que cada troca ganhou e perdeu
referencia/   codefun.h e codefun_GDB.h do professor (só como prova de origem)
enunciado.pdf / enunciado.txt   o enunciado original
```

### Por que dois formatos do mesmo código

O professor pede **um arquivo** (`.txt` ou `.cpp`). Mas um arquivo de quase 400 linhas é
difícil de ler, de dividir entre cinco pessoas e de estudar para a defesa.

Então o código vive em `src/`, uma parte por arquivo, e `entrega/bingo.cpp` é montado
a partir dele por script. **Não são duas versões**: é o mesmo código, e o script prova isso.

| | `src/` | `entrega/bingo.cpp` |
|---|---|---|
| Para quê | ler, estudar, mexer, dividir tarefa | postar no AVA |
| Quantos arquivos | 10 partes (`.h` + `.cpp`) | 1 |
| Quem edita | você | ninguém — é gerado |

---

## As 10 partes

| Parte | Arquivo | Responde a pergunta |
|---|---|---|
| Comum | `src/comum.h` | quanto mede a cartela? |
| Tela | `src/tela.*` | onde escrever e de que cor? |
| **Ordenação** | `src/ordenacao.*` | **as duas sobrecargas do `bubblesort`** |
| Cartela | `src/cartela.*` | como nasce uma cartela válida? |
| Sorteio | `src/sorteio.*` | qual o próximo número, e sem repetir? |
| Vitória | `src/vitoria.*` | essa cartela está completa? |
| Exibição | `src/exibicao.*` | como o tabuleiro e a lista aparecem na tela? |
| Jogadores | `src/jogadores.*` | quem é o dono de cada cartela? |
| Jogo | `src/jogo.*` | como a partida acontece do início ao fim? |
| Menu | `src/menu.*` | Jogar, Sobre ou Sair? |
| Main | `src/main.cpp` | a largada e o `switch` do menu |

Comece por `docs/00-mapa-do-codigo.md` (o fluxo) e por `funcoes/README.md` (o que cada
função faz, uma ficha por função).

---

## Como rodar

**No VS Code** (abra a **pasta**, não o arquivo): `Ctrl+Shift+B` → gera `bingo.exe` na raiz.
Para depurar: `F5`.

**Na linha de comando:**

```bat
g++ -std=c++17 -Wall -Wextra -static src/*.cpp -o bingo.exe
bingo.exe
```

Compila com **2 avisos** com `-Wall -Wextra` (g++ 16.1.0, MinGW-w64 UCRT) — os mesmos
do arquivo que o grupo mandou: `'linha' set but not used` e `'opcao' is used uninitialized`.
Os dois estão em `docs/91-o-que-pode-ser-contestado.md`, com o conserto.

> ⚠️ **Esta versão precisa de 122 colunas × 37 linhas** (medido) e **não** chama
> `system("mode con: ...")`. Num `cmd` de 80×25 ou num Windows Terminal de 120×30 as
> cartelas da direita cortam e a lista de sorteados fica fora da tela. Antes de apresentar,
> maximize a janela — ou aplique o conserto de layout descrito em
> `docs/91-o-que-pode-ser-contestado.md`.

---

## Como preparar a entrega

```bat
node ferramentas/gerar-entrega.mjs             REM gera entrega/bingo.cpp
node ferramentas/gerar-entrega.mjs verificar   REM avisa se src/ mudou depois disso
g++ -std=c++17 -Wall -Wextra entrega/bingo.cpp -o entrega/bingo.exe
```

Depois **rode o `entrega/bingo.exe`** e jogue uma partida inteira antes de postar.
O arquivo a postar é `entrega/bingo.cpp` — **um só**, na atividade "Trabalho T1".

### Conferência automática (opcional, mas é uma boa resposta na defesa)

`ferramentas/conferir-partida.mjs` lê a saída de uma partida e confere sozinho o que dá para
checar por texto: as 5 cartelas na tela, cada linha na faixa certa, linhas crescentes e sem
repetido, cartelas diferentes entre si, lista de sorteados crescente e sem repetir, e se o
vencedor anunciado tem mesmo os 25 números já sorteados.

```bash
printf '1\nAna\nBia\nCaio\nDuda\nEva\n' > entrada.txt
for i in $(seq 1 200); do printf '\n' >> entrada.txt; done
cat entrada.txt | ./bingo.exe > saida.txt
node ferramentas/conferir-partida.mjs saida.txt
```

Rodado assim 6 vezes nesta versão, o jogo passou nas 6 — 150 linhas de cartela conferidas.
(Prefira **pipe** a `programa < arquivo` — é o jeito que foi testado aqui.)

⚠️ **Cuidado ao testar por arquivo nesta versão:** se a entrada acabar sem um `3` (Sair),
o laço infinito do menu enche o disco (9,3 GB num teste). Sempre com teto: `| head -c 8000000`.

Checklist de 3 minutos antes de subir:
1. No menu, digite `a` — **hoje isso TRAVA o programa** (86 MB de "Valor inválido" em 2 s).
   Se o professor fizer esse teste, é o item C1 do `91-o-que-pode-ser-contestado.md`.
2. Digite um nome com sobrenome — tem que ficar inteiro na cartela.
3. Jogue até alguém ganhar — hoje sai só o nome do jogador, **sem o número da cartela** (o enunciado pede os dois).
4. Volte ao menu e jogue de novo — as cartelas têm que ser outras.
5. Abra o arquivo postado e confira que é ele mesmo (o enunciado diz que a conferência é do grupo).

---

## O que o professor cobra, e onde isso está

| Cobrança | Onde |
|---|---|
| Menu Jogar / Sobre / Sair, só sai no Sair | `src/menu.cpp` |
| 5 cartelas 5×5, faixa por linha, sem repetir, sem cartelas iguais, linhas crescentes | `src/cartela.cpp` |
| Nome do jogador por cartela | `src/jogadores.cpp` |
| Sorteio 1–75 sem repetir, exibido em ordem crescente e sempre visível | `src/sorteio.cpp` + `src/exibicao.cpp` |
| Cada sorteio só com ENTER | `src/jogo.cpp` |
| Cor diferente no número da vez e nos já marcados | `src/tela.cpp` + `src/exibicao.cpp` |
| Para no bingo, mostrando nome **e** número da cartela | `src/vitoria.cpp` + `src/jogo.cpp` — ⚠️ hoje sai só o nome |
| Sub-rotinas com passagem de parâmetros | 21 funções; por valor e por array. ⚠️ esta versão **não usa `&`** em nenhuma assinatura — ver item no `91` |
| Sem template, sem variável global | não há nenhum dos dois — só `#define`, que é macro de pré-processador. A **sobrecarga** de `bubblesort` não é template: são duas funções escritas à mão |
| Autores e comentários | cabeçalho em `src/comum.h`, `src/main.cpp` e no arquivo de entrega |

Nota: 50% funcionamento · 20% organização e clareza · 20% recursos da linguagem · 10% autores e comentários.

---

## Defesa — leia antes da aula

⚠️ **Sem defesa, a nota é ZERO**, e a nota do código depende do desempenho nela.
Cada integrante precisa saber explicar o trabalho **inteiro**, não só a sua parte.

- `funcoes/` — uma ficha por função, cada uma terminando com **uma pergunta de defesa e a resposta**.
- `docs/91-o-que-pode-ser-contestado.md` — os pontos fracos desta versão, com o conserto e o custo de cada um.
- `docs/versao-anterior-09-09/90-perguntas-de-defesa.md` — 29 perguntas com resposta. Foi escrito para a
  versão anterior, mas a maioria das respostas vale igual (as funções de cartela, sorteio, vitória e tela
  são as mesmas em espírito). Confira antes de decorar.

⚠️ **Cópia (de colega ou da internet) = ZERO sem recuperação.** Por isso os dois
repositórios são **privados**. Não torne público e não poste o código em lugar nenhum.
