# Perguntas de defesa — com a resposta pronta

A defesa é **obrigatória** e vale a nota inteira: sem ela, o trabalho é ZERO. E cada
integrante precisa saber explicar o trabalho **todo**, não só a parte que escreveu.

Use esta lista como treino: leia a pergunta, responda em voz alta, **depois** confira.
Se a resposta não sair sozinha, abra o documento da parte (`docs/0X-*.md`).

---

## As 5 perguntas que vão cair com certeza

**1. Como vocês garantem que a linha 3 da cartela só tem números de 31 a 45?**
> `gerarCartela` calcula os limites a partir do número da linha: `limiteInicial = 15*i + 1`
> e `limiteFinal = 15*(i+1)`. Para `i = 2` isso dá 31 e 45. O `rand()` sorteia dentro
> desse intervalo. (`src/cartela.cpp`)

**2. E se sortear um número que já está na linha?**
> `numeroRepetidoNaLinha` varre só as colunas **já preenchidas** daquela linha; enquanto
> ela devolver `true`, o `do-while` sorteia outro. (`src/cartela.cpp`)

**3. Como um número não sai duas vezes no sorteio?**
> Mesma ideia, com o vetor dos já cantados: `sorteio` sorteia de 1 a 75 dentro de um
> `do-while` que só aceita quando `verificarSorteado` disser que aquele número nunca saiu.
> (`src/sorteio.cpp`)

**4. A lista de sorteados aparece em ordem crescente — vocês ordenam ela a cada rodada?**
> Não. `inserirEmOrdem` empurra para a direita todos os números maiores que o novo e encaixa
> ele no buraco: a lista **já nasce ordenada**. É ordenação por inserção, que está no PDF
> *Métodos de Pesquisa e ordenação* do professor. Reordenar 75 números a cada rodada seria
> trabalho jogado fora. (`src/sorteio.cpp`)

**5. Onde estão as sub-rotinas com passagem de parâmetros?**
> Em todo o programa — são 21 funções. Os três jeitos aparecem: **por valor**
> (`sorteio(... int quantidade)` nas contas internas), **por array** (`gerarCartela(int mat[][TAM])`,
> que já chega como endereço) e **por referência** — `lerOpcao(int &opcao)`,
> `inserirEmOrdem(int lista[], int &quantidade, int valor)` e `sorteio(int numSorteados[], int &quantidade)`,
> que escrevem direto na variável de quem chamou.

---

## Cartelas

**6. Por que ordenar a linha depois de gerar, e não gerar já em ordem?**
> Porque as duas regras são mais simples separadas: primeiro garantir que não repete,
> depois ordenar. Ordenar 5 elementos custa quase nada.

**7. O que é o `cond` do bubblesort?**
> É o sinalizador de troca. Se uma passagem inteira não trocou nada, a linha já está
> ordenada e o laço para antes da hora. (`src/cartela.cpp`)

**8. Como vocês garantem que não há duas cartelas iguais?**
> `cartelasIguais` compara posição a posição. Cada cartela nova é gerada dentro de um
> `do-while` que repete enquanto ela for igual a **alguma** das anteriores — a cartela 5
> é comparada com as quatro. (`src/jogo.cpp`)

**9. Duas cartelas com os mesmos números em ordem diferente enganam essa checagem?**
> Não, porque toda linha já foi ordenada antes. Mesmos números ⇒ mesma matriz.

---

## Sorteio e vitória

**10. Quando o jogo acaba?**
> No `while` do fim de `jogar`: para quando aparece o primeiro ganhador ou quando os
> 75 números acabam. (`src/jogo.cpp`)

**11. Como vocês sabem que uma cartela está completa?**
> `cartelaCompleta` testa os 25 números contra a lista de sorteados; basta **um** não ter
> saído para devolver `false` na hora. (`src/vitoria.cpp`)

**12. E se dois jogadores completarem na mesma rodada?**
> Os cinco são testados na mesma rodada e cada vencedor ganha a sua linha de mensagem,
> uma abaixo da outra. O contador `ganhadores` cuida disso. (`src/jogo.cpp`)

**13. Qual a diferença entre o vermelho e o verde?**
> Vermelho é o número que **acabou de sair**; verde é tudo que já tinha saído. Vale na
> cartela e na lista de sorteados. É o "destacando o atual valor sorteado com uma cor
> diferente" que o enunciado pede.

**14. Por que `quantidade` leva `&`?**
> Porque `inserirEmOrdem` e `sorteio` precisam **aumentar o contador de quem chamou**.
> Sem a referência, o `++` morreria dentro da função e `jogar` teria que lembrar de somar
> por fora — que é exatamente o tipo de erro que a referência elimina.

---

## Tela

**15. O que é `\033[10;5H`?**
> Sequência de escape ANSI: o caractere ESC, seguido de `linha;coluna` e a letra `H`, manda
> o terminal mover o cursor. É o que `gotoxy` monta. Veio do `codefun_GDB.h` do professor
> (está em `referencia/`).

**16. Por que `gotoxy` soma 1 em x e y?**
> O terminal conta linhas e colunas a partir de 1; o programa conta a partir de 0.

**17. Por que a tabela de cores tem 16 posições?**
> É o mapeamento dos códigos 0–15 do `codefun.h` (BLACK, BLUE, GREEN… RED = 4) para os
> códigos ANSI de fundo (40–47 e 100–107). `49` volta ao fundo padrão. A tabela é a mesma
> do header do professor.

**18. Como `pintarNumero` acerta a coluna certa dentro da cartela?**
> `exibir` separa os números com `\t`, e o terminal para a cada 8 colunas. `pintarNumero`
> refaz essa conta (`((posicaoX / 8) + 1) * 8`) para chegar na mesma célula.

**19. Por que `system("mode con: cols=120 lines=30")`?**
> Porque o tabuleiro tem três cartelas lado a lado e a lista de sorteados embaixo: medindo
> uma partida inteira, são 120 colunas por 30 linhas.

---

## Menu e entrada

**20. Por que o programa não fecha depois de uma partida?**
> `chamarMenu` é um `do-while` que só sai quando a opção é 3. `jogar` e `sobre` retornam
> para dentro dele. É o "só deve ser encerrado ao selecionar Sair" do enunciado.

**21. O que acontece se eu digitar uma letra no menu?**
> `cin.fail()` detecta que não veio número; `cin.clear()` tira o `cin` do estado de erro
> (sem isso, nenhuma leitura seguinte funciona); `cin.ignore(1000, '\n')` joga fora o que
> foi digitado; a opção volta a 0 e a pergunta se repete. Sem isso o programa entra em
> laço infinito — é o primeiro teste que se faz num menu.

**22. E se a entrada acabar (Ctrl+Z)?**
> `cin.eof()` cobre esse caso: insistir em perguntar seria laço infinito, então o programa
> trata como "Sair".

**23. Por que `getline` e não `cin >>` para o nome?**
> `cin >>` para no primeiro espaço: "João Vitor" ocuparia a vaga de dois jogadores — e o
> quinto nunca seria perguntado. `getline` lê a linha inteira. Funciona porque `lerOpcao`
> já descartou o Enter que sobrou do menu.

---

## Regras do enunciado

**24. Vocês usaram variável global?**
> Não. O que está fora de função são `#define` — macro de pré-processador, texto trocado
> antes de compilar. Toda variável vive dentro de uma função; as cinco cartelas são
> declaradas dentro do `main`.

**25. Usaram template?**
> Não, nenhum.

**26. Onde estão os autores?**
> No cabeçalho em comentário no topo do arquivo, e na tela **Sobre** — junto com o professor,
> a disciplina e o mês/ano. Vale 10% da nota.

**27. Por que o código está separado em vários arquivos, se a entrega é um só?**
> Os arquivos em `src/` são para ler e estudar; o arquivo entregue é gerado a partir deles
> e contém exatamente o mesmo código, na ordem em que compila. Foi conferido função por
> função: as 21 batem.

---

## A pergunta desconfortável

**28. Quem escreveu o quê?**
> **Combinem isso no grupo antes da aula.** O enunciado diz que a defesa é feita pelos
> integrantes e que sem ela a nota é zero. Cada um precisa conseguir explicar qualquer
> parte — inclusive as que não escreveu.

**29. Por que o código de vocês parece com o do João?**
> Ver `91-o-que-pode-ser-contestado.md`, item C13. Resumo: é o mesmo trabalho do mesmo grupo;
> a base é dele e esta versão é a evolução — com o que mudou registrado em `MUDANCAS.md`.
