# Anotações
- Permitir seed

## Próximos passos (TODO)
- As 3 `Frontier*` (`FrontierFila`, `FrontierPilha`, `FrontierPrioridade`) —
  wrapping mecânico de `std::queue`/`std::stack`/`std::priority_queue`; a
  única pegadinha é o `ComparadorF` (min-heap: `a->f() > b->f()`).
- `BuscaGenerica.cpp`: o laço genérico exigido pelo enunciado (requisito
  crítico, -6 pts se BFS/IDDFS/A* não compartilharem literalmente esta
  função). Também é o lugar natural pra decidir e implementar a limpeza
  (`delete`) dos `NoBusca*` alocados por `sucessoraCubo`/`sucessoraComHeuristica`
  — ninguém libera esses nós ainda.
- `BFS.cpp`/`IDDFS.cpp`/`AEstrela.cpp`: wiring fino (montar `Frontier` +
  `visitados` certos e chamar `buscaGenerica`).
- `FactoryAlgoritmo.cpp`, `Controller.cpp`, `VisualizadorTerminal.cpp`: MVP
  jogável em terminal.
- Permitir seed no embaralhar (item já citado no topo) — decidir se vira uma
  função livre tipo `embaralhar(int nMovimentos, unsigned seed)`, reusável
  tanto pelo `Controller::tratarEmbaralhar` quanto por `test_busca.cpp`.

## Progresso

### 01-09-2026
- `Movimento.hpp/.cpp`: `parseMovimento`, `toString`, `movimentoInverso`
  implementados (18 movimentos: 6 faces x 3 sentidos).
- `EstadoCubo.hpp/.cpp`: `operator==` (delega pro `operator==` do
  `std::array`), `HashEstado` (hash polinomial base 31 — só precisa ser
  consistente com `operator==`, colisão é resolvida pelo `unordered_set`),
  `estadoResolvido()` (mapeamento fixo face→cor: U=Branco, D=Amarelo,
  F=Verde, B=Azul, L=Laranja, R=Vermelho).
- Fechada a numeração dos 24 stickers e o agrupamento dos 8 cantos — ver
  `CLAUDE.md`, seção "Numeração dos stickers (decisão de implementação)".
  Pré-requisito para `Sucessora::aplicarMovimento` (tabelas de permutação por
  movimento) e `Avaliadora::heuristicaCantos`.
- Fechada a tabela de permutação HORARIO das 6 faces (1 ciclo próprio + 2
  ciclos laterais cada) — ver `CLAUDE.md`, seção "Tabela de permutação
  (aplicarMovimento)". ANTI_HORARIO/DUPLO não precisam de tabela própria:
  são o mesmo ciclo aplicado 3x/2x.
- `Sucessora.cpp::aplicarMovimento` implementado usando a tabela acima
  (revisado linha a linha, bate com o design). Falta: remover o `return 1;`
  morto depois do `switch` em `passosPara` (inofensivo, mas gera aviso de
  código inalcançável em alguns compiladores); preencher os testes de
  `test_movimentos.cpp`; implementar `sucessoraCubo`/`sucessoraComHeuristica`
  (dependem de `heuristicaCantos`, que ainda não existe em `Avaliadora.cpp`).



### 03-09-2026
em `Avaliadora.cpp`, static const EstadoCubo = resovido, calcula o cubo resolvido uma só vez, e reaproveita nas próximas, já que estadoResolvido() sempre devolve o mesmo resultado. 
(cantosIncorretos + 3)/4 é um truque de arredondamento para cima com divisão inteira, sem precisar chamar o std::ceil 
A tabela CANTOS é uma copia da contida no README.md, só reorganizada como array de structs para facilitar a iteração



### 16-09-2026
- Removido o `return 1;` morto depois do `switch` em `passosPara`
  (`Sucessora.cpp`) — item que já estava anotado como TODO.
- `test_movimentos.cpp`/`test_avaliadora.cpp` preenchidos e passando (8 test
  cases / 34 assertions) — validam `aplicarMovimento` (4x mesmo giro =
  identidade, giro+inverso = identidade, DUPLO = 2x HORARIO, nas 6 faces) e
  `Avaliadora` (objetivo/heurística no estado resolvido e após 1 movimento).
  Status detalhado em `README.md` > Verificação > Status dos testes.
- `Sucessora.cpp`: `sucessoraCubo`/`sucessoraComHeuristica` implementadas.
  Decisões:
  - As duas funções chamam uma auxiliar comum `gerarFilhos(atual, visitados,
    calcularHeuristica)` no namespace anônimo, pra não duplicar o loop de 18
    movimentos — só `calcularHeuristica` decide se preenche `filho->h`.
  - Poda do movimento inverso: só ocorre quando `atual->pai != nullptr`
    (a raiz não tem o que podar); usa `movimentoInverso` já existente em
    `Movimento.hpp`.
  - Dedup contra `visitados` só roda quando o ponteiro não é `nullptr` — IDDFS
    vai chamar passando `nullptr` de propósito (ver decisão "sem visitados
    globais" abaixo).
  - Custo sempre +1 em `profundidade`, nunca `passosPara` (ver decisão
    "custo de cada movimento é sempre 1").
  - **Pendência conhecida**: `gerarFilhos` faz `new NoBusca()` pra cada filho
    e ninguém dá `delete` ainda. Decisão adiada de propósito pra
    `BuscaGenerica` (Fase 4 do roteiro de implementação), que é quem sabe
    quando um nó deixou de ser necessário.
  - Bugs pegos só na hora de compilar (não na leitura): faltou vírgula entre
    os dois primeiros parâmetros de `gerarFilhos` (`NoBusca* atual
    std::unordered_set<...>` sem `,`), typo `unorded_set` -> `unordered_set`,
    e `filhos.pushback(...)` -> `push_back` (faltou o `_`). Lição: esses
    typos de nome de função/tipo não aparecem revisando o código a olho tão
    rápido quanto compilando — compilar cedo (mesmo sem testes prontos pra
    aquele trecho) pega isso na hora.

## Decisões importantes (heurística e busca)

Decisões que impactam diretamente como BFS/IDDFS/A* vão se comportar — documentadas
aqui pra não se perderem antes de `sucessoraCubo`/`sucessoraComHeuristica` serem
implementadas.

### Custo de cada movimento é sempre 1, independente do sentido
- **Decisão**: os 18 `Movimento`s (6 faces × 3 sentidos) custam **1** cada na busca,
  ou seja, `filho->profundidade = atual->profundidade + 1` sempre — nunca usar
  `passosPara(sentido)` (que retorna 1/2/3) como custo.
- **Motivo**: `passosPara` é só um detalhe de implementação de `aplicarMovimento`
  (quantas vezes rodar o ciclo de 90° internamente pra calcular o resultado de um
  giro anti-horário ou duplo). Fisicamente um giro anti-horário é **um** movimento
  de pulso só, igual ao horário — não são 3 movimentos.
- **Impacto se implementado errado**: se `sucessoraCubo` usasse `passosPara` como
  custo, o BFS deixaria de ser BFS de verdade (ele assume custo uniforme = 1 por
  aresta; a "camada N" passaria a misturar profundidades reais diferentes) e o A*
  ficaria inconsistente com a heurística (que conta em "número de giros", não em
  "número de rotações de 90° internas"). Era exatamente a dúvida "a IA iria
  considerar todos os movimentos de custo 1 antes dos de custo 3" — resposta: não
  pode acontecer, por isso o custo tem que ser fixado em 1 na hora de criar o
  `NoBusca` filho, nunca herdado de `passosPara`.

### ANTI_HORARIO precisa continuar sendo um movimento de primeira classe
- **Decisão**: manter os 3 sentidos (`HORARIO`, `ANTI_HORARIO`, `DUPLO`) como
  movimentos distintos e válidos pra sucessora gerar, mesmo sabendo que
  `ANTI_HORARIO` é implementado internamente como 3x o ciclo horário.
- **Motivo**: tirar `ANTI_HORARIO` do conjunto de movimentos (deixando só
  `HORARIO`/`DUPLO`) obrigaria a IA a gastar 3 movimentos pra fazer o que uma
  pessoa faz com um giro só, inflando artificialmente a profundidade da busca e
  o tamanho da solução mostrada — além de não bater com o God's Number (11 HTM)
  usado como referência de limite de scramble/IDDFS, que já conta giro
  anti-horário como 1 movimento.

### Objetivo é "cada face com 1 cor só", não "bater com as cores fixas do estadoResolvido()"
- **Decisão**: `ehEstadoObjetivo` verifica só se cada face tem os 4 stickers da
  mesma cor entre si — não compara com o mapeamento fixo de cor por face.
- **Motivo**: resolve a dúvida "tem que aceitar que qualquer cubo montado é uma
  solução, independente da orientação das faces" — como o cubo 2x2 não tem
  centros fixos (só cantos), a noção de "resolvido" já é orientação-agnóstica por
  natureza. Já implementado assim em `Avaliadora.cpp` (ver progresso 03-09-2026);
  manter esse comportamento se `ehEstadoObjetivo` for revisitado depois.
- **Atenção**: isso é diferente de `heuristicaCantos`, que **precisa** comparar
  contra um `estadoResolvido()` fixo (não importa qual configuração, só precisa
  ser sempre a mesma referência) pra fazer sentido como heurística admissível — os
  dois usam critérios diferentes de propósito de forma intencional, não é
  inconsistência.
