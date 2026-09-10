# chamarMenu — desenha o menu principal e devolve a opção escolhida
**Onde:** `src/menu.cpp:31`  ·  **Declarada em:** `src/menu.h:13`  ·  **Módulo:** menu

## O que faz
Limpa a tela, imprime as 3 opções (Jogar/Sobre/Sair) e chama `lerOpcao` para
validar a escolha digitada. Só desenha e devolve — quem decide o que fazer com
a opção é o `main` (ver comentário em `menu.h:3-4`).

## Parâmetros
Nenhum.

## Devolve
`int` — a opção escolhida (1, 2 ou 3), já validada por `lerOpcao`.

## Quem chama
`main()`, dentro do `do...while` do loop principal, em `src/main.cpp:26`.

## Para a defesa
P: "A variável `opcao` que vocês passam pra `lerOpcao` já vem com algum valor?"
R: Não — é declarada em branco (`int opcao;`, linha 38) e passada assim mesmo; funciona porque `lerOpcao` sobrescreve com `cin` antes de usar.
⚠️ Por isso o `g++ -Wall -Wextra` acusa `'opcao' is used uninitialized` na linha 40 (declarada na 38) — inofensivo na prática, mas é um dos 2 avisos de compilação da versão nova.
