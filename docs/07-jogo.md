# Jogo — o maestro que junta cartela, sorteio, exibição e vitória numa partida do início ao fim

## O que esta parte resolve

`jogo.h` e `jogo.cpp` implementam uma função só, `jogar` (assinatura em `jogo.h:12`, corpo em `jogo.cpp:22-141`). Ela é chamada uma vez por partida, a partir do menu (`menu.cpp:68`), e não implementa nenhuma regra do bingo por conta própria — quem sorteia sem repetir é `sorteio.cpp`, quem gera cartela é `cartela.cpp`, quem decide se alguém ganhou é `vitoria.cpp`, quem desenha é `exibicao.cpp`/`tela.cpp`. O trabalho de `jogar` é decidir a **ordem** e o **momento** de cada uma dessas chamadas, e guardar num só lugar as coordenadas de tela do tabuleiro inteiro (`jogo.cpp:29-35`). É por isso que o comentário de topo do arquivo chama essa função de "maestro" (`jogo.cpp:2`).

## Arquivos e funções

| função | arquivo:linha | o que faz |
|---|---|---|
| `jogar` | `jogo.h:12` (assinatura) · `jogo.cpp:22-141` (implementação) | Comanda a partida inteira: gera as 5 cartelas distintas, pede os nomes, desenha o tabuleiro uma vez e repete sorteia -> destaca -> espera Enter -> confere vencedor até alguém completar a cartela (ou os 75 números acabarem). |

## Como funciona, passo a passo

**1. Estado da partida** (`jogo.cpp:24-26`) — três variáveis locais: `numSorteados[75]` guarda os números já cantados nesta partida em ordem crescente; `quantidade` conta quantos já saíram; `ganhadores` conta quantas cartelas fecharam na rodada atual. Nenhuma é global — todas nascem dentro de `jogar` e morrem quando a partida acaba, o que é exigência do enunciado (`enunciado.txt:72`, R32).

**2. Coordenadas centralizadas** (`jogo.cpp:29-35`) — as posições das 5 cartelas (`x1,y1` ... `x5,y5`), da lista de sorteados (`xSorteados,ySorteados`) e da linha de avisos (`xAviso,yAviso`) são declaradas juntas, antes de qualquer chamada. O comentário do topo do arquivo já avisa a razão (`jogo.cpp:5-6`): mexer no layout do jogo é mexer nessas 7 linhas, não caçar números soltos espalhados pelas chamadas.

**3. Geração das 5 cartelas, sem repetir nenhuma** (`jogo.cpp:39-56`) — a cartela 1 é gerada direto (`39`, chama `gerarCartela` de `cartela.cpp:46-66`). Cada cartela seguinte nasce dentro de um `do-while` que só sai quando ela é diferente de **todas** as anteriores: a cartela 2 é regerada enquanto for igual à 1 (`41-44`); a 3, enquanto for igual à 1 ou à 2 (`45-48`); a 4, contra 1, 2 ou 3 (`49-52`); a 5, contra 1, 2, 3 ou 4 (`53-56`). A comparação em si (`cartelasIguais`, posição a posição) mora em `cartela.cpp:68-81` — `jogo.cpp` só decide contra quem cada cartela nova precisa ser checada.

**4. Nomes e desenho inicial** (`jogo.cpp:58-64`) — `nomearCartelas(nome)` (`58`) pergunta o nome de cada jogador, um `getline` por vez (implementação em `jogadores.cpp:15-23`). Depois, `exibir` é chamado uma vez para cada cartela, cada uma com seu próprio par de coordenadas e seu número de identificação literal (1 a 5, linhas `60-64` — é aqui que a cartela "recebe" o ID que R13 pede). `exibir` (`exibicao.cpp:52-69`) escreve o cabeçalho "Cartela: N" / "Jogador: nome" e depois os 25 números com `\t` entre eles.

**5. Primeiro Enter** (`jogo.cpp:66-68`) — antes de sortear qualquer coisa, o programa escreve "Pressione Enter para comecar o sorteio..." e trava em `cin.ignore(1000, '\n')`. Esse é o único Enter que existe fora do laço principal.

**6. O laço do sorteio** (`jogo.cpp:70-140`, um `do-while`) — a cada volta:
- `sorteio(numSorteados, quantidade)` (`72`) sorteia um número de 1 a 75 que ainda não saiu e já o guarda em ordem crescente dentro de `numSorteados` (lógica em `sorteio.cpp:37-49`).
- `mostrarSorteado` (`74`) escreve "Numero sorteado: N" na linha de aviso; `exibirSorteados` (`75`) reimprime a lista inteira de já sorteados, em ordem, com o número atual em destaque (`exibicao.cpp:71-103`).
- As 5 cartelas recebem `pintarNumero(..., RED)` (`78-82`) — se o número existe na cartela, aquela célula fica vermelha; se não existe, a função não faz nada naquela cartela (`exibicao.cpp:22-50`, o `return` de dentro do laço interno).
- `cin.ignore(1000, '\n')` (`85`) trava esperando o Enter da rodada.
- Depois do Enter, as mesmas 5 cartelas recebem `pintarNumero(..., GREEN)` (`88-92`) — o número que estava vermelho vira verde — e a lista de sorteados é redesenhada sem destaque (`atual = 0`, linha `94`).
- `ganhadores` volta a 0 (`96`) e as 5 cartelas são testadas contra `cartelaCompleta` (`vitoria.cpp:10-24`), uma por uma, em blocos `if` independentes — não `if/else` (`98-131`). Cada cartela completa imprime `nome[i] ganhou com a cartela N!` numa linha própria, deslocada por `yAviso + 1 + ganhadores` (`100,107,114,121,128`), e só depois incrementa `ganhadores`.
- Se alguém ganhou, aparece "Pressione Enter para voltar ao menu..." e o programa espera o último Enter (`133-138`).
- A condição de parada do `do-while` é `ganhadores == 0 && quantidade < 75` (`140`): o laço só continua enquanto ninguém venceu e ainda sobram números pra sortear.

**7. Fim** — quando o laço termina, `jogar` simplesmente retorna; quem limpa a tela e reimprime as opções é `chamarMenu` (`menu.cpp:58`), no próximo giro do menu.

## Por que foi feito assim

**Coordenadas num bloco só, não espalhadas** (`jogo.cpp:29-35`) — a alternativa óbvia era passar números literais direto em cada chamada de `exibir`/`pintarNumero`. Isso funciona, mas qualquer ajuste de layout (por exemplo, mover a cartela 3) obrigaria a caçar o mesmo número em várias linhas espalhadas pelo arquivo, com risco real de esquecer uma chamada e desalinhar cartela com pintura. Nomear as coordenadas uma vez resolve isso.

**Cascata de `do-while` em vez de um vetor de cartelas** — a assinatura usa `mat1` a `mat5` como cinco parâmetros distintos (`jogo.h:12`), não `int cartelas[5][TAM][TAM]`. Isso deixa cada comparação de R16 (cartela repetida) escrita por extenso — "a 4 não pode ser igual à 1, à 2 nem à 3" — em vez de escondida dentro de um laço `for` sobre um vetor de matrizes. É mais repetitivo (4 blocos quase iguais, linhas `41-56`), mas também é mais fácil de apontar numa defesa oral exatamente qual comparação garante qual regra. A alternativa com vetor de matrizes reduziria a repetição, mas exigiria um laço aninhado a mais para não perder a leitura "compara com todas as anteriores".

**Vermelho antes do Enter, verde depois** (`78-82` -> `85` -> `88-92`) — usar duas cores em dois momentos, e não uma cor só, é o que separa R23 (destacar o número que acabou de sair) de R18 (marcar os que já saíram antes). Se fosse uma cor só, não daria pra saber olhando a cartela se aquele número é da rodada atual ou de três rodadas atrás.

**`ganhadores` testado em cinco `if` independentes, não `if/else`** (`98-131`) — com `if/else` só a primeira cartela completa seria anunciada. Testar as cinco sempre permite reconhecer duas cartelas fechando no mesmo sorteio (o enunciado não proíbe isso), e o deslocamento `yAviso + 1 + ganhadores` empilha as mensagens sem uma sobrescrever a outra.

**`quantidade < 75` além de `ganhadores == 0`** (`140`) — matematicamente redundante: como cada cartela só tem números entre 1 e 75, no momento em que os 75 saem, toda cartela já deveria estar completa. A condição extra é um cinto de segurança contra laço infinito caso alguma outra parte do código falhe em marcar a vitória.

**`exibir` chamado uma vez só, fora do laço** (`60-64`) — a alternativa era redesenhar as cartelas a cada rodada, como o enunciado poderia sugerir ("exibir durante toda a execução"). Aqui a leitura foi diferente: desenhar uma vez e nunca apagar já satisfaz "durante toda a execução" (R17), e evita redesenhar 25 números x 5 cartelas a cada Enter — só a célula que mudou (`pintarNumero`) é reescrita.

**`cin.ignore(1000, '\n')` em vez de `cin.get()`** (`68`, `85`, `137`) — descarta a linha inteira pendente (até 1000 caracteres ou até o `\n`) de uma vez, então um caractere perdido de uma leitura anterior não atravessa para o próximo Enter esperado.

## Requisitos do enunciado que esta parte atende

- **R11** — "Deve-se gerar então 5 cartelas randomicamente" (`enunciado.txt:47`). O número 5 é estrutural aqui: 1 chamada direta + 4 blocos `do-while` (`jogo.cpp:39-56`).
- **R16** — "Não pode haver cartelas repetidas" (`enunciado.txt:51`). A decisão de comparar cada cartela nova contra todas as anteriores é escrita nesta função (`41-56`); quem compara de fato é `cartelasIguais` (`cartela.cpp:68-81`).
- **R17** — "Exibir na tela as cartelas geradas durante toda a execução" (`enunciado.txt:52`). Garantido por `exibir` ser chamado uma única vez, antes do laço, e nunca apagado (`60-64`).
- **R18 / R23 / R27** — "Marcar os valores já sorteados... com cores diferenciadas" e "destacando o atual valor sorteado com uma cor diferente" (`enunciado.txt:53`, `57-58`, `63-64`). A sequência vermelho-antes-do-Enter / verde-depois é decidida aqui (`78-82`, `85`, `88-92`).
- **R22 / R24** — números sorteados "exibidos na tela em ordem crescente" e visíveis "durante todo o sorteio" (`enunciado.txt:57`, `60-61`). `exibirSorteados` é chamado a cada rodada, duas vezes (`75`, `94`) — a ordenação em si é de `sorteio.cpp`.
- **R25** — "o sorteio para quando um dos jogadores fizer 'bingo'" (`enunciado.txt:59`). É a condição do laço, `ganhadores == 0` (`140`).
- **R26** — "Cada sorteio só deve ser realizado ao selecionar a tecla enter" (`enunciado.txt:63`). É a posição do único `cin.ignore` do laço (`85`), entre mostrar o número e voltar ao topo.
- **R28** — "Ao iniciar o jogo, deve-se solicitar o nome do jogador de cada cartela" (`enunciado.txt:66`). `nomearCartelas` é chamado logo depois de gerar as 5 cartelas e antes de qualquer sorteio (`58`).
- **R29** — "Ao final, deve ser exibido o nome do jogador e o número da cartela vencedora" (`enunciado.txt:66-67`). É o texto literal `nome[i] << " ganhou com a cartela N!"` (`101,108,115,122,129`).
- **R30** — "Utilize sub-rotinas e passagem de parâmetros" (`enunciado.txt:70`). `jogar` recebe 6 parâmetros (`jogo.h:12`) e demonstra passagem por referência no ponto onde chama `sorteio(numSorteados, quantidade)` (`72`) — `quantidade` é `int` comum em `jogar`, mas o segundo parâmetro de `sorteio` é `int&` (`sorteio.h:11`), então o contador sobe sozinho sem `jogar` precisar de um `quantidade++` depois da chamada.

## Perguntas de defesa sobre esta parte

1. **Como a função garante que as 5 cartelas geradas sejam todas diferentes entre si?**
   Cada cartela nova entra num `do-while` que só sai quando ela é diferente de todas as anteriores: a 2ª compara com a 1ª (`jogo.cpp:41-44`), a 3ª com a 1ª e a 2ª (`45-48`), e assim por diante até a 5ª comparar com as outras quatro (`53-56`).

2. **Como o programa garante que um sorteio só acontece depois que o jogador aperta Enter?**
   `sorteio()` é chamado no topo do laço (`72`); o único `cin.ignore` do corpo do laço (`85`) fica depois de mostrar o número em vermelho e antes do laço voltar ao topo — então o próximo número só é sorteado depois que o Enter da rodada atual foi consumido.

3. **O que acontece se duas cartelas completarem no mesmo sorteio?**
   As cinco checagens de `cartelaCompleta` (`98-131`) são `if` independentes, não `if/else`. Mais de um `ganhadores++` pode acontecer na mesma rodada, e cada mensagem usa `gotoxy(xAviso, yAviso + 1 + ganhadores)` (`100,107,114,121,128`) pra empilhar sem uma sobrescrever a outra.

4. **Onde está a passagem por referência dentro dessa função?**
   `quantidade` é uma variável local de `jogar` (`25`), passada para `sorteio(numSorteados, quantidade)` (`72`). O segundo parâmetro de `sorteio` é `int&` (`sorteio.h:11`), então o valor volta atualizado para `jogar` sem precisar de retorno nem de um incremento manual depois da chamada.

5. **Por que existem 5 parâmetros de matriz (`mat1` a `mat5`) em vez de um vetor de cartelas?**
   Não conferi essa decisão com o grupo. A estrutura escolhida deixa cada comparação de "cartela repetida" escrita por extenso, uma cartela contra as anteriores por nome (`41-56`), o que é mais fácil de apontar linha a linha numa defesa do que um laço `for` sobre um vetor de matrizes.

6. **Por que o laço para em `quantidade < 75` além de `ganhadores == 0`?**
   É um cinto de segurança (`140`): como toda cartela só tem números entre 1 e 75, no momento em que os 75 saem, toda cartela já deveria estar completa — esse ramo da condição não deveria disparar sozinho, mas evita um laço infinito se algo falhar em outro lugar do código.

## Limites conhecidos

- **Nenhum lembrete de "aperte Enter" durante o sorteio.** Conferido: entre as linhas `70` e `138`, o único texto que pede Enter de forma explícita é o de antes do laço (`66-67`, "Pressione Enter para comecar o sorteio..."). Dentro do laço, a única pista na tela é "Numero sorteado: N" (`74`, implementado em `exibicao.cpp:14-20`) — não é um pedido, é só a informação do resultado. Quem não souber a regra de antemão pode achar que o programa travou.
- **Enter em fila pode adiantar uma rodada sem o jogador perceber.** `cin.ignore(1000, '\n')` (`68` e `85`) descarta só a próxima linha do buffer de entrada. Se o jogador apertar Enter duas vezes seguidas antes de o programa consumir a primeira, a segunda fica na fila e é engolida silenciosamente pela chamada seguinte — adiantando um sorteio sem uma decisão consciente naquela rodada. Não reproduzi esse cenário ao vivo nesta versão (o `docs/99-o-que-foi-medido.md` só testou esse tipo de problema na versão do João, não neste arquivo); é dedução direta de como `cin.ignore` funciona, não teste.
- **Nome comprido pode invadir a cartela vizinha.** O `docs/99-o-que-foi-medido.md` mediu que a Versão Final ocupa exatamente 120x30, sem sobra. As cartelas 1 e 2 ficam a 40 colunas de distância (`x1=2`, `x2=42`, `jogo.cpp:29-30`), e `exibir` escreve `"Jogador: " + nome` sem cortar (`exibicao.cpp:57-58`). Um nome longo o bastante invade visualmente a área da cartela ao lado até ser sobrescrito por ela, já que `exibir(mat2,...)` roda logo em seguida (`jogo.cpp:61`). Não testei ao vivo com um nome desse tamanho.
