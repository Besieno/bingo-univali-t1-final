# Mapa do código — comece por aqui

Esta é a **versão do grupo de 10/09/2026**. O jogo inteiro cabe num arquivo de 468 linhas
(é o que vai ser entregue). Separado em 10 partes, nenhuma passa de 120 linhas.

> O que cada **função** faz, uma por arquivo: pasta [`funcoes/`](../funcoes/).
> Este mapa é o andar de cima: em que ordem as coisas acontecem e quem depende de quem.

---

## Quem chama quem

```
main()                                  src/main.cpp
 └── repete até a opção ser 3:
      chamarMenu()                       src/menu.cpp   ← desenha o menu e devolve a opção
       └── lerOpcao()                    src/menu.cpp   ← só devolve 1, 2 ou 3
      switch (opcao)
        1 → jogar()                      src/jogo.cpp   ← a partida inteira
        2 → sobre()                      src/menu.cpp
        3 → imprime a despedida e sai
```

Nesta versão o `switch` mora no `main`, e `chamarMenu` só pergunta. É diferente da versão
anterior, em que o menu era um laço que chamava tudo por dentro.

E dentro de `jogar()`, na ordem exata:

```
jogar()
 1. gerarCartela(mat1)                   src/cartela.cpp
 2. gerarCartela(mat2..mat5)             repetindo enquanto verifyCartela() achar igual
 3. nomearCartelas(nome)                 src/jogadores.cpp   ← pergunta os 5 nomes
 4. "Enter para continuar..."            src/jogo.cpp
 5. exibir(mat1..mat5)                   src/exibicao.cpp    ← desenha o tabuleiro
 6. repete até alguém ganhar:
      sorteio()                          src/sorteio.cpp     ← número novo, sem repetir
      quantidade++                       src/jogo.cpp        ← o contador sobe AQUI, fora da função
      mostrarSorteado()                  src/exibicao.cpp    ← ordena e imprime a lista toda,
                                                               com o número da vez em VERMELHO
      pintarNumero(... RED) × 5          src/exibicao.cpp    ← marca nas cartelas
      espera o Enter                     src/jogo.cpp
      pintarNumero(... GREEN) × 5        src/exibicao.cpp    ← vira verde: "já saiu"
      apagarSorteado()                   src/exibicao.cpp
      cartelaCompleta() × 5              src/vitoria.cpp     ← alguém ganhou?
```

## Quem depende de quem

```
main  →  menu
      →  jogo  →  cartela     →  ordenacao   (ordena a linha da cartela)
               →  jogadores
               →  sorteio
               →  exibicao    →  tela        (cursor e cor)
                              →  ordenacao   (ordena a lista de sorteados)
               →  vitoria     →  sorteio     (pergunta se um número já saiu)
               →  tela
```

`comum.h` (só o `#define TAM 5`) é incluído por quase todo mundo — é a única coisa que
todas as partes precisam saber.

**A seta nunca volta.** `cartela.cpp` não sabe que existe tela; `sorteio.cpp` não sabe que
existe cartela. Só `jogo.cpp` conhece todo mundo. É isso que faz cada arquivo caber na
cabeça sozinho.

---

## A parte nova: `ordenacao`

O `bubblesort` aparece **duas vezes, com o mesmo nome**:

```cpp
void bubblesort(int mat[][TAM], int linha);  // ordena uma LINHA de uma cartela
void bubblesort(int vet[], int n);           // ordena o VETOR dos sorteados
```

Isso é **sobrecarga de função** — e tem um PDF só sobre isso na disciplina
(*Sobrecarga de Função*). Quem escolhe qual das duas roda é o compilador, pelo tipo do
argumento. Vale ponto em "recursos da linguagem" (20% da nota), e é uma pergunta de defesa
fácil de responder: *"o nome é o mesmo porque a operação é a mesma; o que muda é o que ela
ordena."*

Por isso as duas ficam juntas em `src/ordenacao.*`, em vez de escondidas dentro de `cartela`
e de `exibicao`.

---

## Onde procurar cada coisa

| Se a pergunta é… | Abra |
|---|---|
| "por que a linha 3 só tem números de 31 a 45?" | `src/cartela.cpp` → `gerarCartela` |
| "como garantem que não repete número?" | `src/cartela.cpp` (na linha) e `src/sorteio.cpp` (na urna) |
| "por que a lista de sorteados sai em ordem?" | `src/exibicao.cpp` → `mostrarSorteado` chama o `bubblesort` de vetor |
| "como o número fica colorido dentro da cartela?" | `src/exibicao.cpp` → `pintarNumero` |
| "o que é esse `\033[`?" | `src/tela.cpp` → `gotoxy` (e `referencia/codefun_GDB.h`) |
| "quando o jogo acaba?" | `src/vitoria.cpp` + o `while` no fim de `src/jogo.cpp` |
| "onde está a sobrecarga de função?" | `src/ordenacao.h` |
| "quem decide o que o menu faz?" | `src/main.cpp` — o `switch` está lá |

---

## Os documentos

| Onde | O que tem |
|---|---|
| [`funcoes/`](../funcoes/) | **um arquivo por função**: o que faz, parâmetros, quem chama, pergunta de defesa |
| [`91-o-que-pode-ser-contestado.md`](91-o-que-pode-ser-contestado.md) | o que ainda falta ou é frágil **nesta** versão, com conserto e custo |
| [`98-requisitos-do-enunciado.md`](98-requisitos-do-enunciado.md) | o enunciado quebrado em requisitos numerados (R01…R43) |
| [`99-o-que-foi-medido.md`](99-o-que-foi-medido.md) | o que foi conferido rodando de verdade |
| [`versao-anterior-09-09/`](versao-anterior-09-09/) | os documentos da versão de 09/09 — ainda úteis para a defesa das funções que não mudaram |

---

## Três coisas que todo integrante precisa saber dizer

1. **Não há variável global.** O que está fora de função são `#define` — macro do
   pré-processador, texto trocado antes de compilar. Não existe em tempo de execução.
2. **Não há template.** Nenhum. É um dos "não pode" do enunciado. *(Sobrecarga ≠ template:
   sobrecarga são duas funções escritas à mão; template seria uma só, genérica.)*
3. **As sub-rotinas recebem parâmetro de duas formas aqui**: por valor (`int num`,
   `int quantidade`) e por array (`int mat[][TAM]`, `int vet[]`, que já chegam como endereço).
   Esta versão **não usa `&` em nenhuma assinatura** — é um ponto que o professor pode cutucar;
   a resposta está no `91-o-que-pode-ser-contestado.md`.
