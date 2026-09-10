# Bingo T1 — o que mudou e por quê

**Base:** o `main.cpp` do João (a versão de 09/09, 339 linhas).
**Resultado:** `main.cpp` nesta pasta, 422 linhas. O original **não foi alterado** — continua em `Downloads\main (5).cpp`.

Compila com `g++ -std=c++17 -Wall -Wextra` sem **nenhum** aviso (antes tinha 1).

---

## Resumo em uma linha

Três coisas que o enunciado pede não estavam feitas, e quatro coisas quebravam na hora de rodar. Estão todas resolvidas. O núcleo do jogo (geração das cartelas, faixas por linha, ordenação, pintura, condição de vitória) já estava certo e **não foi mexido**.

---

## Parte 1 — Requisitos do enunciado que faltavam

### 1.1 Lista dos números sorteados em ordem crescente

O enunciado pede isso duas vezes:

> "Serão sorteados valores de 1 a 75, sendo que os valores não devem ser repetidos e **devem ser exibidos na tela em ordem crescente**, destacando o atual valor sorteado com uma cor diferente."

> "os valores sorteados **também devem ser exibidos na tela durante todo o sorteio**."

A versão antiga mostrava só o número da vez (`Numero sorteado: 42`) e ainda tentava apagá-lo logo em seguida. Não existia lista nenhuma.

**O que foi feito:** duas funções novas.

- `inserirEmOrdem()` — `main.cpp:195`. Em vez de reordenar o vetor inteiro a cada sorteio, empurra pra direita quem for maior e encaixa o número novo no buraco. É ordenação por inserção, o método que está no PDF *Métodos de Pesquisa e ordenação*.
- `exibirSorteados()` — `main.cpp:128`. Desenha a lista em 3 linhas de 25 números, com o número da vez em **vermelho** e os anteriores em **verde**. Números de um dígito ganham um espaço na frente pra ficarem alinhados com os de dois.

O vetor `numSorteados` agora fica sempre ordenado, então `verificarSorteado()` continua funcionando igual — não precisou de um segundo vetor.

### 1.2 Número da cartela vencedora

> "Ao final, deve ser exibido o nome do jogador **e o número da cartela vencedora**."

Antes saía só `Ana ganhou!`. Agora sai `Ana ganhou com a cartela 3!` — `main.cpp:336-360`.

### 1.3 Identificação dos autores no código-fonte

Vale **10% da nota** ("Identificação dos autores e comentários pertinentes"). Os nomes existiam só na tela `Sobre`, e o arquivo começava direto no `#include`. Foi adicionado o cabeçalho em comentário no topo — `main.cpp:1-14`.

### 1.4 Mês errado no Sobre

O enunciado pede "o mês/ano" na tela Sobre. Estava escrito **"Maio de 2026"**, mas a entrega é 11/09/2026. Corrigido para **Setembro de 2026** — `main.cpp:377` e no cabeçalho (`main.cpp:6`).

---

## Parte 2 — Bugs que quebravam na hora de rodar

Todos foram reproduzidos rodando o programa de verdade, não são teoria.

### 2.1 O menu travava com qualquer letra (era o mais grave)

`lerOpcao` fazia `cin >> opcao` e nunca chamava `cin.clear()`. Quando o usuário digita uma letra, o `cin` entra em estado de erro, **não consome o caractere errado**, e toda leitura seguinte falha na hora.

**Medido:** digitando `a` no menu, o programa cuspiu **154 MB de "Valor inválido" e 2.807.365 repetições em 5 segundos**, com a CPU no talo. Só morria com Ctrl+C.

É o primeiro teste que um professor faz num menu.

**Correção** — `main.cpp:30-48`:
- `cin.clear()` tira o `cin` do estado de erro
- `cin.ignore(1000, '\n')` joga fora o que não era número
- um caso a mais pra `cin.eof()` (Ctrl+Z), que também travava

**Depois da correção:** digitando `a`, `b`, `99`, `3` → três avisos e sai limpo.

### 2.2 Nome com espaço roubava a vez do jogador seguinte

`cin >> nome[i]` para no primeiro espaço. Testado: digitando `Joao Vitor` para o jogador 1, saía **Cartela 1 = "Joao" e Cartela 2 = "Vitor"** — e o jogador 5 nunca era perguntado.

Detalhe irônico: os nomes da própria equipe (`João Vitor Silva da Cruz`) quebram essa leitura.

**Correção** — `main.cpp:57-63`: `getline(cin, nome[i])` lê a linha inteira, e agora pergunta um jogador por vez, dizendo de qual cartela é. Exigiu `#include <string>`.

### 2.3 A tela mostrava um número que não tinha sido sorteado

`apagarSorteado()` fazia `cout << ""` — string vazia não apaga caractere nenhum.

**Testado:** sorteia 75, depois sorteia 4 → a tela mostrava **`Numero sorteado: 45`**.

**Correção** — `main.cpp:78-83`: `mostrarSorteado` agora escreve espaços por cima da linha antes de escrever o número novo. A função `apagarSorteado` foi removida: além de não funcionar, apagar o número contraria o enunciado, que manda os valores sorteados ficarem visíveis o tempo todo.

### 2.4 Duas cartelas ficavam fora da tela

As cartelas 2 e 5 começavam na coluna 90 e o "Numero sorteado" ia pra linha 36. Medindo a saída real: o tabuleiro antigo precisava de **122 colunas × 39 linhas**. O `cmd` padrão é 80×25 e o Windows Terminal abre em 120×30 — ou seja, na máquina do professor as cartelas da direita cortavam e **o número sorteado nunca aparecia**.

**Correção:** layout refeito para 3 cartelas em cima (x = 2, 42, 82) e 2 embaixo, com a lista de sorteados abaixo. Medido de novo: agora usa **114 colunas × 26 linhas**. Cabe em 120×30 com folga.

As coordenadas foram todas juntadas no começo de `jogar()` (`main.cpp:268-274`) em vez de ficarem espalhadas em três lugares diferentes — mexer no layout agora é mexer em um lugar só.

Também foi adicionado `system("mode con: cols=120 lines=30 > nul 2>&1")` em `main.cpp:414`, que abre a janela no tamanho certo quando o jogo roda no `cmd`. O `> nul` é pra não piscar mensagem de erro em terminal que não aceita o comando.

### 2.5 O jogo parecia travado depois de digitar os nomes

`cin.ignore()` **sem argumentos descarta 1 caractere só**, não a linha inteira. Por isso o código antigo tinha `cin.ignore(); cin.ignore();` duplicado em vários pontos: o primeiro comia o `\n` que sobrou e o segundo é que realmente esperava o Enter — sem nenhuma mensagem na tela dizendo isso.

E se alguém digitasse `ok` antes do Enter, sobravam caracteres no buffer e **dois sorteios passavam sozinhos** sem o jogador ver.

**Correção:** todos os `cin.ignore()` viraram `cin.ignore(1000, '\n')`, que descarta a linha toda. Os pares duplicados viraram chamada única, e foi adicionada a mensagem `"Pressione Enter para comecar o sorteio..."` (`main.cpp:303-305`) pra ninguém achar que travou.

---

## Parte 3 — Cores no padrão do professor

O `codefun.h` e o `codefun_GDB.h` que o professor entregou definem as cores assim: `BLACK 0`, `BLUE 1`, `GREEN 2`, `AQUA 3`, **`RED 4`**...

O código antigo definia **`RED 1`** — que no padrão do professor é **azul**. Funcionava por acaso, porque o `ansiBG` também tinha sido reescrito numa versão simplificada (`40 + cor`) que casa com a ordem crua do ANSI. Mas eram dois erros que se cancelavam, e a pergunta na defesa seria imediata: *"por que RED vale 1 aqui e 4 no meu header?"*

**Correção:**
- constantes no valor do professor — `main.cpp:26-28`
- `ansiBG()` passou a usar a tabela do `codefun_GDB.h` — `main.cpp:65-68`

Conferido rodando: o fluxo emite `\033[41m` (vermelho) e `\033[42m` (verde), e **nunca** `\033[44m` (azul).

> **Vale saber na defesa:** as funções `gotoxy`, `textbackground` e `ansiBG` vieram do `codefun_GDB.h` do professor. É material dele, não é cópia de internet — mas todo mundo tem que saber dizer isso.

---

## Parte 4 — Passagem por parâmetro e limpeza

"Recursos da linguagem utilizados" vale **20% da nota**, e *Subrotinas e Passagem de Parâmetros* é um PDF inteiro da disciplina. O código antigo não usava referência (`&`) em lugar nenhum.

### 4.1 Passagem por referência onde faz sentido

- `void lerOpcao(int &opcao)` — `main.cpp:30`. Antes era `int lerOpcao(int opcao)`: recebia um parâmetro que nunca usava e devolvia por retorno. Agora escreve direto na variável de quem chamou.
- `void inserirEmOrdem(int lista[], int &quantidade, int valor)` — `main.cpp:195`. O contador sobe para quem chamou.
- `int sorteio(int numSorteados[], int &quantidade)` — `main.cpp:250`. Sorteia, guarda e já atualiza a quantidade.

### 4.2 `quantidade` e `numSorteados` saíram do `main`

Antes desciam por valor de `main` → `chamarMenu` → `jogar`, então o `quantidade++` nunca voltava pro `main`. Não dava bug (cada partida recomeçava do zero, que era o desejado), mas ficava parecendo descuido — um contador que não volta ao lado de um vetor que volta.

Como são estado de **uma partida**, agora são declarados dentro de `jogar()` (`main.cpp:264-265`). Isso tirou 3 parâmetros de `jogar`, 3 de `chamarMenu` e 3 declarações do `main`.

### 4.3 `opcao` sem inicializar

`main` declarava `int opcao;` e passava sem valor. Era o único aviso do compilador (`-Wuninitialized`). Agora `opcao` é variável local de `chamarMenu`, inicializada em 0 — `main.cpp:385`.

### 4.4 Nomes misturando português e inglês

- `verifyNum` → **`numeroRepetidoNaLinha`**
- `verifyCartela` → **`cartelasIguais`**

Ficaram em inglês só `gotoxy`, `textbackground` e `ansiBG` (são do header do professor) e `bubblesort` (é o nome do algoritmo).

### 4.5 Comentários que descreviam outra coisa

- **Linha 90 do original:** o comentário dizia *"Mostra o nome do jogador associado à cartela"* mas a linha imprimia `"Cartela: "`. Estava descrevendo a linha de baixo.
- **Linhas 132-138 do original:** os três comentários de `verificarSorteado` eram **cópia** dos de `verifyNum` — falavam em *"posições anteriores da linha atual"* e *"percorre somente as colunas"*, mas a função varre o vetor de números sorteados, não uma linha da cartela.

Os dois foram reescritos pra dizer o que a função realmente faz. Isso cai direto no critério de 10% ("comentários pertinentes e oportunos").

### 4.6 `chamarMenu` virou `void`

Retornava `int` sempre 0 e ninguém lia o retorno.

---

## O que NÃO foi mexido (de propósito)

Foi tudo conferido e está certo:

- **Faixas por linha** (linha 1 = 1-15, linha 2 = 16-30...). Confere com o enunciado ao pé da letra. Não é o bingo clássico por coluna, e está correto do jeito que está.
- **Vitória = cartela cheia.** "A primeira pessoa que completar uma cartela é a vencedora." Certo.
- **`cartelasIguais`.** Como cada linha é ordenada, duas cartelas com os mesmos números são obrigatoriamente matrizes idênticas. A checagem está logicamente correta.
- **A conta de tabulação do `pintarNumero`** (`((posicaoX / 8) + 1) * 8`). Parece frágil, mas está certa: a saída foi renderizada num terminal virtual e cada cor cai exatamente em cima do seu número, em qualquer coluna inicial.
- **`bubblesort`, `gerarCartela`, `cartelaCompleta`, `exibir`, `gotoxy`** — sem alteração de lógica.
- **Não há estouro de vetor.** Instrumentei o código pra medir: o maior índice escrito em `numSorteados` é **71**, num vetor de 75.

**Também não foi feita** a refatoração pra matriz 3D (`int cartelas[5][5][5]`), que apagaria umas 50 linhas repetidas. Motivo: **matriz tridimensional não aparece em nenhum PDF da disciplina**, e a defesa é individual — os cinco precisam saber explicar o trabalho inteiro. O ganho de organização não compensa alguém travar explicando array de três dimensões.

---

## Testes que foram rodados

| Teste | Antes | Depois |
|---|---|---|
| Digitar letra no menu | trava, 2.807.365 repetições em 5 s | 3 avisos, sai limpo |
| Ctrl+Z (fim de entrada) | trava | encerra o programa |
| Nome composto ("Joao Vitor") | vira 2 jogadores | 1 jogador, nome inteiro |
| Partida completa | ok | ok, vencedor com nº da cartela |
| Duas partidas + Sobre + Sair | ok | ok, encerra com código 0 |
| Tamanho de tela necessário | 122 × 39 | 114 × 26 |
| Avisos do compilador (`-Wall -Wextra`) | 1 | 0 |

---

## Três coisas pra vocês decidirem

1. **`getline` e `cin.clear()` foram vistos em aula?**
   Não achei os dois nos PDFs que o professor disponibilizou. São comandos elementares e o próprio `codefun_GDB.h` dele já usa `cin.ignore(...)` com dois argumentos e `<limits>` — então tem base. Mas vale confirmar com ele. Se ele não aceitar `getline`, a alternativa é voltar pro `cin >>` e avisar na tela "digite só o primeiro nome" — perde a robustez, mas não trava.

2. **Acentuação nas mensagens.**
   O arquivo está em UTF-8 e as mensagens têm acento (`Valor inválido`). No terminal do VS Code aparece certo; num `cmd` antigo pode virar `invÃ¡lido`. O outro arquivo do grupo (`bingo.cpp`) escolheu escrever tudo sem acento justamente por isso. Não mexi porque é decisão de vocês — se quiserem, é só tirar os acentos das strings.

3. **Nome muito comprido na cartela 3.**
   A cartela 3 começa na coluna 82. Um nome com mais de ~30 caracteres passa da coluna 120 e quebra a linha. Não é bug, mas se quiserem blindar, dá pra limitar o nome exibido.

---

## Como testar

Abra a **pasta** no VS Code, abra `main.cpp` e aperte `Ctrl+Alt+N`.

Testes que vale fazer antes de entregar:
1. No menu, digitar `a` e depois `9` — tem que reclamar e continuar funcionando.
2. Digitar um nome com sobrenome — tem que ficar inteiro na cartela.
3. Jogar até alguém ganhar — tem que aparecer o nome **e** o número da cartela.
4. Voltar ao menu e jogar de novo — as cartelas têm que ser diferentes.
5. Conferir que a lista de sorteados cresce em ordem crescente e que o número da vez fica vermelho.
