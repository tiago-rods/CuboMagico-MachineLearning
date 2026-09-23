# Cubo Mágico 2x2x2 com IA (BFS / IDDFS / A*) + interface Terminal e OpenGL 3D

Trabalho de faculdade em grupo (T1). Entrega: 24/09/2026 ao meio-dia. Linguagem: C++.
Descrição original do professor em `docs/descricao-projeto.md`.

## Requisitos obrigatórios (fonte da nota)

- Estado, Função Sucessora e Função Avaliadora devem ficar **evidentes** no código.
- 3 buscas, todas **iterativas** (sem recursão): Busca em Largura (BFS), Busca em
  Profundidade Limitada Iterativa (IDDFS), Busca A* (heurística à escolha).
- **Requisito crítico (-6 pts se falhar)**: as 3 buscas devem compartilhar **literalmente
  o mesmo laço genérico**, independente da estrutura de dados usada:
  ```
  Adicionar estado inicial na estrutura
  Enquanto a estrutura não estiver vazia:
      Remover próximo estado da estrutura
      Avaliar estado
          SE estado final -> mostrar solução e encerrar
      Adicionar estados seguintes na estrutura
  Retornar "Sem solução"
  ```
- Usuário escolhe entre jogar manualmente ou pedir para uma das 3 IAs resolver.
- Mostrar quantidade de estados visitados até achar a solução.
- Mostrar a sequência de passos da solução de forma intuitiva.
- Interface amigável o suficiente para visualizar e manipular o cubo (pode ser via terminal).
- Extra (não pontuado obrigatoriamente): escolha de heurística, interface 3D, etc.

## Como compilar e rodar

### Pré-requisitos

Compilador `g++`, `cmake` e `ninja`. No Windows, o jeito recomendado é instalar os três
pelo MSYS2, no terminal **"MSYS2 UCRT64"** (não é o PowerShell comum):
```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Se for compilar num PowerShell comum (fora do terminal MSYS2 UCRT64), primeiro adicione o
toolchain ao `PATH` dessa sessão (precisa repetir toda vez que abrir um PowerShell novo):
```
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
```

### Compilar (versão terminal, sem a tela 3D)

```
cmake -S . -B build -G Ninja "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
cmake --build build --target cubo_magico
```
(a flag `CMAKE_POLICY_VERSION_MINIMUM` só é necessária com CMake >= 4.0, por causa de uma
dependência baixada automaticamente — ver detalhes na seção "Status dos testes")

### Compilar com a tela 3D (OpenGL)

```
cmake -S . -B build -G Ninja -DWITH_OPENGL=ON "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
cmake --build build --target cubo_magico
```
Depois de compilar com `-DWITH_OPENGL=ON`, copie as DLLs de runtime pro lado do `.exe`
(necessário toda vez que a pasta `build` for recriada do zero — sem isso o programa fecha
sozinho ao tentar abrir a janela 3D):
```
Copy-Item "build\_deps\freeglut-build\bin\libfreeglut.dll" "build\" -Force
Copy-Item "C:\msys64\ucrt64\bin\libstdc++-6.dll","C:\msys64\ucrt64\bin\libgcc_s_seh-1.dll","C:\msys64\ucrt64\bin\libwinpthread-1.dll" "build\" -Force
```

### Rodar

```
.\build\cubo_magico.exe
```

### Recompilar depois de mudar código

Não precisa repetir a configuração (o primeiro `cmake -S . -B build ...`) — só o passo de
build, que recompila apenas o que mudou:
```
cmake --build build --target cubo_magico
```

### Comandos dentro do programa

```
U, U', U2, D, D', L, L', L2, R, R', R2, F, F', F2, B, B', B2   gira uma face
embaralhar N [seed]        embaralha o cubo (seed opcional, pra repetir o mesmo embaralhamento)
bfs | iddfs | astar        pede pra uma das 3 IAs resolver o cubo atual
estado                     redesenha o cubo
trocar terminal|opengl     troca a interface (opengl só se compilado com -DWITH_OPENGL=ON)
sair                       encerra o programa
```

## Decisões de arquitetura do grupo

Além do exigido, o grupo optou por seguir **SOLID/POO com design patterns** e por
**planejar uma interface 3D em OpenGL desde já, em paralelo com a interface de terminal**
(não como stretch goal isolado).

### Camadas

```
App/Controller  →  depende de abstrações, nunca de implementações concretas
   ├── IAlgoritmoBusca (Strategy)  →  BFS / IDDFS / AEstrela
   │        └── usa BuscaGenerica + IFrontier (Fila/Pilha/PrioridadeFila)
   ├── Núcleo do Cubo (sem nenhuma dependência de UI ou de busca)
   │        └── EstadoCubo, Movimento, Sucessora, Avaliadora
   └── IVisualizador (Strategy/DIP)  →  VisualizadorTerminal / VisualizadorOpenGL
```

- **SRP**: cada classe uma responsabilidade (estado é só dados; sucessora só gera
  movimentos; avaliadora só julga objetivo/heurística; visualizador só desenha e
  captura input).
- **OCP**: novo algoritmo de busca = nova classe `IAlgoritmoBusca` sem tocar em
  `BuscaGenerica`; nova view (ex: web) = nova classe `IVisualizador` sem tocar no núcleo.
- **LSP**: qualquer `IFrontier` (fila/pilha/heap) e qualquer `IVisualizador`
  (terminal/OpenGL) são intercambiáveis onde a interface é usada.
- **ISP**: interfaces pequenas — `IFrontier` só tem `inserir/remover/vazia`;
  `IVisualizador` só tem `renderizar(EstadoCubo)`, `lerComando()` e
  `mostrarMensagem(string)`.
- **DIP**: `BuscaGenerica` e o `Controller` do menu dependem só de abstrações
  (`IFrontier`, `IAlgoritmoBusca`, `IVisualizador`), nunca de `FrontierFila` ou
  `VisualizadorOpenGL` diretamente — concretizações injetadas na composição (`main.cpp`).

**Design patterns usados explicitamente** (citar na arguição):
- **Strategy**: cada algoritmo de busca e cada view é uma estratégia intercambiável.
- **Bridge**: `IFrontier` desacopla o laço genérico (`BuscaGenerica`) da estrutura de
  dados concreta — é o mecanismo que cumpre o requisito do laço fixo.
- **Factory Method simples**: `criarAlgoritmo(TipoBusca)` e `criarVisualizador(TipoView)`
  isolam o `switch`/`if` de escolha do usuário num único lugar.

### Núcleo do domínio (Estado, Sucessora, Avaliadora)

- **Representação de estado**: 6 faces completas, 24 stickers (`std::array<Cor,24>`),
  **não** a redução por corner fixo — mais simples de explicar na arguição, mapeia 1:1
  com o desenho ASCII/3D. O God's Number do 2x2x2 (11 HTM) garante que BFS ingênuo
  continua viável para fins didáticos com limite de scramble razoável (6-8 movimentos
  no menu).
- **Movimentos**: 6 faces (U,D,L,R,F,B) × 3 sentidos (90° horário, 90° anti-horário,
  180°) = 18 movimentos, os mesmos usados pelo jogador e pela IA.
- **Função sucessora**: `aplicarMovimento(EstadoCubo, Movimento) -> EstadoCubo` (pura,
  tabelas de permutação de índices por face) + `sucessoraCubo(NoBusca*, visitados?) ->
  vector<NoBusca*>`, com poda de "desfazer o último movimento".
- **Função avaliadora**: `ehEstadoObjetivo(EstadoCubo) -> bool` (cada face com 1 cor só)
  e `heuristicaCantos(EstadoCubo) -> int` = `ceil(cantos_incorretos / 4.0)`. Admissível
  porque cada giro de face move exatamente 4 dos 8 cantos, logo
  `movimentos_necessários >= ceil(k/4)`.

### Numeração dos stickers (decisão de implementação)

`índice = face*4 + pos`, ordem de faces U=0,D=1,F=2,B=3,L=4,R=5. Dentro de cada
face, `pos` segue sempre a mesma convenção olhando a face de fora:
`0`=superior-esquerda, `1`=superior-direita, `2`=inferior-esquerda,
`3`=inferior-direita. Planificação em cruz (U em cima de F, D embaixo de F, L à
esquerda de F, R à direita de F, B à direita de R):

```
                +--------+
                |  0   1 |
                |  2   3 |
      +--------+--------+--------+--------+
      | 16  17 |  8   9 | 20  21 | 12  13 |
      | 18  19 | 10  11 | 22  23 | 14  15 |
      +--------+--------+--------+--------+
                |  4   5 |
                |  6   7 |
                +--------+
```

Os 8 cantos (trincas de índices que sempre giram juntos, usadas tanto por
`aplicarMovimento` quanto por `heuristicaCantos`):

| Canto | U/D | F/B | L/R |
|---|---|---|---|
| ULF | 2 | 8 | 17 |
| URF | 3 | 9 | 20 |
| DLF | 4 | 10 | 19 |
| DRF | 5 | 11 | 22 |
| ULB | 0 | 13 | 16 |
| URB | 1 | 12 | 21 |
| DLB | 6 | 15 | 18 |
| DRB | 7 | 14 | 23 |

Regra de rotação própria da face (mesma para as 6 faces, já que `pos` é sempre
"visto de fora"): HORARIO move os stickers no ciclo `0→1→3→2→0`; ANTI_HORARIO é
o ciclo inverso `0→2→3→1→0`; DUPLO troca `0↔3` e `1↔2`. O ciclo dos stickers
vizinhos (linha/coluna de cada face lateral arrastada pela camada) é definido
por face em `aplicarMovimento`.

### Tabela de permutação (aplicarMovimento)

Como cada ciclo de permutação tem 4 elementos, só é necessário derivar a
tabela HORARIO por face: `ANTI_HORARIO` é o mesmo ciclo aplicado 3 vezes
(equivale ao inverso) e `DUPLO` é aplicado 2 vezes. `aplicarMovimento` decide
apenas **quantas vezes** rodar os ciclos daquela face (1x/2x/3x) — não existem
tabelas separadas para os 18 movimentos, só para as 6 faces.

Todo giro mexe em 12 stickers: 1 ciclo de 4 na própria face (regra acima) +
2 ciclos de 4 nos stickers vizinhos (2 por cada uma das 4 faces laterais).
Cada ciclo é escrito como `(a,b,c,d)`, significando que o conteúdo anda
`a→b→c→d→a` (ou seja: `novo[b]=antigo[a]`, `novo[c]=antigo[b]`,
`novo[d]=antigo[c]`, `novo[a]=antigo[d]`).

Derivado a partir da tabela de cantos acima (regra de redirecionamento de
direção do sticker após o giro de 90°, validada contra o fato conhecido de
que R horário leva Frente→Cima):

| Face | Próprios | Lateral 1 | Lateral 2 |
|---|---|---|---|
| U | (0,1,3,2) | (8,16,12,20) | (9,17,13,21) |
| D | (4,5,7,6) | (10,22,14,18) | (19,11,23,15) |
| F | (8,9,11,10) | (2,20,5,19) | (17,3,22,4) |
| B | (12,13,15,14) | (1,16,6,23) | (21,0,18,7) |
| L | (16,17,19,18) | (0,8,4,15) | (13,2,10,6) |
| R | (20,21,23,22) | (9,1,14,5) | (3,12,7,11) |

Não existe padrão simples "posição k de uma face sempre vai pra posição k de
outra" entre faces diferentes (U por coincidência mantém isso, R não) — cada
linha da tabela foi derivada individualmente, não deduzida por simetria.

### O laço genérico

Uma única função `buscaGenerica(IFrontier&, estadoInicial, sucessora, ehObjetivo,
visitados?, limiteProfundidade=-1)` implementa exatamente o pseudocódigo do enunciado.

- **BFS**: `FrontierFila` (std::queue) + `unordered_set` de visitados,
  `limiteProfundidade=-1`.
- **A***: `FrontierPrioridade` (std::priority_queue ordenada por `f=g+h`) +
  `unordered_set` de visitados, `limiteProfundidade=-1`. Sucessora calcula `h` via
  `heuristicaCantos`. O estado é marcado como visitado na **geração** (não guarda
  o melhor `g`); o que mantém isso correto é o desempate da fila de prioridade:
  com f igual, sai primeiro o de **menor g**.
- **IDDFS**: laço externo que roda `buscaGenerica` repetidas vezes, aumentando
  `limiteProfundidade` de 1 até 11 (God's Number), **cada rodada com uma
  `FrontierPilha` nova**, sem visitados globais. A orquestração de aumento de
  profundidade fica fora de `buscaGenerica`, que nunca é alterada.

Semântica dos parâmetros (documentação completa em docs/anotacoes.md):
- `limiteProfundidade = L`: nós com `profundidade >= L` são avaliados mas **não
  expandidos**, então soluções de até L movimentos são encontradas. `-1` = sem
  limite.
- `estadosVisitados` conta os estados removidos e avaliados **naquela chamada**.
  O IDDFS precisa somar entre as rodadas.
- `buscaGenerica` é dona de todos os `NoBusca*` da chamada e libera todos antes de
  retornar.

> ⚠️ A heurística atual (`heuristicaCantos`) é **inadmissível** em relação ao
> objetivo "cubo montado em qualquer orientação", então o A* pode devolver uma
> solução até 2 movimentos acima do ótimo. Detalhes e correções possíveis em
> `docs/anotacoes.md` ("Achado pro grupo").

Verificação do requisito: nenhum `if`/`#ifdef` dentro de `BuscaGenerica.cpp` que
dependa do algoritmo — só chama `IFrontier`. Demonstrável com grep na arguição
(BFS/IDDFS/A* só aparecem em comentários, nunca em código).

### Interface: Terminal (MVP) + OpenGL 3D (em paralelo)

Ambas implementam `IVisualizador { renderizar(EstadoCubo); Comando lerComando();
mostrarMensagem(string); }`.

- **VisualizadorTerminal**: desenho ASCII em planificação de cruz (mesmo layout de
  índices do `EstadoCubo`), cores via ANSI (Windows Terminal/PS5.1+ suporta
  `ENABLE_VIRTUAL_TERMINAL_PROCESSING`). Menu: Jogar / Embaralhar / Resolver com
  BFS / IDDFS / A* / Mostrar estado / Trocar view / Sair.
- **VisualizadorOpenGL**: **freeglut + pipeline fixo** (glBegin/glRotatef/glTranslatef)
  em vez de OpenGL moderno com shaders — curva de aprendizado bem menor (sem
  VBO/VAO/GLSL), suficiente para desenhar 8 cubies coloridos (`glutSolidCube`) e
  rotacioná-los. Dependências via CMake `FetchContent`. **Implementado por completo
  (17/09/2026)**:
  - 8 cubies (cantos) em grade 3D (`VisualizadorOpenGL::desenharCena`), coloridos nas
    3 faces visíveis conforme `EstadoCubo` (`RenderCubie::desenharCubie`), plástico
    cinza escuro entre os stickers e gap mínimo entre cubies (só o suficiente pra
    evitar z-fighting nas quinas).
  - Câmera orbital (`Camera.cpp`): arraste do botão esquerdo do mouse gira a câmera
    em torno do cubo (não o cubo em si); scroll do mouse controla o zoom
    (`glutMouseWheelFunc`).
  - Comandos de movimento digitados no teclado (mesmo parser de texto do terminal:
    `U`, `U'`, `U2`...) — buffer acumulado por `glutKeyboardFunc` até Enter,
    reaproveitando `parseMovimento` de `Movimento.cpp`.
  - Animação de giro: ao reconhecer um movimento válido, a camada afetada (4 dos 8
    cantos) gira em N frames interpolados antes de `aplicarMovimento` ser chamado de
    verdade — puramente visual, não afeta o `EstadoCubo` durante a animação.
  - Texto de ajuda na tela (HUD 2D via `glutBitmapCharacter` + `gluOrtho2D`)
    explicando a sintaxe de movimento e ecoando o buffer sendo digitado em tempo
    real.
  - Janela DPI-aware (`SetProcessDPIAware` no Windows) pra evitar que o Windows
    borre a imagem em monitores com escala >100%.
  - `VisualizadorOpenGL` só lê `EstadoCubo` e desenha; toda regra continua no núcleo.
  - Testado via `PreviewMain.cpp`/executável `cubo_opengl_preview` (não entra no
    `cubo_magico` final) — ver "Como rodar o preview do OpenGL" abaixo.
- Usuário escolhe entre as duas views no início do programa (Factory Method
  `criarVisualizador`).

### Como rodar o preview do OpenGL

```
cmake -S . -B build -DWITH_OPENGL=ON -G Ninja "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
cmake --build build --target cubo_opengl_preview
```

O executável de teste é `build/cubo_opengl_preview.exe`. No Windows, ele precisa de
4 DLLs que **não** ficam automaticamente do lado do `.exe` (pendência conhecida, ver
`docs/anotacoes.md`): `libfreeglut.dll` (em `build/_deps/freeglut-build/bin/`) e
`libstdc++-6.dll`/`libgcc_s_seh-1.dll`/`libwinpthread-1.dll` (em
`C:\msys64\ucrt64\bin\`, ou equivalente da toolchain MinGW usada) — copie as 4 pra
`build/` antes de rodar, ou adicione essas pastas ao `PATH`.

**Controles**: arraste o botão esquerdo do mouse pra girar a câmera; scroll pra
zoom; digite um movimento (`U`, `R'`, `F2`...) e aperte Enter pra aplicar (com
animação).

### Estrutura de pastas / CMake

```
include/
  cubo/        EstadoCubo.h, Movimento.h, Sucessora.h, Avaliadora.h
  search/      NoBusca.h, IFrontier.h, FrontierFila.h, FrontierPilha.h, FrontierPrioridade.h,
               BuscaGenerica.h, IAlgoritmoBusca.h, BFS.h, IDDFS.h, AEstrela.h
  view/        IVisualizador.h, Comando.h
src/
  cubo/        EstadoCubo.cpp, Sucessora.cpp, Avaliadora.cpp
  search/      BuscaGenerica.cpp, BFS.cpp, IDDFS.cpp, AEstrela.cpp
  view/
    terminal/  VisualizadorTerminal.cpp
    opengl/    VisualizadorOpenGL.cpp, Camera.cpp, RenderCubie.cpp
  app/         Controller.cpp, FactoryAlgoritmo.cpp, FactoryVisualizador.cpp
  main.cpp
tests/         test_movimentos.cpp, test_avaliadora.cpp, test_busca.cpp
```

`CMakeLists.txt` raiz: `cubo_core` (cubo+search, sem dependências externas) como
biblioteca separada de `cubo_view` (terminal sempre, OpenGL via
`FetchContent(freeglut)` atrás de uma opção `WITH_OPENGL` ligada por padrão);
`tests/` linka só `cubo_core` + doctest (FetchContent) — testes não dependem de OpenGL.

## Cronograma sugerido (4 semanas)

1. **Semana 1**: núcleo (Estado/Sucessora/Avaliadora) + testes de movimentos; esqueleto
   do `BuscaGenerica`/`IFrontier` e as 3 implementações de Frontier.
2. **Semana 2**: BFS + IDDFS + A* completos e testados (comparação de estados
   visitados); `VisualizadorTerminal` completo — programa 100% funcional cobrindo os
   10 pontos obrigatórios.
3. **Semana 3**: `VisualizadorOpenGL` — janela freeglut, 8 cubies estáticos coloridos a
   partir do `EstadoCubo`, câmera orbital.
4. **Semana 4**: animação de giro de face em 3D, integração via `IVisualizador`/Factory,
   polimento, preparação da arguição (tabela comparativa BFS/IDDFS/A*, grep mostrando
   o laço único).

## Verificação

### Status da implementação (23/09/2026)

- **Parte B (algoritmos de busca) completa**: `BFS`/`IDDFS`/`AEstrela`,
  `FactoryAlgoritmo` e `sucessoraComMelhorG` integrados em `main` (branch
  `pessoaB`), com os testes comparativos de `test_busca.cpp`.
- **Parte C (Controller + terminal + `main`) completa**: `Controller` (menu
  principal, movimento com checagem de objetivo, embaralhar, resolver,
  trocar view), `VisualizadorTerminal` (desenho ASCII colorido + leitura de
  comandos) e `main.cpp`. Inclui a função `embaralhar(nMovimentos, seed)`
  (`cubo_core`, `std::mt19937` + módulo — determinística entre
  compiladores, ver seção "Como compilar e rodar").
- **`VisualizadorOpenGL::mostrarMensagem` implementado**: a tela 3D também
  passou a reconhecer todos os comandos (antes só reconhecia movimentos de
  face) e a mostrar mensagens na tela (resultado de BFS/IDDFS/A*, "cubo
  resolvido", comando inválido), reaproveitando o `desenharTexto` já
  existente. As duas views agora compartilham o mesmo reconhecedor de
  comandos (`interpretarComando`, em `src/view/InterpretadorComando.cpp`),
  pra nunca mais ficarem entendendo vocabulários diferentes.
- **Jogo completo e jogável de ponta a ponta**: `cubo_tests` com 12 casos /
  130 assertions passando; testado manualmente cobrindo jogar manualmente,
  embaralhar com/sem seed, resolver com os 3 algoritmos, trocar entre
  terminal e OpenGL, e comandos inválidos.

### Status da implementação (17/09/2026)

- **Parte D (visualizador OpenGL) completa**: D.1 a D.6 do checklist de
  `docs/divisao-trabalho.md` implementados e testados visualmente via
  `cubo_opengl_preview` (janela, câmera orbital + zoom, grade de 8 cubies
  coloridos conforme `EstadoCubo`, entrada de movimento por teclado com
  animação de giro, integração em `FactoryVisualizador` atrás de
  `#ifdef COM_OPENGL`). D.7 (extra, animação) também feito. Só falta a
  integração final quando `Controller`/`VisualizadorTerminal` (Partes B/C)
  estiverem prontos — ver lembrete de integração em `docs/anotacoes.md`.
- **Parte A (frontiers + laço genérico) completa (19/09/2026)**:
  `FrontierFila`/`FrontierPilha`/`FrontierPrioridade` e `buscaGenerica`, com
  gerenciamento de memória dos `NoBusca*` (a pendência do `delete` foi
  fechada). Verificado com um driver compilado direto no g++: 18 scrambles de
  1 a 6 movimentos nos 3 algoritmos, caso sem solução, e 0 bytes vazados. O
  contrato de uso do `buscaGenerica` (pra quem escreve BFS/IDDFS/A*) está em
  `docs/anotacoes.md`, seção "19-09-2026 (Parte A — Frontiers + laço
  genérico)".
- **Ainda pendente**:
  - Parte B: `BFS`/`IDDFS`/`AEstrela`, `FactoryAlgoritmo`, `test_busca.cpp`.
  - Parte C: `Controller`, `VisualizadorTerminal`, `main.cpp`.
  - `VisualizadorOpenGL::mostrarMensagem` ainda vazio: é por ele que a view 3D
    mostraria os passos da solução e a quantidade de estados visitados.

### Status da implementação (16/09/2026)

- **Pronto**: `EstadoCubo`, `Movimento`, `Avaliadora` (`ehEstadoObjetivo` +
  `heuristicaCantos`), `Sucessora` completo (`aplicarMovimento` +
  `sucessoraCubo`/`sucessoraComHeuristica`, com poda de busca reforçada:
  nunca repete a face do último movimento e evita reordenar pares de faces
  opostas — inspirada no solver de Sebastian Lague, ver
  `docs/anotacoes.md`).
- **Pendente**: as 3 `Frontier*`, `BuscaGenerica` (laço genérico —
  requisito crítico), `BFS`/`IDDFS`/`AEstrela`, `FactoryAlgoritmo`,
  `Controller`, `VisualizadorTerminal`. Todos ainda são stubs com `// TODO`.
- Detalhes de decisão de cada etapa ficam em `docs/anotacoes.md` (seção
  "Progresso").

### Status dos testes (16/09/2026)

- `tests/test_movimentos.cpp` e `tests/test_avaliadora.cpp` estão implementados
  e passando: **8 test cases / 34 assertions, 0 falhas**.
  - `test_movimentos.cpp`: valida `aplicarMovimento` para as 6 faces via
    `SUBCASE` — 4×mesmo giro = identidade, giro+inverso = identidade,
    `DUPLO` = 2×`HORARIO`.
  - `test_avaliadora.cpp`: estado resolvido → `ehEstadoObjetivo`=true e
    `heuristicaCantos`=0; 1 movimento → deixa de ser objetivo e
    `heuristicaCantos`=1.
  - `test_busca.cpp` ainda são `TEST_CASE` vazios — dependem de
    `sucessoraCubo`/`sucessoraComHeuristica`, `Frontier*`, `BuscaGenerica` e
    `BFS`/`IDDFS`/`AEstrela`, que ainda são stubs.
- Build local usado para validar (MSYS2 g++ + Ninja):
  ```
  cmake -S . -B build -G Ninja -DCMAKE_POLICY_VERSION_MINIMUM=3.5
  cmake --build build --target cubo_tests
  build\tests\cubo_tests.exe
  ```
  O `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` é necessário com CMake >= 4.0: o
  `CMakeLists.txt` do doctest (baixado via `FetchContent`) declara uma versão
  mínima antiga demais e o CMake recusa configurar sem essa flag.

### Checklist de verificação (plano completo)

- [x] Testes unitários (`doctest`): 4×mesmo giro = identidade; giro+inverso = identidade;
  `U2` = `U,U`; estado resolvido → objetivo=true, heurística=0.
- [x] Rodar os 3 algoritmos no mesmo scramble pequeno (1-5 movimentos, seed fixa) e checar
  que todos retornam soluções de mesmo comprimento ótimo. **Ressalva**: enquanto a
  heurística não for corrigida, o esperado é `bfs == iddfs` e `astar <= bfs + 2`
  (ver aviso em "O laço genérico") — coberto em `tests/test_busca.cpp`.
- [ ] Medir e documentar até que profundidade de scramble o BFS responde em tempo
  aceitável na máquina do grupo (base: 6-8 movimentos); usar esse número como limite
  do menu de embaralhar.
- [x] Teste manual: percorrer menu completo (jogar, embaralhar com seed fixa, resolver com
  os 3 algoritmos, trocar entre view terminal/OpenGL) sem crashes em entradas inválidas.
- [x] Checar via grep que `BuscaGenerica.cpp` não referencia símbolos de BFS/IDDFS/A* —
  evidência do requisito crítico do laço único. (Hoje os nomes só aparecem em
  comentários; no código o arquivo só usa `IFrontier`.)
