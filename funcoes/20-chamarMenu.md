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

> **Corrigido em 10/09** (item C7): a variável era declarada como `int opcao;` e passada sem
> valor, o que gerava `'opcao' is used uninitialized`. Agora é `int opcao = 0;`
> (`src/menu.cpp:46`) e o compilador não reclama mais.
>
> Ficou de propósito a outra metade: `lerOpcao` **continua recebendo um parâmetro que nunca lê**
> (ela sobrescreve com `cin >> opcao` antes de qualquer leitura). Não gera aviso, mas é uma
> pergunta fácil na defesa — a resposta honesta é "o parâmetro é herança de uma versão antiga;
> hoje ele não serve para nada".
