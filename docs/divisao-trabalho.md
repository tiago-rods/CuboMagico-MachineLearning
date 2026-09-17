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


Parte D - Tiago
Divisão
- [x] D.1 — Validar o ambiente antes de escrever qualquer render
Ligar -DWITH_OPENGL=ON e confirmar que o CMake baixa/compila o freeglut nesta máquina, com uma janela GLUT vazia (só glutCreateWindow + glutMainLoop). Isso evita repetir a dor de cabeça do doctest (FetchContent quebrando por versão de política, path com espaço, etc.) — melhor descobrir isso agora, sem nenhuma lógica de cubo no meio.

- [x] D.2 — Câmera orbital + cena estática
Camera.cpp: projeção em perspectiva (gluPerspective) + gluLookAt/glRotatef controlado por arraste do mouse (glutMotionFunc/glutMouseFunc), testado desenhando só 1 cubo parado no centro.

- [x] D.3 — Grade dos 8 cubies posicionados
Fixar a convenção de coordenadas (decisão 1 acima) e desenhar os 8 glutSolidCube nas posições certas, ainda sem cor por sticker (cor única só pra confirmar que a grade 2x2x2 está certa visualmente).

- [x] D.4 — Colorir conforme EstadoCubo
RenderCubie.cpp::desenharCubie: pra cada canto, ler os 3 índices de sticker (tabela do README), mapear Cor → RGB, e desenhar os 3 quadrados coloridos nas faces certas (decisão 2 acima). Dá pra testar visualmente com estadoResolvido() (deve aparecer com faces uniformes) e depois com um estado embaralhado manualmente.


- [x] D.5 — Entrada de comando via teclado
glutKeyboardFunc acumulando caracteres num buffer até Enter, reaproveitando o parseMovimento que já existe em Movimento.cpp (mesmo parser do terminal — não reinventa).

- [x] D.6 — Integração
FactoryVisualizador.cpp (hoje só tem // TODO, retorna nullptr) passa a instanciar VisualizadorOpenGL quando TipoView::OPENGL; VisualizadorOpenGL::lerComando usa a solução da decisão 3.

- [x] D.7 — Extra (Semana 4 do cronograma, não bloqueia nota): animar o giro da camada interpolando rotação em N frames antes de aplicar o movimento de verdade no EstadoCubo.

**Parte D concluída (17/09/2026)**, incluindo o extra D.7 e uma rodada de polimento de UX (câmera, zoom, cores, HUD de ajuda) — detalhes em `docs/anotacoes.md`. Falta só a integração final com `Controller`/`VisualizadorTerminal` quando as Pessoas A/B/C terminarem — ver lembrete sobre a tela inicial de escolha de modo (Manual/BFS/IDDFS/A*) em `docs/anotacoes.md` > "Próximos passos (TODO)".