# Jogadores — coleta o nome de quem joga cada cartela e guarda num vetor que o resto do jogo usa por índice

## O que esta parte resolve

O enunciado pede duas coisas de identificação: cada cartela precisa de um número e de um nome
(`enunciado.txt:49`), e esse nome tem que ser perguntado ao jogador no início da partida
(`enunciado.txt:66`). Esta parte é só isso: uma função que pergunta 5 nomes, um por cartela, e
devolve num vetor de `string` que `jogo.cpp` distribui para tudo que precisa mostrar "de quem" é
a cartela — o rótulo em cima do tabuleiro (`exibicao.cpp`) e a frase final de quem ganhou
(`jogo.cpp`).

Não é aqui que se decide layout, cor ou regra de vitória. É só a ponte entre o teclado e o vetor
`nome[]` que o resto do programa consome.

## Arquivos e funções

| Função | Arquivo:linha | O que faz em uma linha |
|---|---|---|
| `nomearCartelas` (declaração) | `jogadores.h:11` | Assinatura: recebe o vetor de nomes para preencher |
| `nomearCartelas` (implementação) | `jogadores.cpp:15-23` | Pergunta o nome de cada uma das `TAM` (5) cartelas e lê com `getline` |

## Como funciona, passo a passo

1. `jogo.cpp:39-56` gera as 5 cartelas primeiro (com os laços `do-while` que redesenham qualquer cartela repetida). Só depois disso, `jogo.cpp:58` chama `nomearCartelas(nome)`. A ordem é: primeiro todo o sorteio (dado numérico), depois a interação com o jogador (texto) — não há uma regra do enunciado exigindo essa ordem específica, foi organização de quem escreveu.

2. Dentro de `nomearCartelas` (`jogadores.cpp:17`), um `for (int i = 0; i < TAM; i++)` roda 5 vezes — uma por cartela. `TAM` vem de `comum.h:23` (`#define TAM 5`), a mesma constante que define a cartela como 5×5.

3. A cada volta, `jogadores.cpp:19` imprime `"Digite o nome do jogador da cartela " << (i + 1) << ": "` — o `i+1` converte o índice (0..4) para o número que o jogador vê (1..5), igual ao "Cartela: N" que `exibicao.cpp:55` mostra depois.

4. `jogadores.cpp:20` lê a resposta com `getline(cin, nome[i])`, guardando a linha inteira na posição `i` do vetor. Não há checagem de nome vazio nem limite de tamanho — quem aperta Enter sem digitar nada grava uma `string` vazia; ver **Limites conhecidos**.

5. Depois das 5 perguntas, `jogadores.cpp:22` roda `cout << "\033c"` — limpa a tela antes de `jogo.cpp` desenhar as 5 cartelas (`jogo.cpp:60-64`), então o jogador não vê os 5 prompts de nome misturados com o tabuleiro.

6. O vetor preenchido volta para `jogo.cpp` por parâmetro (é o mesmo `nome[]` que `chamarMenu` recebeu de `main.cpp:33` e repassou por `menu.cpp:68`). Duas partes usam esse vetor depois: `exibicao.cpp:58` (`nome[contador - 1]`, para rotular cada cartela na tela) e `jogo.cpp:101,108,115,122,129` (`nome[0]`..`nome[4]`, na frase de vitória).

## Por que foi feito assim

- **`getline` em vez de `cin >> nome[i]`**: `cin >>` corta a leitura no primeiro espaço. Um jogador chamado "João Vitor" com `cin >>` preencheria `nome[0]` só com "João" e a próxima chamada do laço leria "Vitor" como se fosse o nome do jogador 2 — o quinto jogador nunca seria perguntado, porque a "sobra" de um nome empurra todos os seguintes. Isso é exatamente o bug que existe na versão do colega (`docs/99-o-que-foi-medido.md`, seção "Bugs reproduzidos rodando": nome "Joao Vitor" vira dois jogadores diferentes). `getline` lê a linha toda até o Enter, então o espaço dentro do nome não quebra nada.

- **Por que o `getline` funciona sem precisar "limpar o buffer" antes**: essa é a pergunta que normalmente pega quem mistura `cin >>` com `getline`. Aqui funciona porque quem chama `jogar()` (e, por tabela, `nomearCartelas()`) é `chamarMenu` (`menu.cpp:64-68`), e a leitura da opção do menu já passou por `lerOpcao`, que faz `cin >> opcao` (`menu.cpp:19`) seguido de `cin.ignore(1000, '\n')` (`menu.cpp:32`) — isso descarta o `\n` que sobrou depois do número digitado. Quando `nomearCartelas` roda pela primeira vez, o buffer já está limpo; não é coincidência, é o `ignore` do menu que garante isso a cada volta do laço.

- **Vetor `string nome[TAM]` em vez de 5 variáveis soltas (`nome1`..`nome5`)**: o resto do programa já trata as cartelas por índice numérico (`contador - 1` em `exibicao.cpp:58`, `nome[0]` a `nome[4]` em `jogo.cpp`). Um vetor deixa o laço `for` de `nomearCartelas` genérico (roda 5 vezes sem repetir código) e mantém a mesma convenção de índice usada em todo o resto do jogo: "cartela N" é sempre `nome[N-1]`.

- **Alternativa que não foi usada — perguntar o nome já dentro do laço que gera cada cartela**: daria menos uma função, mas misturaria duas responsabilidades diferentes (sortear números vs. ler texto do teclado) na mesma função, e essa mistura é exatamente o tipo de coisa que os critérios de "organização e clareza" (`enunciado.txt:90`) penalizam. Separar em `nomearCartelas` também é o que o enunciado pede em "Utilize sub-rotinas e passagem de parâmetros" (`enunciado.txt:70`): é uma sub-rotina com uma responsabilidade única.

## Requisitos do enunciado que esta parte atende

- **R14** — "Cada cartela terá um número de identificação e um nome" (`enunciado.txt:49`) + "deve-se solicitar o nome do jogador de cada cartela" (`enunciado.txt:66`). O número vem de `cartela.cpp`/`exibicao.cpp` (fora desta parte); o nome é o que `nomearCartelas` coleta.
- **R28** — "Ao iniciar o jogo, deve-se solicitar o nome do jogador de cada cartela." (`enunciado.txt:66`). É a função inteira: 5 prompts, um por cartela, antes do sorteio começar (`jogo.cpp:58` roda antes de `jogo.cpp:68` pedir o primeiro Enter do sorteio).
- **R30** — "Utilize sub-rotinas e passagem de parâmetros." (`enunciado.txt:70`). `nomearCartelas` recebe `nome[]` por parâmetro e escreve nele; é o vetor declarado em `main.cpp:33` passando por três camadas de chamada (`main` → `chamarMenu` → `jogar` → `nomearCartelas`) até ser preenchido.
- Contribui (mas não resolve sozinha) para a **R29** — "Ao final, deve ser exibido o nome do jogador e o número da cartela vencedora" (`enunciado.txt:66-67`): esta parte só garante que `nome[]` esteja preenchido; quem de fato imprime a frase de vitória é `jogo.cpp:101-129`.

## Perguntas de defesa sobre esta parte

1. **Por que usar `getline` e não `cin >> nome[i]`?**
   `cin >>` para no primeiro espaço; um nome composto ("João Vitor") viraria dois nomes e desalinharia todos os jogadores seguintes. `getline` lê a linha inteira até o Enter.

2. **Como vocês garantem que o `getline` não lê uma linha vazia sobrando de uma leitura anterior?**
   Porque quem chama antes é `lerOpcao` (`menu.cpp:19-32`), que já descarta o `\n` deixado pelo `cin >> opcao` com `cin.ignore(1000, '\n')` (`menu.cpp:32`). O buffer já chega limpo em `nomearCartelas`.

3. **Por que o nome fica num vetor e não em variáveis separadas por jogador?**
   Porque todo o resto do jogo já referencia as cartelas por índice (`nome[contador-1]` em `exibicao.cpp:58`, `nome[0]`..`nome[4]` em `jogo.cpp`); um vetor deixa o laço genérico e mantém a mesma convenção em todo o código.

4. **O que acontece se o jogador apertar Enter sem digitar nada?**
   `getline` aceita string vazia sem erro; `nome[i]` fica `""`. Não há checagem contra isso em `jogadores.cpp:15-23`, então uma cartela pode ficar sem nome visível e, se ela ganhar, a frase final (`jogo.cpp:101` etc.) sairia sem o nome antes de "ganhou com a cartela N!".

5. **Por que `nomearCartelas` roda depois de gerar as 5 cartelas, e não antes?**
   Não há exigência do enunciado nessa ordem — foi escolha de organização: primeiro todo o sorteio de números (`jogo.cpp:39-56`, que pode re-rodar por causa de cartela repetida), depois a única parte que pede algo digitado, para não misturar as duas fases na tela.

6. **`nomearCartelas` usa passagem por referência (`&`)? Por que sim ou por que não?**
   Não usa `&` explícito porque `string nome[]` já decai para ponteiro ao ser passado como parâmetro de array — o array em si já é passado por referência efetiva; a função escreve direto nas posições do vetor original, sem precisar copiar nada de volta. `&` explícito (`int &quantidade`) só é necessário para tipos escalares, como o `quantidade` de `sorteio` — não para arrays.

## Limites conhecidos

- **Nome vazio não é barrado**: `jogadores.cpp:15-23` não tem nenhuma checagem contra `getline` retornando string vazia. Não rodei para confirmar o efeito visual exato, mas pela leitura do código o vetor aceitaria `""` sem aviso, e a frase de vitória (`jogo.cpp:101,108,115,122,129`) imprimiria só "ganhou com a cartela N!" sem nome antes.
- **Nome sem limite de tamanho**: não há `substr`/truncamento em `jogadores.cpp:20`. `exibicao.cpp:58` imprime `"Jogador: " << nome[contador-1]` sem limpar a linha antes (diferente de `mostrarSorteado`, que limpa a linha em `exibicao.cpp:17` antes de reescrever). Como as colunas das cartelas ficam em x=2, 42 e 82 na mesma linha (`jogo.cpp:29-31`), um nome muito longo na cartela mais à direita de cada fileira (cartela 3 ou cartela 5, que não têm outra cartela depois para "sobrescrever" a sobra) não tem limite de coluna — não testei o efeito exato na tela, mas o código não protege contra isso.
- **`TAM` faz dupla função**: `comum.h:23` define `TAM` como o tamanho da cartela (5×5), e `jogadores.cpp:17` reaproveita o mesmo `TAM` como "número de jogadores/cartelas". Funciona porque o enunciado fixa os dois em 5 (`enunciado.txt:47`, "5 cartelas" e `enunciado.txt:14`, "cinco colunas... cinco linhas"), mas é coincidência de valor, não a mesma coisa conceitualmente — se um dia o jogo aceitasse cartelas 5×5 para mais ou menos de 5 jogadores, essa reutilização quebraria.
