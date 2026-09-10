# Bingo — Trabalho M1 (versão final)

**Algoritmos e Programação II (22817) — UNIVALI · Escola Politécnica**
Professor: Rafael Ballotin Martins · Entrega: **11/09/2026 até 08:00**

Equipe: Bernardo Sieno · Henrique Dorow · João Vitor Silva da Cruz · Julio Cesar Manabe Padilha · Nicolas do Vale Mezencio

> Este é o repositório da **versão que vai ser entregue**. O rascunho — o código do
> João separado por partes, com a lista do que falta — vive em `bingo-univali-t1`.

---

## O que é cada pasta

```
src/          o jogo, um arquivo por parte  ← é aqui que se mexe
entrega/      o MESMO código, num arquivo só ← é isto que vai pro AVA
docs/         um documento por parte: o que faz, por quê, e as perguntas de defesa
ferramentas/  o script que gera entrega/bingo.cpp a partir de src/
referencia/   codefun.h e codefun_GDB.h do professor (só como prova de origem)
MUDANCAS.md   o que mudou da versão do João para esta, com o porquê de cada mudança
enunciado.pdf o enunciado original
```

### Por que dois formatos do mesmo código

O professor pede **um arquivo** (`.txt` ou `.cpp`). Mas 478 linhas num arquivo só é
difícil de ler, de dividir entre cinco pessoas e de estudar para a defesa.

Então o código vive em `src/`, uma parte por arquivo, e `entrega/bingo.cpp` é montado
a partir dele por script. **Não são duas versões**: é o mesmo código, e o script prova isso.

| | `src/` | `entrega/bingo.cpp` |
|---|---|---|
| Para quê | ler, estudar, mexer, dividir tarefa | postar no AVA |
| Quantos arquivos | 9 partes (`.h` + `.cpp`) | 1 |
| Quem edita | você | ninguém — é gerado |

---

## As 9 partes

| Parte | Arquivo | Responde a pergunta |
|---|---|---|
| Comum | `src/comum.h` | quanto mede a cartela? |
| Tela | `src/tela.*` | onde escrever e de que cor? |
| Cartela | `src/cartela.*` | como nasce uma cartela válida? |
| Sorteio | `src/sorteio.*` | qual o próximo número, e sem repetir? |
| Vitória | `src/vitoria.*` | essa cartela está completa? |
| Exibição | `src/exibicao.*` | como o tabuleiro aparece na tela? |
| Jogadores | `src/jogadores.*` | quem é o dono de cada cartela? |
| Jogo | `src/jogo.*` | como a partida acontece do início ao fim? |
| Menu | `src/menu.*` | Jogar, Sobre ou Sair? |
| Main | `src/main.cpp` | a largada |

Cada uma tem seu documento em `docs/`. Comece por `docs/00-mapa-do-codigo.md`.

---

## Como rodar

**No VS Code** (abra a **pasta**, não o arquivo): `Ctrl+Shift+B` → gera `bingo.exe` na raiz.
Para depurar: `F5`.

**Na linha de comando:**

```bat
g++ -std=c++17 -Wall -Wextra -static src/*.cpp -o bingo.exe
bingo.exe
```

Compila **sem nenhum aviso** com `-Wall -Wextra` (g++ 16.1.0, MinGW-w64 UCRT).

> O jogo precisa de uma janela de **120×30**. O `main` já tenta ajustar sozinho
> (`system("mode con: cols=120 lines=30")`), mas se a janela for menor as cartelas
> da direita cortam. Windows Terminal e o terminal do VS Code já abrem grandes o bastante.

---

## Como preparar a entrega

```bat
node ferramentas/gerar-entrega.mjs             REM gera entrega/bingo.cpp
node ferramentas/gerar-entrega.mjs verificar   REM avisa se src/ mudou depois disso
g++ -std=c++17 -Wall -Wextra entrega/bingo.cpp -o entrega/bingo.exe
```

Depois **rode o `entrega/bingo.exe`** e jogue uma partida inteira antes de postar.
O arquivo a postar é `entrega/bingo.cpp` — **um só**, na atividade "Trabalho T1".

Checklist de 3 minutos antes de subir:
1. No menu, digite `a` e depois `9` — tem que reclamar e continuar funcionando.
2. Digite um nome com sobrenome — tem que ficar inteiro na cartela.
3. Jogue até alguém ganhar — tem que sair o nome **e** o número da cartela.
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
| Para no bingo, mostrando nome **e** número da cartela | `src/vitoria.cpp` + `src/jogo.cpp` |
| Sub-rotinas com passagem de parâmetros | todas; passagem por referência em `lerOpcao`, `inserirEmOrdem` e `sorteio` |
| Sem template, sem variável global | não há nenhum dos dois — só `#define`, que é macro de pré-processador |
| Autores e comentários | cabeçalho em `src/comum.h`, `src/main.cpp` e no arquivo de entrega |

Nota: 50% funcionamento · 20% organização e clareza · 20% recursos da linguagem · 10% autores e comentários.

---

## Defesa — leia antes da aula

⚠️ **Sem defesa, a nota é ZERO**, e a nota do código depende do desempenho nela.
Cada integrante precisa saber explicar o trabalho **inteiro**, não só a sua parte.

- `docs/90-perguntas-de-defesa.md` — as perguntas prováveis, com a resposta pronta.
- `docs/91-o-que-pode-ser-contestado.md` — os pontos fracos que sobraram, com o conserto de cada um.
- Cada `docs/0X-*.md` termina com as perguntas específicas daquela parte.

⚠️ **Cópia (de colega ou da internet) = ZERO sem recuperação.** Por isso os dois
repositórios são **privados**. Não torne público e não poste o código em lugar nenhum.
