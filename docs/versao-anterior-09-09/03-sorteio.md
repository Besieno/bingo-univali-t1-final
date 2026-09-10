# Sorteio — a urna do bingo: tira um número novo de 1 a 75, garante que nunca repete e entrega a lista sempre em ordem crescente

## O que esta parte resolve

O jogo precisa cantar números de 1 a 75, um de cada vez, sem nunca repetir um já sorteado, e mostrar
esses números na tela sempre em ordem crescente (não na ordem em que saíram). Essa parte guarda três
responsabilidades separadas: **saber se um número já saiu** (`verificarSorteado`), **guardar um número
novo já no lugar certo da lista ordenada** (`inserirEmOrdem`) e **orquestrar as duas coisas pra entregar
um número válido** (`sorteio`). Quem usa essa entrega é `jogo.cpp`, que chama `sorteio()` uma vez por
rodada dentro do laço principal da partida.

## Arquivos e funções

| Função | Arquivo:linha | O que faz em uma linha |
|---|---|---|
| `verificarSorteado` | `sorteio.cpp:11` (declarada em `sorteio.h:9`) | Varre os números já sorteados e diz se `num` já está entre eles |
| `inserirEmOrdem` | `sorteio.cpp:23` (declarada em `sorteio.h:10`) | Insere `valor` na posição certa do vetor, empurrando os maiores pra manter a ordem crescente |
| `sorteio` | `sorteio.cpp:37` (declarada em `sorteio.h:11`) | Sorteia números aleatórios até achar um inédito, insere na lista ordenada e devolve o número sorteado |

## Como funciona, passo a passo

**`verificarSorteado` (`sorteio.cpp:11-21`)** — busca linear simples. O `for` roda só de `0` até
`quantidade` (`sorteio.cpp:13`), ou seja, só passa pelas posições do vetor que já têm número de verdade;
o resto do array de 75 posições (declarado em `jogo.cpp:24`) ainda está com lixo de memória e nunca é
lido. Se algum `vet[i]` bater com `num`, retorna `true` na hora (`sorteio.cpp:15-18`); se o laço terminar
sem achar, retorna `false` (`sorteio.cpp:20`).

**`inserirEmOrdem` (`sorteio.cpp:23-35`)** — é uma inserção ordenada, o mesmo método do PDF de ordenação
da disciplina (citado no comentário do topo do arquivo, `sorteio.cpp:3`), só que aplicado a um elemento
por vez em vez de ordenar um vetor inteiro de uma vez. `pos` começa no fim da lista preenchida
(`pos = quantidade`, linha 25). Enquanto ainda tem elemento à esquerda de `pos` **e** esse elemento é
maior que `valor`, ele desliza uma casa pra direita (`sorteio.cpp:27-31`) — é exatamente o "abre espaço"
de uma inserção clássica. Quando o laço para (achou um elemento menor ou chegou no início do vetor),
`pos` é a posição certa: `lista[pos] = valor` (linha 33) encaixa o número ali. `quantidade++` (linha 34)
fecha a função — e como `quantidade` é parâmetro por referência (`int &quantidade` em `sorteio.h:10`),
esse incremento já vale pra variável de quem chamou, sem precisar de nenhum passo extra do lado de fora.

**`sorteio` (`sorteio.cpp:37-49`)** — é quem junta as duas peças. Um `do-while` (linhas 41-44) sorteia
`rand() % 75 + 1` (linha 43: resto da divisão por 75 dá `0..74`, o `+1` desloca pra `1..75`) e só para
de tentar de novo quando `verificarSorteado(numSorteados, num, quantidade)` devolver `false` — ou seja,
essa é uma **amostragem por rejeição**: chuta um número, se já saiu descarta e chuta outro, até acertar
um inédito. Achado o número, `inserirEmOrdem` (linha 46) guarda ele já ordenado e soma 1 em `quantidade`;
`sorteio` devolve `num` (linha 48) pra quem chamou saber qual foi o número da vez.

**Onde isso entra na partida (`jogo.cpp`)** — `numSorteados[75]` e `quantidade = 0` nascem no início de
`jogar` (`jogo.cpp:24-25`). Dentro do laço da partida, cada rodada chama `int num = sorteio(numSorteados,
quantidade);` (`jogo.cpp:72`) uma única vez — é essa única chamada por volta do laço que garante "um
sorteio por vez". O número volta pintado de vermelho nas cartelas (`jogo.cpp:78-82`), o jogo espera um
Enter (`cin.ignore(1000, '\n')` em `jogo.cpp:85`) e só então pinta de verde (`jogo.cpp:88-92`) e testa se
alguém fez bingo. `exibirSorteados` (`jogo.cpp:75` e `94`) é chamada logo depois de cada `sorteio()` —
ela não pertence a este arquivo, mas é o que efetivamente imprime a lista na tela; a ordenação que ela
mostra já vem pronta de `inserirEmOrdem`, `exibirSorteados` só percorre o vetor e imprime.

## Por que foi feito assim

**Sortear por rejeição em vez de embaralhar um baralho de 1 a 75 uma vez só.** A alternativa clássica
seria montar um vetor com os 75 números em ordem, embaralhar ele inteiro uma vez (Fisher-Yates) e ir
consumindo em sequência — aí nunca haveria a chance de sortear um número repetido, e cada sorteio seria
O(1). Não foi esse o caminho porque embaralhar um vetor é um algoritmo a mais pra explicar e testar, e o
domínio aqui é pequeno (no máximo 75 tentativas mesmo no pior caso, porque `verificarSorteado` corta
qualquer repetição). "Chuta e confere" é mais simples de justificar na defesa e reaproveita a mesma ideia
de busca linear que já aparece em outras partes do trabalho (a checagem de número repetido dentro da
própria linha da cartela, por exemplo). O custo de re-tentar cresce um pouco perto do fim (com 74 números
já sorteados, a chance de acertar de primeira é 1/75), mas isso nunca trava o programa: o `do-while`
sempre acaba encontrando o número que falta.

**Inserção ordenada em vez de reordenar tudo a cada rodada.** O comentário do arquivo já registra essa
decisão (`sorteio.cpp:3-4`): em vez de rodar um sort completo no vetor de sorteados toda vez que um
número novo entra, `inserirEmOrdem` só desloca os elementos maiores que o valor novo e encaixa ele no
lugar certo. Reordenar do zero a cada rodada (por exemplo repetindo o mesmo bubblesort usado nas linhas
da cartela) faria cada vez mais trabalho conforme a lista cresce — e esse trabalho seria refeito do zero
em toda rodada, mesmo a lista já estando ordenada da rodada anterior. Inserir mantém a invariante "a lista
já está ordenada" o tempo todo, então o trabalho por rodada é só o necessário pra aquele número entrar.

**`quantidade` por referência (`sorteio.h:10-11`).** Tanto `inserirEmOrdem` quanto `sorteio` recebem
`int &quantidade`. A alternativa seria devolver a nova quantidade como valor de retorno (ou exigir que
`jogo.cpp` fizesse `quantidade++` depois de cada chamada) — só que isso obriga quem chama a lembrar de
atualizar o contador na mão, e esquecer esse passo é exatamente o tipo de bug fácil de escapar (fica um
sorteio "perdido": o número foi guardado mas `quantidade` não sabe disso, e a próxima verificação de
repetição passa a ignorar essa posição do vetor). Com referência, o contador sobe dentro da própria
função que mexeu na lista, num lugar só, e satisfaz a exigência do enunciado de usar passagem de
parâmetros (`enunciado.txt:70`) de um jeito que realmente muda o comportamento de quem chamou — não é
passagem por referência decorativa.

**Vetor de tamanho fixo 75, sem alocação dinâmica.** `numSorteados[75]` é declarado com tamanho fixo em
`jogo.cpp:24`. Como só existem 75 valores possíveis e `verificarSorteado` impede repetição, esse vetor
nunca precisa de mais que 75 posições — não há motivo pra uma estrutura que cresce dinamicamente (o que,
além de desnecessário aqui, entraria em conflito com a regra de não usar recurso fora do que foi visto em
aula, `enunciado.txt:72`).

**Três funções pequenas em vez de uma função só.** `verificarSorteado` só responde uma pergunta binária,
`inserirEmOrdem` só resolve onde encaixar um valor, e `sorteio` só orquestra as duas. Juntar tudo numa
função só funcionaria, mas dificultaria explicar cada peça isoladamente na defesa oral e tornaria mais
fácil esconder um bug (por exemplo, misturar a lógica de checagem de repetição com a de inserção faria
mais difícil garantir que a lista fica ordenada). Sub-rotinas pequenas e nomeadas pelo que fazem é
literalmente a exigência do enunciado (`enunciado.txt:70`, "Utilize sub-rotinas e passagem de
parâmetros").

## Requisitos do enunciado que esta parte atende

- **R20** — `enunciado.txt:56` "Serão sorteados valores de 1 a 75" → `sorteio.cpp:43`, `rand() % 75 + 1`.
- **R21** — `enunciado.txt:56-59` "os valores não devem ser repetidos [...] se foi sorteado o número 2, o
  mesmo não pode ser sorteado uma segunda vez" → `verificarSorteado` (`sorteio.cpp:11-21`) dentro do
  `do-while` de `sorteio` (`sorteio.cpp:41-44`).
- **R22** — `enunciado.txt:57` "devem ser exibidos na tela em ordem crescente" → a ordenação em si é
  produzida aqui, por `inserirEmOrdem` (`sorteio.cpp:23-35`); quem imprime na tela é `exibirSorteados`
  (chamada em `jogo.cpp:75` e `94`), mas ela só tem uma lista pra imprimir em ordem porque recebeu o vetor
  já ordenado por esta parte.
- **R30** — `enunciado.txt:70` "Utilize sub-rotinas e passagem de parâmetros" → três funções (nenhuma é
  `main`), com parâmetro por valor (`num`, `valor`) e por referência (`&quantidade`, `sorteio.h:10-11`).
- **R26** (parcial, compartilhado com `jogo.cpp`) — `enunciado.txt:63` "Cada sorteio só deve ser realizado
  ao selecionar a tecla 'enter'" → `sorteio()` em si não sabe nada sobre Enter; quem garante uma chamada
  por rodada é o laço de `jogo.cpp` (chamada única em `jogo.cpp:72`, espera em `jogo.cpp:85`). Ver
  "Limites conhecidos" abaixo.

## Perguntas de defesa sobre esta parte

1. **Como vocês garantem que o número sorteado fica entre 1 e 75?**
   `rand() % 75` dá um resto entre `0` e `74`; somar `1` (`sorteio.cpp:43`) desloca o intervalo pra `1..75`.

2. **Como garantem que um número não sai duas vezes?**
   `verificarSorteado` (`sorteio.cpp:11-21`) varre só as posições já preenchidas do vetor de sorteados; o
   `do-while` de `sorteio` (`sorteio.cpp:41-44`) sorteia de novo enquanto ela devolver `true`.

3. **A lista aparece sempre em ordem crescente — vocês ordenam tudo de novo a cada rodada?**
   Não. `inserirEmOrdem` (`sorteio.cpp:23-35`) encaixa cada número novo direto na posição certa,
   deslocando só quem é maior que ele; a lista nunca fica desordenada, então nunca precisa de um sort
   completo.

4. **Por que `quantidade` é `int &quantidade` e não `int quantidade`?**
   Porque `inserirEmOrdem` e `sorteio` (`sorteio.h:10-11`) precisam alterar o contador de quem chamou
   (a variável `quantidade` declarada em `jogo.cpp:25`) diretamente. Com passagem por valor, o incremento
   se perderia ao sair da função e `jogo.cpp` teria que somar na mão depois de cada chamada — um passo
   fácil de esquecer.

5. **O que aconteceria se tirassem o `do-while` de `sorteio` e chamassem `rand() % 75 + 1` só uma vez?**
   Nada impediria sortear um número repetido — a única coisa que garante unicidade é justamente esse
   laço testando `verificarSorteado` antes de aceitar o número. Sem ele, R21 quebra.

6. **Por que na linha 27 de `sorteio.cpp` está escrito `and` e não `&&`, como no resto do projeto?**
   São sinônimos padrão da linguagem (operadores alternativos do C++, equivalentes a `&&`/`||`/`!`), não
   é comando externo nem biblioteca — mas o projeto usa os dois estilos em arquivos diferentes, o que é
   uma inconsistência de escrita, não um erro funcional.

## Limites conhecidos

- **A responsabilidade de esperar o Enter não mora aqui.** `sorteio()` sempre entrega exatamente um
  número novo por chamada, e quem decide *quando* chamar é o laço de `jogo.cpp` (uma chamada por volta,
  `jogo.cpp:72`). O `cin.ignore(1000, '\n')` que espera o Enter (`jogo.cpp:68` e `85`) não limpa o que
  sobrar no buffer de entrada antes de esperar — não há um `cin.clear()`/checagem de `rdbuf()->in_avail()`
  antes dele. Isso quer dizer que, se o jogador digitar Enter antes da hora (por exemplo, apertar duas
  vezes seguidas), esse Enter extra pode ser consumido sem o jogador perceber, adiantando a rodada
  seguinte. É um limite da integração com `jogo.cpp`, não da lógica de `sorteio.cpp`/`sorteio.h` em si.
  Não rodei esse cenário ao vivo nesta sessão — a conclusão vem da leitura do código, não de um teste
  reproduzido.

- **`and` na linha 27 é inconsistente com `&&` usado no resto do projeto** (mesmo ponto que aparece no
  relatório de auditoria para o arquivo inteiro). Funciona igual, mas é bom estar pronto pra explicar por
  que os dois estilos convivem.

- **`verificarSorteado` não valida o intervalo de `num`.** A função confia que quem chama sempre passa um
  número entre 1 e 75 — ela não rejeitaria, por exemplo, um `num` negativo ou maior que 75, só diria "não
  está na lista". Isso nunca é um problema na prática porque a única chamada real vem de dentro do
  próprio `sorteio()`, que já gera o número no intervalo certo (`sorteio.cpp:43`) — mas é uma checagem que
  a função não faz sozinha, fica por conta de quem a usa.
