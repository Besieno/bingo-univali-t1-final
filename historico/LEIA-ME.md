# Histórico

Três arquivos, em ordem cronológica. Nada aqui é compilado nem entregue — é registro.

| Arquivo | Data | O que é |
|---|---|---|
| `main-09-09-2026-arquivo-unico-original.cpp` | 09/09 | a versão anterior, 478 linhas. Tinha `inserirEmOrdem`, `getline` nos nomes e `lerOpcao` com `cin.clear()` — coisas que a versão atual não tem |
| `bingo-entrega-anterior-09-09.cpp` | 09/09 | o arquivo único que tinha sido gerado daquela versão |
| `main (6) - versao do grupo - 10-09-2026.cpp` | **10/09** | **a versão que está no ar agora**, como o grupo mandou: 398 linhas, arquivo único |

`MUDANCAS-da-versao-anterior.md` conta o que mudou da versão do João (09/09) para a de 09/09 —
é história da versão anterior, não da atual.

## Por que a troca

Em 10/09 o grupo mandou uma versão nova (`main (6).cpp`), que partiu da base do João e seguiu
por outro caminho. O Bernardo pediu para o repositório passar a ser essa versão. Ela **ganhou**
coisas que faltavam (a lista de sorteados na tela, o prompt do menu, um nome por jogador,
a sobrecarga de `bubblesort`) e **perdeu** correções que a de 09/09 tinha:

| | 09/09 | 10/09 (atual) |
|---|---|---|
| letra no menu | tratada (`cin.clear()`) | **laço infinito** — 86 MB em 2 s |
| nome com espaço | `getline`, nome inteiro | `cin >>`, corta no espaço |
| número da cartela vencedora | exibido | só o nome do jogador |
| lista de sorteados | tinha | tem (ordenada a cada rodada) |
| `RED` | 4 (padrão do professor) | 1 |
| avisos do compilador | 0 | 2 |

A lista completa, com conserto e custo de cada item, está em
`docs/91-o-que-pode-ser-contestado.md`. **Nada foi alterado no código** — a decisão de
aplicar é do grupo.

> Se a ideia for recuperar uma correção específica da versão antiga, ela está inteira em
> `main-09-09-2026-arquivo-unico-original.cpp` e explicada em `docs/versao-anterior-09-09/`.
