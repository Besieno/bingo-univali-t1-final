> **O que é este arquivo:** o enunciado do professor quebrado em requisitos numerados
> (R01, R02...), cada um com a citação literal e como se testa rodando o programa.
> É a régua usada nos outros documentos — quando algum deles cita "R22", é aqui que está.
> O texto do enunciado sem formatação está em `enunciado.txt`, na raiz; o PDF original é o `enunciado.pdf`.

# Requisitos atômicos — Trabalho M1 Bingo (UNIVALI, Algoritmos e Programação II)

Fonte única das citações: `enunciado.txt` (99 linhas, lido por inteiro). Toda citação abaixo é literal do arquivo, com linha real.

## A. Prazo, modalidade e entrega

**R01** — Prazo de entrega
> `enunciado.txt:8` "Data de entrega: 11/09/2026. (até 08:00h)"
Teste: não roda no programa; conferir data/hora do upload no ambiente virtual.

**R02** — Grupo de 5 integrantes
> `enunciado.txt:9` "Modalidade: Cinco integrantes."
Teste: logístico; reflete em R06 (Sobre) e R39 (autores no código) — os dois devem listar exatamente 5 nomes.

**R36** — Postar na atividade certa
> `enunciado.txt:83` "Postar nesta atividade: Trabalho T1"
Teste: conferir no portal que o upload foi feito na atividade "Trabalho T1", não em outra.

**R37** — Formato do arquivo e responsabilidade de conferência
> `enunciado.txt:84-86` "Código fonte desenvolvido: Em formato .txt ou .cpp, é de responsabilidade do grupo verificar se o arquivo postado é o correto."
Teste: abrir o arquivo efetivamente postado depois do upload e comparar com a versão final (evitar postar rascunho/versão do João por engano).

## B. Menu

**R03** — Três opções exatas no menu
> `enunciado.txt:20-23` "O jogo deverá possuir um menu onde será possível escolher: • Jogar • Sobre • Sair"
Teste: rodar o `.exe`, checar que a tela inicial oferece exatamente Jogar/Sobre/Sair.

**R04** — Só "Sair" encerra o programa
> `enunciado.txt:28-29` "seu jogo só deve ser encerrado ao selecionar essa opção"
Teste: escolher Sair → processo termina. Em qualquer outro fluxo o processo não deve terminar sozinho.

**R05** — Toda outra opção retorna ao menu ao final de sua execução
> `enunciado.txt:29-31` "caso qualquer outra opção seja escolhida ela deve retornar ao menu no fim de sua execução"
Teste: rodar Sobre até o fim → volta ao menu. Rodar Jogar até terminar uma partida (bingo feito) → volta ao menu, não fecha o programa.

## C. Sobre

**R06** — Exibe nome de cada integrante da equipe
> `enunciado.txt:34-36` "deverão ser exibidas a equipe de desenvolvimento (o nome de cada membro da equipe)"
Teste: abrir Sobre, contar 5 nomes (consistente com R02).

**R07** — Exibe mês/ano
> `enunciado.txt:36` "o mês/ano"
Teste: abrir Sobre, conferir presença de mês/ano.

**R08** — Exibe professor/disciplina
> `enunciado.txt:36` "e o nome do professor/disciplina"
Teste: abrir Sobre, conferir nome do professor e/ou da disciplina.

## D. Jogar — geração das cartelas

**R09** — Cartela com 25 números (5 colunas × 5 linhas, 5 valores por linha)
> `enunciado.txt:13-14` "cada cartela possui cinco colunas com cinco linhas, e cada linha possui 5 valores" + `enunciado.txt:39` "cada cartela possui 25 números, sendo que cada linha possui 5 números sorteados"
Teste: jogar, contar itens impressos por cartela = 25.

**R10** — Faixa de valores por linha
> `enunciado.txt:42-46` linha1 1-15 · linha2 16-30 · linha3 31-45 · linha4 46-60 · linha5 61-75
Teste: para cada cartela exibida, checar que todo número da linha N está dentro da faixa correspondente.

**R11** — 5 cartelas por execução
> `enunciado.txt:47` "Deve-se gerar então 5 cartelas randomicamente"
Teste: jogar, contar cartelas distintas exibidas = 5.

**R12** — Novo sorteio a cada nova execução (sorteio diferenciado entre rodadas do programa)
> `enunciado.txt:48` "A cada nova execução deverá haver um novo sorteio com valores diferenciados"
Teste: rodar o `.exe` duas vezes (processos separados), Jogar nas duas, comparar cartelas — devem diferir. Se a seed for fixa ou ausente (`srand` fixo ou não chamado), este requisito falha.

**R13** — Cartela tem número de identificação
> `enunciado.txt:49` "Cada cartela terá um número de identificação e um nome"
Teste: cada cartela exibida mostra um ID distinto.

**R14** — Cartela tem nome (associado ao jogador)
> `enunciado.txt:49` (mesma linha) + `enunciado.txt:66` "deve-se solicitar o nome do jogador de cada cartela"
Teste: nome digitado no início aparece atrelado à cartela correspondente na exibição.

**R15** — Sem valores repetidos na mesma linha
> `enunciado.txt:50` "Não pode haver valores repetidos nas linhas"
Teste: inspecionar cada linha de cada cartela, nenhum valor duplicado dentro da própria linha.

**R16** — Sem cartelas repetidas entre as 5 geradas
> `enunciado.txt:51` "Não pode haver cartelas repetidas"
Teste: comparar as 5 cartelas da mesma execução — nenhuma pode ter o mesmo conjunto/disposição de números de outra. Rodar várias vezes (dado o range fixo por linha, a colisão exata é rara mas deve ser tratada no código).

**R17** — Cartelas ficam visíveis na tela durante toda a execução
> `enunciado.txt:52` "Exibir na tela as cartelas geradas durante toda a execução."
Teste: jogar várias rodadas de sorteio (apertando Enter repetidas vezes); as 5 cartelas continuam impressas a cada rodada, não somem após a geração inicial.

**R18** — Valores já sorteados marcados com cor diferenciada nas cartelas
> `enunciado.txt:53` "Marcar os valores já sorteados nas cartelas com cores diferenciadas" (reforçado em `enunciado.txt:60-61`)
Teste: sortear um número presente em alguma cartela; conferir mudança visual de cor nesse número dentro da cartela.

**R19** — Linhas das cartelas em ordem crescente
> `enunciado.txt:54` "As linhas devem estar em ordem crescente"
Teste: cada linha de cada cartela, os 5 números aparecem em ordem crescente da esquerda para a direita.

## E. Jogar — sorteio dos números do bingo

**R20** — Sorteio no intervalo 1 a 75
> `enunciado.txt:56` "Serão sorteados valores de 1 a 75"
Teste: rodar até o fim, todo número sorteado exibido está entre 1 e 75.

**R21** — Sem repetição de valor sorteado
> `enunciado.txt:56-59` "os valores não devem ser repetidos [...] se foi sorteado o número 2, o mesmo não pode ser sorteado uma segunda vez"
Teste: listar todos os sorteados de uma partida, checar ausência de duplicata.

**R22** — Valores sorteados exibidos em ordem crescente na tela
> `enunciado.txt:57` "devem ser exibidos na tela em ordem crescente"
Teste: a lista/exibição de sorteados na tela aparece ordenada numericamente, não na ordem cronológica do sorteio.

**R23** — Valor recém-sorteado destacado com cor diferente
> `enunciado.txt:57-58` "destacando o atual valor sorteado com uma cor diferente"
Teste: a cada rodada, o número que acabou de sair aparece com cor distinta dos demais já sorteados.

**R24** — Lista de sorteados permanece visível durante todo o sorteio
> `enunciado.txt:60-61` "os valores sorteados também devem ser exibidos na tela durante todo o sorteio"
Teste: jogar várias rodadas; a lista cumulativa de sorteados cresce e continua toda visível (não é substituída só pelo último número).

**R25** — Sorteio para quando algum jogador faz bingo
> `enunciado.txt:59` "o sorteio para quando um dos jogadores fizer 'bingo'"
Teste: jogar até uma cartela completar os 25 números; o programa não pede mais Enter para sortear.

**R26** — Cada sorteio só ocorre ao apertar Enter
> `enunciado.txt:63` "Cada sorteio só deve ser realizado ao selecionar a tecla 'enter'"
Teste: conferir que o programa pausa aguardando Enter antes de cada número novo (não há loop automático por tempo nem sorteio em lote).

**R27** — Valor sorteado destacado na cartela no momento do sorteio
> `enunciado.txt:63-64` "deve-se destacar o valor sorteado na cartela"
Teste: a cada Enter, se o número sorteado existe em alguma cartela, ele aparece destacado imediatamente naquela rodada (não só ao final).

## F. Início e fim da partida

**R28** — Solicita nome do jogador de cada cartela ao iniciar
> `enunciado.txt:66` "Ao iniciar o jogo, deve-se solicitar o nome do jogador de cada cartela."
Teste: escolher Jogar, contar 5 prompts de nome (um por cartela) antes do sorteio começar.

**R29** — Ao final exibe nome do jogador e número da cartela vencedora
> `enunciado.txt:66-67` "Ao final, deve ser exibido o nome do jogador e o número da cartela vencedora."
Teste: jogar até o fim, checar que a mensagem final contém o nome digitado + o ID da cartela vencedora.

## G. Restrições técnicas obrigatórias (não testáveis só rodando — exigem leitura do código-fonte)

**R30** — Obrigatório uso de sub-rotinas com passagem de parâmetros
> `enunciado.txt:70` "Utilize sub-rotinas e passagem de parâmetros."
Teste: revisão do `.cpp` — lógica dividida em funções que recebem parâmetros (não tudo em `main()` nem uso de globais para "passar" dado entre funções, o que violaria também R32).

**R31** — Proibido `template`
> `enunciado.txt:71` "Não utilize templates"
Teste: revisão do `.cpp` — grep por `template<`/`template <`; não pode haver ocorrência. Compilar limpo não garante ausência (template pode compilar), é checagem manual.

**R32** — Proibido variável global
> `enunciado.txt:72` "Não utilize variáveis globais"
Teste: revisão do `.cpp` — nenhuma variável declarada fora de escopo de função/main.

**R33** — Proibido comando não visto na disciplina
> `enunciado.txt:72` "e nenhum comando ainda não abordado na disciplina."
Teste: não verificável sozinho — precisa confrontar com o conteúdo/aulas efetivamente dadas na disciplina (ex.: `auto`, lambdas, STL avançada, `vector`, etc., só valem se já vistos em aula). Requisito de julgamento humano, o Bernardo/grupo precisa confirmar o que foi ensinado.

## H. Defesa oral

**R34** — Defesa obrigatória, com questionamentos, feita pelos integrantes na aula
> `enunciado.txt:76-78` "Durante a defesa serão realizados questionamento sobre o trabalho realizado pelo grupo. A defesa é obrigatória e deverá ser feita pelos integrantes do grupo na aula."
Teste: logístico, não testável no código — cada integrante precisa saber explicar a parte que fez.

**R35** — Ausência → segunda chamada
> `enunciado.txt:77-79` "Se algum integrante não estiver presente durante a aula de defesa, deverá solicitar a segunda chamada de avaliação, caso o pedido seja deferido, o mesmo defenderá posteriormente em data a ser agendada com o professor."
Teste: logístico, não testável no código.

## I. Critérios de avaliação (tratados como requisitos)

**R38** — Organização e clareza do código = 20% da nota
> `enunciado.txt:90` "Organização e clareza do código = 20% da nota."
Teste: revisão de código (indentação, nomes, divisão em funções, legibilidade) — não roda no programa.

**R39** — Identificação dos autores + comentários pertinentes/oportunos = 10% da nota
> `enunciado.txt:91` "Identificação dos autores e Comentários pertinentes e oportunos no código = 10% da nota."
Teste: abrir o `.cpp`, checar cabeçalho com os 5 nomes (R02) e comentários explicativos ao longo do código (não vazio, não redundante).

**R40** — Funcionamento correto conforme a especificação = 50% da nota
> `enunciado.txt:92` "Funcionamento correto conforme a especificação = 50% da nota."
Teste: é a soma prática de R03–R29 rodando o programa — cada requisito funcional testado compõe esses 50%.

**R41** — Recursos da linguagem utilizados = 20% da nota
> `enunciado.txt:93` "Recursos da linguagem utilizados = 20% da nota."
Teste: revisão de código — uso de recursos de C++ vistos em aula (arrays/structs, funções, laços, manipulação de string/cor no console etc.), sem violar R31-R33.

**R42** — Defesa condiciona toda a nota do código; sem defesa, nota ZERO
> `enunciado.txt:94-95` "Defesa do código (Todas as notas relativas ao código dependem do desempenho na defesa, Sem a defesa o trabalho terá nota ZERO)"
Teste: logístico — garantir presença e capacidade de explicar qualquer trecho na defesa oral.

**R43** — Evidência de cópia (colegas/internet) = nota ZERO final, sem recuperação
> `enunciado.txt:97-98` "*Evidências de cópia de outras fontes (colegas, Internet) no código fonte implicam em nota ZERO como nota final deste trabalho (sem possibilidade de recuperação da nota)."
Teste: não testável por execução — risco explícito do enunciado. Observação minha (não é citação do enunciado, é leitura de risco): como existem duas versões do mesmo trabalho no grupo (a do João e a "final"), esse é o item mais sensível a checar antes de entregar — o código postado precisa ser demonstravelmente entendido/produzido pelo grupo, não uma cópia disfarçada.

---

## Requisitos "escondidos em frase solta" que exigem atenção redobrada na implementação
- `enunciado.txt:48` — sorteio precisa mudar entre execuções (não é só "sortear uma vez"; é sortear com seed variável a cada `.exe` novo) → R12.
- `enunciado.txt:52` — cartelas exibidas "durante toda a execução", não só na geração inicial → R17.
- `enunciado.txt:60-61` — lista de sorteados exibida "durante todo o sorteio", cumulativa, não só o último número → R24.
- `enunciado.txt:63` — o sorteio é **passo a passo por Enter**, não em loop automático nem tudo de uma vez → R26.
- `enunciado.txt:29-31` — o programa é um **loop de menu**: qualquer opção volta ao menu, só Sair fecha → R04/R05 (fácil de esquecer e deixar o `main()` linear, encerrando após uma partida).