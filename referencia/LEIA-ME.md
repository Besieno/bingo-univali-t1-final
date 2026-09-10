# Material do professor (referência, não é código nosso)

Os dois arquivos aqui são **do professor**, copiados de `Desktop\Algoritmos e programação 2\`.
Estão neste repositório privado só como prova de origem — o jogo **não os inclui**,
porque a entrega tem que ser um arquivo único que compile sozinho.

| Arquivo | O que tem dentro |
|---|---|
| `codefun.h` | versão Windows: `gotoxy` por `SetConsoleCursorPosition`, `clrscr()` por `system("cls")` |
| `codefun_GDB.h` | versão ANSI: `gotoxy` monta `\033[linha;colunaH`, `textbackground` usa a tabela `ansiBG[16]` |

## Por que isso importa na defesa

A pergunta previsível é: *"de onde veio esse `\033[` que vocês usam?"*

Resposta com prova: **do `codefun_GDB.h` que o próprio professor entregou**. O `gotoxy`,
o `textbackground` e a tabela `ansiBG` do nosso `src/tela.cpp` são a mesma implementação
dele — não é comando de fora da disciplina, e não é código copiado da internet.

As constantes de cor também são as dele: `BLACK 0`, `BLUE 1`, `GREEN 2`, `AQUA 3`, **`RED 4`**.
Nosso `src/tela.h` usa esses valores. (A versão do João, no outro repositório, define
`RED 1` — que no padrão do professor é azul; funciona por acaso porque ele também
simplificou o `ansiBG` para `40 + cor`.)
