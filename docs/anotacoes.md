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



# Pontos Importantes
  Anti horario conta como 3 movimentos, verificar para deixar 1 so movimento pela IA
  Problemas: IA iria considerar todos os movimentos de 1 de custo antes do de 3, pensar sobre isso
  antihorário é necessário?
  tem que aceitar que qualquer cubo montado é uma solução, independente da orientação das faces
