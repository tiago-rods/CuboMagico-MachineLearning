# Decisões da Pessoa B — Algoritmos de busca

Escopo: `src/search/BFS.cpp`, `IDDFS.cpp`, `AEstrela.cpp`,
`src/app/FactoryAlgoritmo.cpp`, `tests/test_busca.cpp`.
Última atualização: 17/09/2026.

Legenda:
- **Decidido**: a Pessoa B já fechou; implementação segue isso.
- **Proposta**: sugestão da Pessoa B que toca arquivo de outra pessoa —
  precisa do "ok" do dono antes de virar decisão.
- **Em aberto**: ainda não decidido, não implementar por enquanto.

---

## Integração com a Parte A (19/09/2026)

A branch `feat/busca_e_laco` foi integrada na `main`. Rodando os 3 algoritmos
com o laço da Pessoa A (scrambles de 0 a 6 movimentos): BFS e IDDFS dão sempre
o mesmo tamanho ótimo, todo caminho devolvido resolve o cubo, e o A* visita
bem menos estados (scramble de 7 movimentos: 204.285 no BFS × 1.625 no A*).

Três ajustes feitos na integração:

1. **`BFS::resolver` não insere mais o estado inicial em `visitados`.** O
   `buscaGenerica` já insere, e tem `assert(visitados == nullptr ||
   visitados->empty())` — com a inserção anterior o programa abortava. A
   decisão "quem insere é o `resolver`" fica **revogada**: quem insere é o laço.
2. **`ComparadorF` voltou para o desempate por maior `g`** (arquivo da Pessoa
   A). Ela tinha escrito menor `g` por causa do "visitados marcados na
   geração", mas o nosso A* passa `visitados = nullptr` e usa
   `sucessoraComMelhorG`, então esse risco não existe. Medido: maior `g`
   visita ~metade dos estados (1.625 × 3.161 no scramble de 7), com a mesma
   solução. Motivo registrado no comentário do próprio arquivo.
3. **Posse dos nós**: a proposta do `filhosVivos` **não foi usada** e está
   encerrada. A Pessoa A resolveu com RAII (`DonoDosNos`): nó que não gera
   filho é liberado na hora, o resto sai no destrutor. Correto e sem
   vazamentos (ela mediu). Custo: no IDDFS a memória cresce com a árvore
   explorada (~500 MB em profundidade 7, estimativa dela), o que na prática
   limita a profundidade do IDDFS.

**A heurística inadmissível foi confirmada pela Pessoa A**, de forma
independente e com a mesma análise. Caso reprodutível do nosso driver:
scramble `L2 R R R' L' U2` → BFS e IDDFS dão 2 movimentos, A* dá 3. Enquanto
não for corrigida, o teste `bfs.size() == astar.size()` falha. As duas
correções possíveis estão em `docs/anotacoes.md` > "Achado pro grupo".

---

## Status da implementação (17/09/2026)

- **Implementado**: `FactoryAlgoritmo.cpp`, `BFS.cpp`, `IDDFS.cpp`,
  `AEstrela.cpp`, `sucessoraComMelhorG` (em `Sucessora.hpp/.cpp`).
- **Testes escritos** (`test_busca.cpp`): `sucessoraComMelhorG` (já pode
  passar) e um `TEST_CASE` por algoritmo com o caso "cubo já resolvido" — esses
  **falham até a Pessoa A entregar** `BuscaGenerica` + `Frontier*`, é esperado.
- **Mudança em `Sucessora.cpp`** (arquivo já pronto): as duas podas saíram de
  dentro de `gerarFilhos` para um helper `facePodada(atual, face)` no namespace
  anônimo, usado também por `sucessoraComMelhorG`. Comportamento idêntico
  (raiz: 18 filhos; após U: 12; após D: 15 — conferido compilando e rodando).
  Se for mexer nas podas, mexa só em `facePodada`.
- `FactoryAlgoritmo.cpp` fica no executável `cubo_magico`, não em `cubo_core`,
  então não é testável pelo `cubo_tests` — sem teste unitário de propósito.

---

## O que cada pessoa precisa saber

### Pessoa A (Frontiers + `BuscaGenerica`) — entregue em 19/09/2026
Os itens 1 a 4 abaixo chegaram implementados como estavam combinados; ficam
aqui como registro do contrato. Ver "Integração com a Parte A" no topo.

1. **Estado visitado = cada `remover()` soma 1** (feito). Remoções da limpeza
   final não contam.
2. **`caminho` montado dentro de `buscaGenerica`** (feito, `reconstruirCaminho`).
3. **`buscaGenerica` é dona de todos os `NoBusca*`** (feito, por RAII — a
   proposta do `filhosVivos` foi descartada, ver topo).
4. **Semântica de `limiteProfundidade`** (feito): nó com
   `profundidade >= limite` **é testado** como objetivo, mas **não é
   expandido**. `-1` = sem limite.
5. **Desempate no `ComparadorF`** (decidido pela B, arquivo da A; **corrigido
   na integração**): com `f` igual, sai primeiro o nó de **maior `g`**:
   ```cpp
   bool FrontierPrioridade::ComparadorF::operator()(const NoBusca* a, const NoBusca* b) const {
       if (a->f() != b->f()) return a->f() > b->f();   // min-heap por f
       return a->profundidade < b->profundidade;       // empate: maior g no topo
   }
   ```
   Por quê: `heuristicaCantos` só vale 0, 1 ou 2, então empates de `f` são a
   regra, não a exceção. Preferir maior `g` aproxima do objetivo mais rápido e
   reduz estados visitados, sem perder otimalidade (só afeta a ordem dentro do
   último nível de `f`).
6. ~~**Quem insere o estado inicial em `visitados` é o `resolver`**~~ —
   **revogado na integração**: quem insere é o `buscaGenerica`, e ele exige
   receber o set vazio (`assert`). O `resolver` só faz o `reserve`.

### Pessoa C (Controller + terminal + `main`)
1. **`test_busca.cpp` usa o seu `embaralhar(nMovimentos, seed)`** (decidido).
   Isso cria dois requisitos para essa função:
   - **Precisa estar em `cubo_core`**, não em `Controller.cpp`. Os testes
     linkam só `cubo_core` (`tests/CMakeLists.txt`); o `Controller` está no
     executável `cubo_magico` e não é visível pros testes. Sugestão: um
     `src/cubo/Embaralhar.cpp` + `include/cubo/Embaralhar.hpp` adicionados
     ao `cubo_core` no `CMakeLists.txt`.
   - **Precisa ser determinística em qualquer compilador**: mesma seed →
     mesmo embaralhamento no g++ (MSYS2) e no MSVC. Usar `std::mt19937`
     (sequência fixada pelo padrão) e escolher o movimento com
     `gerador() % 18`. **Não** usar `std::uniform_int_distribution`: o
     algoritmo dela é definido pela implementação e muda entre bibliotecas.
2. **`criarAlgoritmo(TipoBusca)` nunca retorna `nullptr`** para os 3 valores
   do enum — o `Controller` pode usar o ponteiro direto.
3. **Número de estados visitados do IDDFS é a soma de todas as rodadas** —
   se o menu exibir esse número, ele já vem somado.

### Pessoa D (OpenGL + preparação da arguição)
- Na tabela comparativa BFS × IDDFS × A*, "estados visitados" = número de
  `remover()` (IDDFS: somado em todas as rodadas). Usar os números que o
  `ResultadoBusca` devolve, sem recontar.

---

## Definições comuns aos 3 algoritmos

### 1. O que conta como "estado visitado" (decidido)
Cada `remover()` da estrutura dentro do laço soma 1 em
`ResultadoBusca::estadosVisitados`. No A*, remover uma entrada duplicada
(ver [A*](#a)) também conta — foi uma remoção real.

### 2. Onde o `caminho` é montado (proposta)
Dentro de `buscaGenerica`, no momento em que o objetivo é encontrado, por uma
função auxiliar genérica:

```cpp
// BuscaGenerica.cpp, namespace anonimo
std::vector<Movimento> reconstruirCaminho(const NoBusca* objetivo) {
    std::vector<Movimento> caminho;
    for (const NoBusca* no = objetivo; no->pai != nullptr; no = no->pai)
        caminho.push_back(no->movimentoAplicado);
    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}
```

Por quê aqui:
- É o **único lugar onde os nós ainda existem**: depois que `buscaGenerica`
  retorna, os nós já foram liberados (ver item 3). Montar o caminho no
  `resolver` exigiria devolver ponteiros vivos e empurrar a limpeza pra fora.
- É igual para os 3 algoritmos, então fica escrito uma vez só e não referencia
  BFS/IDDFS/A* (não atrapalha o grep do laço único na arguição).
- `ResultadoBusca` já tem `caminho` e `estadoFinal` — o contrato já aponta
  para isso.

### 3. Dono dos `NoBusca*` e `delete` (proposta — **superada**)
> A Pessoa A implementou com RAII (`DonoDosNos` em `BuscaGenerica.cpp`), não
> com a contagem descrita abaixo. A regra de posse do primeiro parágrafo
> continua valendo; o resto fica como registro da discussão.

**`buscaGenerica` é dona de todo nó**: o raiz (que ela mesma cria) e todo nó
que a `sucessora` devolve. Regra de posse: **o que a sucessora devolve passa a
ser da busca; o que a sucessora descarta, ela mesma nunca aloca** (ou libera
antes de retornar).

Não dá para simplesmente dar `delete` logo após o `remover()`: os filhos
guardam `pai` e o caminho é reconstruído subindo por eles. Duas formas:

**(a) Recomendada — contagem de filhos vivos.** Adiciona
`int filhosVivos = 0;` em `NoBusca`. Um nó é liberado quando já saiu da
estrutura e não tem mais filhos vivos; a liberação sobe em cascata pelos pais.

```cpp
// BuscaGenerica.cpp, namespace anonimo
void liberarSeSemFilhos(NoBusca* no) {
    while (no != nullptr && no->filhosVivos == 0) {
        NoBusca* pai = no->pai;
        delete no;
        if (pai != nullptr) --pai->filhosVivos;
        no = pai;
    }
}
```
Uso no laço: ao inserir cada filho, `++no->filhosVivos`; ao final da iteração,
`liberarSeSemFilhos(no)`. Ao sair do laço (achou ou não): montar o caminho,
liberar o nó objetivo e esvaziar a estrutura chamando `liberarSeSemFilhos` em
cada nó removido.

**(b) Alternativa simples — arena.** Guardar todo nó num
`std::vector<std::unique_ptr<NoBusca>>` local e liberar tudo no fim da chamada.

Por que (a) em vez de (b): **o IDDFS**. Com arena, cada rodada guarda todos
os nós gerados até o fim — com ~13 filhos por nó, a rodada de limite 7 gera
~62 milhões de nós (vários GB). Com a contagem, o IDDFS guarda só o ramo
atual + irmãos pendentes na pilha (memória proporcional a profundidade ×
ramificação), que é justamente a vantagem do IDDFS sobre o BFS. Para BFS e
A* as duas formas gastam praticamente o mesmo.

---

## BFS

| Decisão | Status |
|---|---|
| `FrontierFila` + `sucessoraCubo` (sem heurística) | obrigatório/óbvio |
| ~~`estadoInicial` inserido em `visitados` dentro do `resolver`~~ | revogado: quem insere é o `buscaGenerica` |
| `visitados.reserve(...)` antes da busca | decidido |

Sobre o `reserve`: o valor inicial sugerido é `1 << 20` (~1 milhão de
entradas), a ser ajustado depois de medir até que profundidade de
embaralhamento o BFS responde em tempo aceitável (item do checklist do
`README.md`). Não reservar o espaço total: sem fixar orientação, o número de
arranjos de stickers do 2x2 é ~88 milhões.

Visitados marcados na **geração** (como a `Sucessora` já faz) é o correto para
BFS: a primeira vez que um estado é gerado já é pelo caminho mais curto.

## IDDFS

| Decisão | Status |
|---|---|
| `FrontierPilha` + `sucessoraCubo`, `visitados = nullptr` | obrigatório/óbvio |
| Limite começa em **0** e vai até `profundidadeMaxima_` (11), de 1 em 1 | decidido |
| **Pilha nova a cada rodada** | decidido |
| `estadosVisitados` = **soma de todas as rodadas** | decidido |

- Começar em 0 depende da semântica de `limiteProfundidade` combinada com a
  Pessoa A (nó no limite é testado, não expandido). Limite 0 = só testa a raiz,
  o que resolve o caso "cubo já resolvido" sem tratamento especial.
- Sem `visitados` de propósito: num DFS, marcar um estado alcançado por um
  caminho longo bloquearia o caminho curto até ele e quebraria a otimalidade.
- A soma de rodadas é o custo real do algoritmo e deixa a comparação com o BFS
  justa.

## A*

| Decisão | Status |
|---|---|
| `FrontierPrioridade` por `f = g + h` + heurística | obrigatório |
| Melhor `g` por estado guardado via **função nova na `Sucessora`** | decidido |
| Desempate de `f` preferindo **maior `g`** (`ComparadorF`, arquivo da A) | decidido |
| Correção da admissibilidade da heurística | **em aberto** |

### Melhor `g` na `Sucessora`
Motivo: a `sucessora` atual descarta um estado assim que ele é gerado pela
primeira vez. No A* essa primeira geração não é necessariamente pelo menor
`g`, então o caminho melhor seria jogado fora.

Formato planejado (em `include/cubo/Sucessora.hpp` / `src/cubo/Sucessora.cpp`):

```cpp
// Como sucessoraComHeuristica, mas so gera um filho se o estado ainda nao foi
// visto ou se chega nele com g menor que o melhor ja registrado; atualiza melhorG.
std::vector<NoBusca*> sucessoraComMelhorG(NoBusca* atual,
                                          std::unordered_map<EstadoCubo, int, HashEstado>& melhorG);
```

Pontos que afetam outras pessoas:
- **Não muda `sucessoraCubo`/`sucessoraComHeuristica`** nem os testes já
  existentes; é uma função a mais no mesmo arquivo.
- A checagem em `melhorG` acontece **antes do `new`** — filho descartado nunca
  é alocado (respeita a regra de posse do item 3).
- `FuncaoSucessora` (em `BuscaGenerica.hpp`) recebe um `unordered_set*`, que
  não carrega `g`. Por isso o `AEstrela::resolver` passa uma lambda fina que
  captura o `melhorG` e chama `buscaGenerica` com `visitados = nullptr`.
  **`BuscaGenerica.hpp` não precisa mudar.**
- Um estado pode ficar duas vezes na fila (o `std::priority_queue` não tem
  "diminuir prioridade"). A entrada antiga sai depois, conta como visitada, e
  seus filhos são filtrados pelo `melhorG`. É o comportamento esperado.

### Heurística — em aberto
`heuristicaCantos` compara com uma orientação fixa, mas `ehEstadoObjetivo`
aceita qualquer orientação (ex.: após `U D'` o cubo é objetivo e `h = 2`).
Isso a torna não admissível. Correção será decidida em grupo depois; não mexer
em `Avaliadora.cpp` por conta disso até lá.

## FactoryAlgoritmo (decidido)
`switch` sobre `TipoBusca` **sem `default`**: se alguém adicionar um valor ao
enum e esquecer de tratar aqui, o compilador avisa (`-Wswitch`). Um
`return nullptr;` depois do `switch` só existe para silenciar o aviso de "fim
de função sem retorno" — nunca é alcançado para valores válidos.

## `tests/test_busca.cpp`

| Decisão | Status |
|---|---|
| Embaralhamentos gerados pelo `embaralhar(n, seed)` da Pessoa C, seed fixa | decidido |
| **Um `TEST_CASE` por algoritmo** | decidido |
| A* × BFS em estados visitados: `<=` ou `<` | **em aberto** |

- Cada `TEST_CASE` de algoritmo cobre: cubo já resolvido (caminho vazio),
  1 movimento, e "aplicar o `caminho` no estado inicial leva a
  `ehEstadoObjetivo`".
- As comparações entre algoritmos (mesmo tamanho de solução; A* × BFS em
  estados visitados) são inerentemente entre os 3, então ficam em
  `TEST_CASE`s comparativos próprios.
- Enquanto o `embaralhar` da Pessoa C não existir, os testes compilam mas não
  podem ser escritos contra ele — ver requisitos da função na seção da
  Pessoa C.
