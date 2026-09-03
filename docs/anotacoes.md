# Anotações
- Permitir seed

## Próximos passos (TODO)
- Remover o `return 1;` morto depois do `switch` em `Sucessora.cpp::passosPara`.
- Preencher os testes de `test_movimentos.cpp` (4x mesmo giro = identidade,
  giro+inverso = identidade, `U2` = dois giros simples) pra validar a tabela
  de permutação de `aplicarMovimento`.
- `Avaliadora.cpp`: `ehEstadoObjetivo` e `heuristicaCantos` (usa a tabela de
  cantos do `CLAUDE.md`).
- `Sucessora.cpp`: `sucessoraCubo` e `sucessoraComHeuristica` (dependem de
  `aplicarMovimento` já pronto e de `heuristicaCantos` pra versão com A*).

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
