# Anotações
- Permitir seed

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