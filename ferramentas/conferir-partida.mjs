/*
  conferir-partida.mjs - confere, na saida de UMA partida, os requisitos que o
  enunciado cobra e que dao pra checar por texto:

    - 5 cartelas na tela
    - cada linha na faixa certa (1-15, 16-30, 31-45, 46-60, 61-75)
    - linhas em ordem crescente, sem numero repetido
    - as 5 cartelas diferentes entre si
    - lista de sorteados crescente, sem repetir, dentro de 1-75
    - o vencedor anunciado tem mesmo os 25 numeros ja sorteados

  Como usar (Git Bash ou WSL; no cmd, gere o arquivo de entrada do seu jeito):

    printf '1
Ana
Bia
Caio
Duda
Eva
' > entrada.txt
    for i in $(seq 1 200); do printf '
' >> entrada.txt; done
    cat entrada.txt | ./bingo.exe > saida.txt
    node ferramentas/conferir-partida.mjs saida.txt

  Atencao: use PIPE (cat arquivo | programa), nao redirecionamento (programa < arquivo).
  Com redirecionamento, o system("mode con: ...") do main consome a entrada e o
  programa sai na hora - e artefato do teste, nao bug do jogo.

  Rodado assim 16 vezes seguidas, o jogo passou nas 16.
*/

import { readFileSync } from 'node:fs'

const ESC = String.fromCharCode(27)
const txt = readFileSync(process.argv[2], 'latin1')

// --- renderiza num grid, parando antes da limpeza de tela que fecha a partida
let corte = txt.length
const idxGanhou = txt.search(/ganhou/)
if (idxGanhou > 0) { const c = txt.indexOf(ESC + 'c', idxGanhou); if (c > 0) corte = c }
const fluxo = txt.slice(0, corte)

const LARG = 200, ALT = 60
const tela = Array.from({ length: ALT }, () => Array(LARG).fill(' '))
let l = 0, c = 0, i = 0
while (i < fluxo.length) {
  if (fluxo[i] === ESC) {
    const r = fluxo.slice(i + 1); let m
    if ((m = r.match(/^\[(\d+);(\d+)H/))) { l = +m[1] - 1; c = +m[2] - 1; i += 1 + m[0].length; continue }
    if ((m = r.match(/^\[(\d+)m/))) { i += 1 + m[0].length; continue }
    if (r[0] === 'c') { for (const x of tela) x.fill(' '); l = 0; c = 0; i += 2; continue }
    i += 1; continue
  }
  const ch = fluxo[i]
  if (ch === '\n') { l++; c = 0 }
  else if (ch === '\r') { c = 0 }
  else if (ch === '\t') { c = Math.floor(c / 8) * 8 + 8 }
  else { if (l < ALT && c < LARG) tela[l][c] = ch; c++ }
  i++
}
const linhas = tela.map((r) => r.join('').replace(/\s+$/, ''))

// --- acha as cartelas: "Cartela: N" marca o canto superior esquerdo
const cartelas = {}
for (let y = 0; y < ALT; y++) {
  for (const m of linhas[y].matchAll(/Cartela: (\d)/g)) {
    const x = m.index
    const grade = []
    for (let k = 0; k < 5; k++) {
      // a cartela vizinha comeca 40 colunas adiante; pega os 5 primeiros numeros a partir de x
      const trecho = (linhas[y + 3 + k] || '').slice(x, x + 40)
      grade.push(trecho.trim().split(/\s+/).map(Number).filter((n) => !isNaN(n)).slice(0, 5))
    }
    cartelas[m[1]] = grade
  }
}

const erros = []
const ok = []

// R11: 5 cartelas
const ids = Object.keys(cartelas).sort()
if (ids.length !== 5) erros.push('esperava 5 cartelas na tela, achei ' + ids.length + ' (' + ids + ')')
else ok.push('5 cartelas na tela')

for (const id of ids) {
  const g = cartelas[id]
  for (let li = 0; li < 5; li++) {
    const linha = g[li]
    if (linha.length !== 5) { erros.push('cartela ' + id + ' linha ' + (li + 1) + ' com ' + linha.length + ' numeros: ' + linha); continue }
    const min = 15 * li + 1, max = 15 * (li + 1)
    for (const n of linha) if (n < min || n > max) erros.push('cartela ' + id + ' linha ' + (li + 1) + ': ' + n + ' fora da faixa ' + min + '-' + max)
    for (let k = 1; k < 5; k++) if (linha[k] <= linha[k - 1]) erros.push('cartela ' + id + ' linha ' + (li + 1) + ' fora de ordem: ' + linha)
    if (new Set(linha).size !== 5) erros.push('cartela ' + id + ' linha ' + (li + 1) + ' com repetido: ' + linha)
  }
}
if (ids.length === 5 && !erros.length) ok.push('faixas por linha, ordem crescente e sem repetido: 25 linhas conferidas')

// R16: cartelas diferentes
const assinaturas = ids.map((id) => cartelas[id].flat().join(','))
if (new Set(assinaturas).size !== assinaturas.length) erros.push('ha cartelas repetidas')
else if (ids.length === 5) ok.push('as 5 cartelas sao diferentes')

// R22/R21: lista de sorteados ordenada e sem repetir
const yLista = linhas.findIndex((s) => /N\S*meros sorteados/.test(s))
if (yLista < 0) erros.push('nao achei a lista de sorteados na tela')
else {
  const nums = []
  for (let k = 1; k <= 3; k++) {
    const s = (linhas[yLista + k] || '').trim()
    if (!s || /[A-Za-z]/.test(s)) break
    nums.push(...s.split(/\s+/).map(Number).filter((n) => !isNaN(n)))
  }
  const declarado = Number((linhas[yLista].match(/\((\d+)\/75\)/) || [])[1])
  if (nums.length !== declarado) erros.push('a lista diz ' + declarado + ' numeros mas mostrou ' + nums.length)
  for (let k = 1; k < nums.length; k++) if (nums[k] <= nums[k - 1]) erros.push('lista fora de ordem em ' + nums[k - 1] + ',' + nums[k])
  if (new Set(nums).size !== nums.length) erros.push('lista com numero repetido')
  for (const n of nums) if (n < 1 || n > 75) erros.push('numero fora de 1-75 na lista: ' + n)
  if (!erros.length) ok.push('lista de sorteados: ' + nums.length + ' numeros, crescente, sem repetir, dentro de 1-75')

  // R25/R29: quem ganhou tem mesmo a cartela cheia?
  // dois formatos de anuncio, dependendo da versao do jogo
  const linhaVit = linhas.find((s) => s.includes('ganhou'))
  let mv = null
  if (linhaVit) {
    let m = linhaVit.match(/A cartela (\d) do jogador (.+?) ganhou!/)
    if (m) mv = { nome: m[2].trim(), cartela: m[1] }
    else m = linhaVit.match(/(.+?) ganhou com a cartela (\d)!/)
    if (!mv && m) mv = { nome: m[1].trim(), cartela: m[2] }
    if (!mv) {
      m = linhaVit.match(/A cartela do jogador (.+?) ganhou!/)
      if (m) {
        // este formato nao diz o numero da cartela: descobre pelo nome escrito na tela
        const nome = m[1].trim()
        // as cartelas ficam lado a lado na MESMA linha de tela, entao procurar o nome
        // na linha inteira casaria com a cartela errada: tem que olhar a coluna certa
        const idDono = ids.find((id) => {
          const y = linhas.findIndex((s) => s.includes('Cartela: ' + id))
          if (y < 0) return false
          const x = linhas[y].indexOf('Cartela: ' + id)
          return (linhas[y + 1] || '').slice(x, x + 40).includes(nome)
        })
        mv = { nome, cartela: idDono || null, semNumero: true }
      }
    }
  }
  if (!mv) erros.push('a partida nao anunciou vencedor')
  else {
    if (mv.semNumero) ok.push('ATENCAO: o anuncio nao diz o numero da cartela (o enunciado pede nome E numero)')
    const cart = mv.cartela ? cartelas[mv.cartela] : null
    if (!cart) erros.push('nao consegui casar o vencedor "' + mv.nome + '" com nenhuma cartela da tela')
    else {
      const faltando = cart.flat().filter((n) => !nums.includes(n))
      if (faltando.length) erros.push('cartela ' + mv.cartela + ' anunciada vencedora mas faltam sorteados: ' + faltando)
      else ok.push('vencedor: cartela ' + mv.cartela + ' com os 25 numeros sorteados, nome "' + mv.nome + '"')
    }
  }
}

console.log((erros.length ? 'FALHOU' : 'PASSOU') + ' | ' + ok.join(' · '))
if (erros.length) console.log('  ' + erros.join('\n  '))
process.exit(erros.length ? 1 : 0)
