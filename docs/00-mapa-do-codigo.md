# Mapa do código — comece por aqui

O jogo tem 478 linhas. Divididas em 9 partes, nenhuma passa de 120.
Este mapa diz **onde cada coisa mora** e **em que ordem as coisas acontecem**.

---

## Quem chama quem

```
main()                                    src/main.cpp
 └── chamarMenu()                         src/menu.cpp
      ├── [1] jogar()                     src/jogo.cpp   ← a partida inteira
      ├── [2] sobre()                     src/menu.cpp
      └── [3] sai do laço
```

E dentro de `jogar()`, na ordem exata em que acontece:

```
jogar()
 1. gerarCartela(mat1)                    src/cartela.cpp
 2. gerarCartela(mat2..mat5)              repetindo enquanto cartelasIguais() disser que já existe
 3. nomearCartelas(nome)                  src/jogadores.cpp   ← pergunta os 5 nomes
 4. exibir(mat1..mat5)                    src/exibicao.cpp    ← desenha o tabuleiro
 5. espera o Enter                        src/jogo.cpp
 6. repete até alguém ganhar:
      sorteio()                           src/sorteio.cpp     ← número novo, sem repetir
      mostrarSorteado()                   src/exibicao.cpp
      exibirSorteados(atual = num)        src/exibicao.cpp    ← lista crescente, o da vez em VERMELHO
      pintarNumero(... RED) × 5           src/exibicao.cpp    ← marca nas cartelas
      espera o Enter                      src/jogo.cpp
      pintarNumero(... GREEN) × 5         src/exibicao.cpp    ← vira verde: "já saiu"
      exibirSorteados(atual = 0)          src/exibicao.cpp
      cartelaCompleta() × 5               src/vitoria.cpp     ← alguém ganhou?
```

## Quem depende de quem

```
main  →  menu  →  jogo  →  cartela      (gera as cartelas)
                        →  jogadores    (nomes)
                        →  sorteio      (números)
                        →  exibicao  →  tela   (cursor e cor)
                        →  vitoria   →  sorteio (pergunta se um número já saiu)
```

`comum.h` (só o `#define TAM 5`) é incluído por quase todo mundo — é a única coisa
que todas as partes precisam saber.

**A seta nunca volta.** `cartela.cpp` não sabe que existe tela; `sorteio.cpp` não sabe
que existe cartela. Só `jogo.cpp` conhece todo mundo. É isso que faz cada arquivo caber
na cabeça sozinho.

---

## Onde procurar cada coisa

| Se a pergunta é… | Abra |
|---|---|
| "por que a linha 3 só tem números de 31 a 45?" | `src/cartela.cpp` → `gerarCartela` |
| "como vocês garantem que não repete número?" | `src/cartela.cpp` (na linha) e `src/sorteio.cpp` (na urna) |
| "por que a lista sai ordenada?" | `src/sorteio.cpp` → `inserirEmOrdem` |
| "como o número fica colorido dentro da cartela?" | `src/exibicao.cpp` → `pintarNumero` |
| "o que é esse `\033[`?" | `src/tela.cpp` → `gotoxy` (e `referencia/codefun_GDB.h`) |
| "quando o jogo acaba?" | `src/vitoria.cpp` + o `while` no fim de `src/jogo.cpp` |
| "por que digitar letra no menu não trava?" | `src/menu.cpp` → `lerOpcao` |
| "por que 'João Vitor' não vira dois jogadores?" | `src/jogadores.cpp` → `getline` |

---

## Os documentos das partes

| # | Documento | Cobre |
|---|---|---|
| 01 | [`01-tela.md`](01-tela.md) | `gotoxy`, `ansiBG`, `textbackground` |
| 02 | [`02-cartela.md`](02-cartela.md) | `gerarCartela`, `bubblesort`, `numeroRepetidoNaLinha`, `cartelasIguais` |
| 03 | [`03-sorteio.md`](03-sorteio.md) | `sorteio`, `verificarSorteado`, `inserirEmOrdem` |
| 04 | [`04-vitoria.md`](04-vitoria.md) | `cartelaCompleta` |
| 05 | [`05-exibicao.md`](05-exibicao.md) | `exibir`, `pintarNumero`, `mostrarSorteado`, `exibirSorteados` |
| 06 | [`06-jogadores.md`](06-jogadores.md) | `nomearCartelas` |
| 07 | [`07-jogo.md`](07-jogo.md) | `jogar` |
| 08 | [`08-menu.md`](08-menu.md) | `lerOpcao`, `sobre`, `chamarMenu` |
| 09 | [`09-main.md`](09-main.md) | `main` |
| 90 | [`90-perguntas-de-defesa.md`](90-perguntas-de-defesa.md) | as perguntas prováveis, com resposta |
| 91 | [`91-o-que-pode-ser-contestado.md`](91-o-que-pode-ser-contestado.md) | os pontos fracos que sobraram |

---

## Três coisas que todo integrante precisa saber dizer

1. **Não há variável global.** O que está fora de função são `#define` — macro do
   pré-processador, texto trocado antes de compilar. Não existe em tempo de execução.
2. **Não há template.** Nenhum. É um dos "não pode" do enunciado.
3. **As sub-rotinas recebem parâmetro de três jeitos**: por valor (`int num`), por array
   (`int mat[][TAM]`, que já chega como endereço) e **por referência** (`int &quantidade`,
   em `lerOpcao`, `inserirEmOrdem` e `sorteio`) — que é o recurso que o enunciado
   pede explicitamente e vale 20% da nota.
