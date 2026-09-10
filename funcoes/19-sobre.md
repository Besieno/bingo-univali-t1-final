# sobre — mostra a tela de créditos (equipe, professor, mês/ano)
**Onde:** `src/menu.cpp:21`  ·  **Declarada em:** `src/menu.h:12`  ·  **Módulo:** menu

## O que faz
Limpa a tela, imprime os 5 nomes da equipe, o nome do professor e da disciplina
e o mês/ano do trabalho. No fim espera o Enter do usuário antes de voltar ao menu.

## Parâmetros
Nenhum.

## Devolve
`void` — nada; a função só imprime na tela e bloqueia esperando Enter.

## Quem chama
`main()`, no `case 2` do menu, em `src/main.cpp:32`.

## Para a defesa
P: "Por que tem dois `cin.ignore()` seguidos no final?"
R: O primeiro descarta o `\n` deixado no buffer pela leitura anterior do menu; o segundo é o que de fato prende a tela esperando o Enter do usuário.
