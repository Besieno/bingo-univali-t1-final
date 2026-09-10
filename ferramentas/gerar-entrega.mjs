/*
  gerar-entrega.mjs  -  junta os arquivos de src/ num unico .cpp para postar no AVA.
  O professor pede "codigo fonte em .txt ou .cpp": um arquivo so.
  A gente estuda e mexe em src/ (um arquivo por parte); este script produz
  entrega/bingo.cpp, que e o MESMO codigo, em um arquivo, na ordem que compila.

  Uso:
    node ferramentas/gerar-entrega.mjs            gera entrega/bingo.cpp
    node ferramentas/gerar-entrega.mjs verificar  so avisa se entrega/ esta desatualizada
*/

import { readFileSync, writeFileSync, existsSync } from 'node:fs'
import { join, dirname } from 'node:path'
import { fileURLToPath } from 'node:url'

const RAIZ = join(dirname(fileURLToPath(import.meta.url)), '..')

// Ordem de dependencia: cada funcao precisa aparecer depois do que ela chama.
const ORDEM = ['tela', 'cartela', 'sorteio', 'vitoria', 'exibicao', 'jogadores', 'jogo', 'menu', 'main']

const CABECALHO = `/*
    Trabalho M1 - BINGO
    Universidade do Vale do Itajaí - Escola Politécnica
    Algoritmos e Programação II (22817)
    Professor: Rafael Ballotin Martins
    Setembro de 2026

    Equipe de desenvolvimento:
        Bernardo Sieno
        Henrique Dorow
        João Vitor Silva da Cruz
        Julio Cesar Manabe Padilha
        Nicolas do Vale Mezencio

    Este arquivo foi montado a partir da pasta src/, onde o mesmo codigo
    esta separado em um arquivo por parte do jogo (tela, cartela, sorteio,
    exibicao, jogadores, vitoria, jogo, menu). O conteudo das sub-rotinas
    e identico: aqui elas aparecem todas juntas, na ordem em que compilam.
*/
`

const NOMES = {
  tela: 'TELA - posicionar o cursor e escolher a cor (do codefun_GDB.h do professor)',
  cartela: 'CARTELA - gerar a cartela: faixa por linha, sem repetir, em ordem crescente',
  sorteio: 'SORTEIO - sortear de 1 a 75 sem repetir e guardar em ordem crescente',
  vitoria: 'VITORIA - a cartela esta completa?',
  exibicao: 'EXIBICAO - desenhar as cartelas, o numero da vez e a lista de sorteados',
  jogadores: 'JOGADORES - o nome do dono de cada cartela',
  jogo: 'JOGO - a partida: gera, pergunta, desenha, sorteia e confere o vencedor',
  menu: 'MENU - Jogar / Sobre / Sair',
  main: 'MAIN - a largada',
}

function lerDefines(arquivo) {
  return readFileSync(join(RAIZ, 'src', arquivo), 'utf8')
    .split(/\r?\n/)
    .filter((l) => /^#define/.test(l) && !/_H\b/.test(l))
}

function corpo(modulo) {
  const txt = readFileSync(join(RAIZ, 'src', modulo + '.cpp'), 'utf8')
  const linhas = txt.split(/\r?\n/)
  const fora = []
  const includes = []
  let emCabecalho = linhas[0] && linhas[0].startsWith('/*')

  for (const l of linhas) {
    if (emCabecalho) {                       // o comentario de topo do arquivo vira o titulo da secao
      if (l.includes('*/')) emCabecalho = false
      continue
    }
    const inc = l.match(/^#include\s+<([^>]+)>/)
    if (inc) { includes.push(inc[1]); continue }
    if (/^#include\s+"/.test(l)) continue    // include local nao faz sentido em arquivo unico
    if (/^using namespace std;/.test(l)) continue
    fora.push(l)
  }
  return { includes, texto: fora.join('\n').replace(/^\n+/, '').replace(/\n+$/, '') }
}

const partes = ORDEM.map((m) => ({ modulo: m, ...corpo(m) }))
const includes = [...new Set(partes.flatMap((p) => p.includes))]
const ordemIncludes = ['iostream', 'string', 'time.h', 'stdlib.h']
includes.sort((a, b) => {
  const ia = ordemIncludes.indexOf(a), ib = ordemIncludes.indexOf(b)
  return (ia < 0 ? 99 : ia) - (ib < 0 ? 99 : ib)
})

const bloco = (titulo) =>
  '// ' + '-'.repeat(72) + '\n// ' + titulo + '\n// ' + '-'.repeat(72) + '\n'

const saida = [
  CABECALHO,
  includes.map((i) => `#include <${i}>`).join('\n'),
  '',
  'using namespace std;',
  '',
  ...lerDefines('comum.h').map((d) => d),
  '',
  '// Códigos de cor no mesmo padrão do codefun.h que o professor passou',
  ...lerDefines('tela.h'),
  '',
  ...partes.map((p) => bloco(NOMES[p.modulo]) + '\n' + p.texto + '\n'),
].join('\n')

const destino = join(RAIZ, 'entrega', 'bingo.cpp')
const modo = process.argv[2]

if (modo === 'verificar') {
  const atual = existsSync(destino) ? readFileSync(destino, 'utf8') : ''
  if (atual.replace(/\r\n/g, '\n') === saida.replace(/\r\n/g, '\n')) {
    console.log('OK: entrega/bingo.cpp esta igual ao que src/ produz.')
    process.exit(0)
  }
  console.log('DESATUALIZADO: src/ mudou depois da ultima geracao.')
  console.log('Rode:  node ferramentas/gerar-entrega.mjs')
  process.exit(1)
}

writeFileSync(destino, saida, 'utf8')
console.log('gerado: entrega/bingo.cpp  (' + saida.split('\n').length + ' linhas, a partir de ' + ORDEM.length + ' arquivos de src/)')
