# Divisão de trabalho (4 integrantes)

Baseado no que já está pronto vs. pendente em `docs/anotacoes.md` e no
checklist de `README.md` > Verificação. Prazo: 24/09/2026 ao meio-dia.

## Já pronto (não precisa dividir)

`EstadoCubo`, `Movimento`, `Avaliadora` (`ehEstadoObjetivo` +
`heuristicaCantos`), `Sucessora` completo (`aplicarMovimento`,
`sucessoraCubo`, `sucessoraComHeuristica`) — e os testes desses três
(`test_movimentos.cpp`, `test_avaliadora.cpp`, 34 assertions passando).

## Pendente — tudo em `// TODO`

`FrontierFila/Pilha/Prioridade`, `BuscaGenerica`, `BFS`/`IDDFS`/`AEstrela`,
`FactoryAlgoritmo`, `Controller`, `VisualizadorTerminal`,
`FactoryVisualizador`, `main.cpp`, `VisualizadorOpenGL`/`Camera`/
`RenderCubie`, `test_busca.cpp`.

## Divisão sugerida

### Pessoa A — Estruturas de busca + laço genérico
- Arquivos: `src/search/FrontierFila.cpp`, `FrontierPilha.cpp`,
  `FrontierPrioridade.cpp`, `BuscaGenerica.cpp`.
- Por quê: é o requisito mais crítico do trabalho (**-6 pts** se
  BFS/IDDFS/A* não compartilharem literalmente o mesmo laço). Poucos
  arquivos, mas exige mais cuidado.
- Também decide: limpeza (`delete`) dos `NoBusca*` alocados por
  `sucessoraCubo`/`sucessoraComHeuristica` (pendência já anotada em
  `docs/anotacoes.md`).

### Pessoa B — Algoritmos de busca
- Arquivos: `src/search/BFS.cpp`, `IDDFS.cpp`, `AEstrela.cpp`,
  `src/app/FactoryAlgoritmo.cpp`, `tests/test_busca.cpp`.
- Por quê: consome a interface da Pessoa A (`IFrontier`/`buscaGenerica`,
  já definida nos `.hpp`) — pode começar a fiação em paralelo, só precisa
  esperar A terminar pra testar de ponta a ponta.
- Também decide: os testes comparativos (mesma solução ótima nos 3
  algoritmos; A* visita menos estados que BFS em scrambles mais
  profundos).

### Pessoa C — MVP jogável (terminal)
- Arquivos: `src/app/Controller.cpp`, `src/view/terminal/
  VisualizadorTerminal.cpp`, `src/app/FactoryVisualizador.cpp`,
  `src/main.cpp`.
- Por quê: cobre "interface amigável" (2 pts) e "jogo pode ser jogado"
  (1 pt) — é o caminho mínimo pra ter algo funcional entregável mesmo se
  a parte 3D atrasar.
- Também decide: a função de embaralhar com seed (`embaralhar(int
  nMovimentos, unsigned seed)` — TODO já anotado em
  `docs/anotacoes.md`), usada em `Controller::tratarEmbaralhar`.

### Pessoa D — Interface 3D (OpenGL)
- Arquivos: `src/view/opengl/VisualizadorOpenGL.cpp`, `Camera.cpp`,
  `RenderCubie.cpp`, + setup do `FetchContent(freeglut)` no
  `CMakeLists.txt` (flag `WITH_OPENGL`).
- Por quê: é o extra que o grupo decidiu implementar em paralelo (não é
  obrigatório pra nota). Bloco mais isolado — só depende de `EstadoCubo`
  (já pronto) — então não bloqueia ninguém se atrasar.
- Quando terminar, ajuda a preparar a arguição: grep mostrando que
  `BuscaGenerica.cpp` não referencia BFS/IDDFS/A* (evidência do laço
  único) e tabela comparativa de estados visitados entre os 3 algoritmos.

## Dependências entre blocos

- **A → B**: B escreve a fiação (montar a `Frontier` certa + chamar
  `buscaGenerica`) mesmo antes de A terminar, mas só testa de fato depois
  que A entregar.
- **A + B → C**: `Controller::tratarResolver` precisa de
  `IAlgoritmoBusca` funcionando; C pode construir menu, `tratarMovimento`
  e `tratarEmbaralhar` sem esperar.
- **D é o mais isolado**: pode rodar 100% em paralelo desde o início.

## Cronograma sugerido (prazo 24/09)

1. **Dias 1–4**: os 4 blocos em paralelo.
2. **Dias 5–6**: integração (A + B + C testando o resolver de ponta a
   ponta; D pluga a view OpenGL via `IVisualizador`/`FactoryVisualizador`).
3. **Dia 7**: testes finais, checklist de `docs/descricao-projeto.md`,
   preparar arguição.
4. **Dia 8**: buffer/polimento.
