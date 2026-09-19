# Fontes
https://github.com/SebLague/Rubiks-Cube
https://www.youtube.com/watch?v=fy0HRViXZnE
https://www.youtube.com/watch?v=Eysf6-E3ino

# Anotações
- Permitir seed
- Fazer função de embaralhamento aleatório do cubo

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
- **LEMBRETE DE INTEGRAÇÃO**: quando `Controller`/`VisualizadorTerminal`
  (Pessoas B/C) estiverem prontos, precisa existir uma **tela inicial**
  pedindo pro usuário escolher o modo de resolução antes de começar
  (Manual / BFS / IDDFS / A*) — hoje nem `VisualizadorOpenGL` nem
  `VisualizadorTerminal` têm esse menu, e `Controller::executar()` ainda é
  só `// TODO`. Isso é requisito obrigatório do enunciado ("usuário deve ter
  a opção de jogar ou escolher qual das IAs solucionará o problema" —
  `docs/descricao-projeto.md`), não só um extra de UX.
- Copiar as DLLs de runtime pro lado do `.exe` (ver nota no `README.md` >
  "Como rodar o preview do OpenGL") deveria virar um passo automático do
  CMake (`add_custom_command(TARGET ... POST_BUILD ...)`) antes da entrega
  final, pra não depender de alguém lembrar de copiar na mão.

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
  **Em termos simples**: a partir de um estado do cubo, a função tenta os 18
  giros possíveis (6 faces × 3 sentidos) e devolve a lista dos estados
  resultantes — ou seja, "para onde dá pra ir daqui com mais um movimento".
  Duas restrições evitam trabalho inútil: (1) não gera de volta o giro que
  desfaria o último movimento aplicado, já que isso só voltaria pro estado
  anterior; (2) se uma lista de estados já visitados for passada, descarta
  qualquer resultado que já esteja nela. `sucessoraComHeuristica` faz
  exatamente o mesmo, e além disso anota em cada estado gerado uma
  estimativa de quão longe ele está do cubo resolvido (via
  `heuristicaCantos`) — é essa estimativa que a busca A* vai usar depois
  pra decidir qual estado explorar primeiro.
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
- `Sucessora.cpp::gerarFilhos`: trocada a poda antiga (só bloqueava o inverso
  exato do último movimento) pelas duas podas do `GreedySolver` do Seb Lague
  (ver seção "Inspiração externa" abaixo):
  - Novo helper `faceOposta(Face)` no namespace anônimo, mapeando os 3 pares
    (U/D, L/R, F/B).
  - Poda 1: `if(temPai && face == faceDoUltimo) continue;` — corta a face
    inteira do último movimento, não só o sentido inverso. Aplicada **antes**
    do loop de `Sentido`, por isso bloqueia os 3 sentidos de uma vez.
  - Poda 2: `if(temPai && face == faceOposta(faceDoUltimo) &&
    static_cast<int>(face) > static_cast<int>(faceDoUltimo)) continue;` —
    entre um par de faces opostas, só permite a ordem em que a face de menor
    índice no enum (`Face { U, D, L, R, F, B }`) vem primeiro.
  - `movimentoInverso` deixou de ser usado dentro de `gerarFilhos` (a poda 1
    já cobre o caso do inverso exato, e cobre mais casos); a função continua
    existindo e sendo usada pelos testes de `test_movimentos.cpp`.
  - Efeito: nó não-raiz passa de até 17 filhos gerados (18 menos o inverso)
    para até 13 (18 menos 1 face inteira de 3 movimentos, menos 1 dos 2
    movimentos restantes do par de face oposta).
  - Bugs pegos ao digitar (compilando): faltou `;` em `return filhos` e
    faltou fechar o `namespace{` (a chave da linha do `gerarFilhos` fechava
    só a função, não o namespace — sem a chave extra, `sucessoraCubo`/
    `sucessoraComHeuristica` ficariam com *linkage* interno, invisíveis pra
    outros `.cpp`). Rebuild depois do fix: `cubo_tests` compila limpo, 8
    casos / 34 assertions continuam passando (nenhum teste atual cobre
    `sucessoraCubo` diretamente, só `aplicarMovimento`).

### 17-09-2026 — Parte D (OpenGL), D.1 a D.4
- `CMakeLists.txt`: `WITH_OPENGL` + `FetchContent(freeglut)` já validados
  (D.1) — janela GLUT vazia abre certo, sem os problemas de FetchContent que
  o doctest deu. `PreviewMain.cpp` criado como executável temporário só pra
  isso (não entra no `cubo_magico` final).
- `Camera.cpp` (D.2): órbita com `gluLookAt` + arraste de mouse
  (`glutMouseFunc`/`glutMotionFunc`). Dois bugs pegos e corrigidos:
  - `arrastar`: o `std::clamp` do ângulo vertical estava sendo somado de
    volta (`anguloVertical_ += clamp(...)`) em vez de atribuído
    (`anguloVertical_ = clamp(...)`) — o ângulo vazava do range [-89,89] em
    vez de travar nele.
  - `zoom`: `std::clamp(distancia_ + delta, 3.0f, 2.0f)` tinha mínimo (3)
    maior que máximo (2) — UB. Corrigido pra `(3.0f, 12.0f)`.
- **Decisão de convenção de eixos** (D.3): `U`→`+y`, `D`→`-y`, `F`→`+z`,
  `B`→`-z`, `L`→`-x`, `R`→`+x`. Os 8 cantos ficam num array
  `posicoesCantos[8][3]` em `VisualizadorOpenGL.cpp`, na mesma ordem da
  tabela de cantos do README (`ULF, URF, DLF, DRF, ULB, URB, DLB, DRB`), com
  espaçamento de `0.52f` e cubie de tamanho `0.98f` (sobra folga visual tipo
  cubo mágico de verdade). `desenharCena()` faz um loop de 8 iterações
  (`glPushMatrix`/`glTranslatef`/`glPopMatrix`) chamando `desenharCubie`.
- **D.4** (`RenderCubie.cpp::desenharCubie`): duas tabelas locais no
  namespace anônimo — `stickersPorCanto[8][3]` (índices U/D, F/B, L/R por
  canto, copiados da tabela de cantos do README) e `sinaisPorCanto[8][3]`
  (mesmos sinais de `posicoesCantos`, precisa ficar em sincronia se um dos
  dois mudar). Corpo do cubie desenhado numa cor neutra (`glutSolidCube`) e
  os 3 stickers como `GL_QUADS` deslocados por um `eps` da superfície
  (evita z-fighting). Três bugs pegos só testando visualmente com
  `estadoResolvido()`:
  - Face F/B e face L/R estavam lendo `estado.stickers[indiceUD]` (copy-paste
    do índice errado) em vez de `indiceFB`/`indiceLR` — todas as 3 faces
    saíam com a cor do sticker U/D.
  - Quad da face F/B tinha o 4º vértice duplicado do 2º
    (`glVertex3f(raio, -raio, z)` repetido) em vez de `(-raio, raio, z)` —
    ficava degenerado (só 3 pontos), face não fechava certo.
  Depois do fix, `estadoResolvido()` mostra as 6 faces do cubo grande
  uniformes, confirmando D.3+D.4.
- **D.5** (`VisualizadorOpenGL::lerComando`): `glutKeyboardFunc` registrado
  em `inicializarJanela()`, callback `callbackTeclado` acumula caracteres em
  `bufferComando_` até `\r`/`\n`, aí chama `processarBuffer()` que tenta
  `parseMovimento(bufferComando_)` (reaproveitado de `Movimento.cpp`, sem
  reinventar parser); erro de parse (`std::invalid_argument`) vira
  `Comando{TipoComando::INVALIDO, ..., bufferComando_}` em vez de propagar a
  exceção. `lerComando()` fica chamando `glutMainLoopEvent()` em loop até a
  flag `comandoPronto_` virar `true` (mesmo padrão de espera ativa que
  `renderizar()` já usa) e devolve `comandoLido_`.
  Bug pego na revisão (ainda não testado em build): sobrou a definição
  antiga de `lerComando()` (o stub `// TODO (D.5)` que retornava
  `Comando{}`) no mesmo `.cpp` junto com a nova — duplicata que quebra a
  compilação por redefinição de função; precisa remover o stub antigo antes
  de compilar.
- **D.6** (`FactoryVisualizador.cpp`) — pontos de atenção levantados antes de
  implementar:
  - `VisualizadorOpenGL.cpp`/`Camera.cpp`/`RenderCubie.cpp` só são
    compilados na lib `cubo_view_opengl`, que só existe dentro do
    `if(WITH_OPENGL)` do `CMakeLists.txt` (linhas 38-62); o executável
    `cubo_magico` só linka essa lib e só recebe a macro `COM_OPENGL` quando
    `WITH_OPENGL=ON` (linhas 57-58).
  - Por isso `FactoryVisualizador.cpp` não pode incluir
    `VisualizadorOpenGL.hpp`/instanciar a classe incondicionalmente — sem
    `WITH_OPENGL=ON` (que é o padrão, a flag é `OFF`) dá erro de link
    (símbolos de `VisualizadorOpenGL` não existem nesse build).
  - Solução: `#ifdef COM_OPENGL` em volta do `#include` do header e em volta
    do `case TipoView::OPENGL: return std::make_unique<VisualizadorOpenGL>();`.
    Sem a macro definida, o `case` cai num fallback (`return nullptr`).
  - **Pendência pra quem for integrar (Controller)**: `criarVisualizador(TipoView::OPENGL)`
    pode devolver `nullptr` num build sem OpenGL — precisa checar antes de
    usar, ou decidir travar com mensagem de erro. Decisão de integração,
    fica fora do escopo estrito do D.6.
  - `VisualizadorTerminal` não precisa de guarda — é sempre compilado,
    independente de `WITH_OPENGL`.
- **D.6 implementado**: `FactoryVisualizador.cpp` monta o `switch(tipo)` com
  `#ifdef COM_OPENGL` em volta do `#include` e do `case TipoView::OPENGL`,
  fallback `nullptr` sem a flag, exatamente como os pontos de atenção acima
  previam. `TipoView::TERMINAL` já instancia `VisualizadorTerminal` (sem
  guarda, sempre compilado).
- Com D.6, os itens obrigatórios do checklist da Parte D (D.1 a D.6) estão
  todos feitos. Falta só o **D.7 (extra, não bloqueia nota)**: animar o giro
  da camada interpolando rotação em N frames antes de aplicar o movimento
  real no `EstadoCubo` — fica pra depois da integração/testes finais
  (Semana 4 do cronograma).
- **D.7 implementado**: `animarMovimento(Movimento)` em `VisualizadorOpenGL`,
  chamado de dentro de `processarBuffer()` assim que `parseMovimento` tem
  sucesso (antes de marcar o comando como pronto). Duas tabelas novas no
  namespace anônimo de `VisualizadorOpenGL.cpp`: `cantosPorFace[6][4]`
  (quais dos 8 cantos pertencem a cada face) e `eixoPorFace[6][3]` (eixo de
  rotação de cada face, mesma convenção de sinais do D.3). `desenharCena()`
  aplica um `glRotatef` extra (antes do `glTranslatef`, pra girar em torno
  da origem do cubo e não do canto) só nos 4 cantos da face em animação.
  24 frames, ~16ms de espera cada (`std::this_thread::sleep_for`) — dá uns
  384ms de animação por giro.
- Testado com `PreviewMain.cpp` adaptado pra chamar `lerComando()` em loop e
  aplicar `aplicarMovimento` de verdade depois da animação — build local
  travou 2x por causas fora do código:
  1. Antivírus (Avast) do ambiente de teste, resolvido com exceção de pasta
     pro projeto e pro `C:\msys64`.
  2. **DLLs de runtime ausentes** (`libfreeglut.dll`,
     `libstdc++-6.dll`/`libgcc_s_seh-1.dll`/`libwinpthread-1.dll`) — o
     `.exe` compilava mas fechava sozinho na hora de abrir (exit code
     `0xC0000135`, `STATUS_DLL_NOT_FOUND`). Nenhuma delas fica do lado do
     `.exe` automaticamente; precisam ser copiadas manualmente ou
     adicionadas via `PATH`. Isso é uma pendência real pra quando o
     `cubo_magico` final for gerado — sem alguma automação (`POST_BUILD`
     no CMake), quem baixar o repo do zero vai bater na mesma trava.
- **Polimento de UX depois do primeiro teste visual bem-sucedido**:
  - `Camera::arrastar`: sinal do delta invertido (`+=`/`+` → `-=`/`-`) —
    arraste do mouse estava girando a câmera no sentido contrário do
    esperado.
  - `SetProcessDPIAware()` (Windows, `#ifdef _WIN32`) chamado no início de
    `inicializarJanela()` — sem isso o Windows fazia bitmap-scaling do
    framebuffer do GLUT em monitores com escala de DPI > 100%, deixando a
    imagem borrada mesmo com a janela em tamanho normal.
  - Zoom nunca estava ligado a nenhum input: `Camera::zoom()` existia desde
    o D.2 mas nada chamava. Adicionado `callbackMouseRoda` +
    `glutMouseWheelFunc` pra ligar o scroll do mouse a ele.
  - Cor do plástico do cubie (`RenderCubie.cpp`) ajustada em duas rodadas de
    feedback: de quase-preto (`0.05`) pra cinza médio (`0.3`) pra melhorar a
    legibilidade, depois pra cinza escuro (`0.15`) — meio termo entre as
    duas primeiras tentativas.
  - Gap entre cubies reduzido: `meiaAresta` (tamanho do cubie) subiu de
    `0.49` pra `0.515`, quase igual ao `espacamento` de `0.52` em
    `VisualizadorOpenGL.cpp` — sobra só um fiapo pra evitar z-fighting nas
    quinas em vez do gap visível de antes.
  - Texto de ajuda 2D sobreposto à cena (`desenharAjuda`/`desenharTexto`,
    `glutBitmapCharacter` + `gluOrtho2D` sobre viewport lido via
    `glGetIntegerv(GL_VIEWPORT, ...)`, não hardcoded, pra sobreviver a
    resize): explica a sintaxe de movimento (`U`/`D`/`L`/`R`/`F`/`B`,
    `'` = anti-horário, `2` = duplo) e ecoa o buffer sendo digitado em
    tempo real (por isso `callbackTeclado` ganhou um `glutPostRedisplay()`
    a cada tecla, não só no Enter).

### 17-09-2026 (Pessoa A — Frontiers + laço genérico)
- `FrontierFila`/`FrontierPilha`/`FrontierPrioridade` implementadas (wrapping de
  `std::queue`/`std::stack`/`std::priority_queue`). `remover()` devolve
  `nullptr` se estiver vazia. As frontiers **não são donas** dos nós: nunca dão
  `delete`.
- `ComparadorF`: `a->f() > b->f()` (min-heap, porque o `priority_queue` remove o
  maior). **Empate em f → menor g primeiro.** Isso é necessário, não só
  estético: a sucessora marca os estados como visitados já na *geração*, então o
  primeiro nó que gera um estado fixa o g dele. Com `h ∈ {0,1,2}` consistente,
  desempatar por maior g deixaria um nó com f igual e g maior fechar um estado
  com g subótimo; desempatando por menor g isso não acontece.
- `BuscaGenerica.cpp`: o laço do enunciado, linha a linha (os comentários no
  código seguem o pseudocódigo). O arquivo não referencia BFS/IDDFS/A* em
  código: só `IFrontier` (dá pra mostrar com grep na arguição; aparecem só em
  comentários).
- **Pendência do `delete` fechada** (esquema escolhido: simples, sem mexer em
  `NoBusca.hpp`):
  - `buscaGenerica` aloca a raiz; um nó removido que gera **0 filhos** (poda,
    dedup ou limite de profundidade) leva `delete` na hora; um nó que gera
    filhos vai pra um vetor `pais`.
  - Invariante: todo nó está em exatamente um destes lugares: na frontier, em
    `pais`, sendo o nó atual, ou já liberado. Isso vale porque `pai` só é
    escrito na criação do filho (`Sucessora.cpp`), então um nó sem filhos não é
    referenciado por ninguém.
  - Limpeza por RAII (struct `DonoDosNos`): o destrutor drena a frontier e
    libera `pais`, tanto quando acha a solução quanto quando não acha (e também
    se houver exceção).
  - **Trade-off**: nosso IDDFS usa memória **O(b^d)**, não O(b·d) como no
    livro, porque `pais` guarda os nós internos da rodada pra reconstruir o
    caminho (estimativa ~40 MB em d=6, ~500 MB em d=7). Na prática o limite do
    IDDFS é o tempo, não a memória. Contagem de referências foi considerada e
    descartada: exigiria campo novo em `NoBusca.hpp` e não ajudaria o BFS.
  - Se alguém um dia fizer "reabrir nó"/trocar o `pai` de um nó no A*, o
    `delete` imediato vira use-after-free (está comentado no código).
- Verificado com um driver compilado direto no g++ (sem CMake nesta máquina,
  não faz parte do repo): cubo resolvido (len 0, 1 visitado, nas 3), 18
  scrambles de 1 a 6 movimentos (o caminho devolvido realmente resolve o cubo
  nos 3 algoritmos, BFS == IDDFS em comprimento), caso sem solução, e um ledger
  de `new`/`delete` confirmando **0 bytes vazados** em todos os casos.

#### Contrato do `buscaGenerica` pra quem escreve BFS/IDDFS/A* (Pessoa B)
- `limiteProfundidade`: `-1` = sem limite. Com `L >= 0`, nós com
  `profundidade >= L` são avaliados mas **não expandidos**, então soluções de
  até L movimentos são encontradas. IDDFS: laço de L = 0..`profundidadeMaxima_`.
- `visitados`: `nullptr` (IDDFS) ou um set **vazio e novo a cada chamada**
  (tem `assert`). O `buscaGenerica` já insere o estado inicial nele. Reaproveitar
  o mesmo set entre as rodadas do IDDFS faria ele responder "sem solução".
- `estadosVisitados` = estados removidos e avaliados **naquela chamada**. O IDDFS
  precisa **somar** entre as rodadas, senão mostra só a última.
- `caminho` vem na ordem raiz→objetivo. `estadoFinal` = estado objetivo quando
  acha; = estado inicial quando não acha.
- A frontier volta **vazia** depois da chamada (dá pra reusar o objeto, mas o
  `tamanho()` no fim é sempre 0).

#### ⚠️ Achado pro grupo: `heuristicaCantos` é inadmissível (A* pode sair não-ótimo)
- `ehEstadoObjetivo` aceita **qualquer** cubo montado (24 orientações, decisão
  registrada abaixo), mas `heuristicaCantos` mede distância só até o
  `estadoResolvido()` fixo. No 2x2 girar o cubo inteiro é uma sequência de giros
  de face (ex.: `U D'`), então existem estados-objetivo com h = 2.
- Verificado: `U D'` → `ehEstadoObjetivo == true` e `heuristicaCantos == 2`
  (18 pares de 2 movimentos dão isso); 1 movimento depois disso: h = 2 com
  distância real 1.
- Efeito medido no driver: BFS e IDDFS sempre batem, mas o **A* devolveu 1
  movimento a mais** em 4 dos 18 scrambles (ex.: `R' U D2` → BFS 2, A* 3). O
  teste `bfs.size() == astar.size()` planejado em `test_busca.cpp` **vai falhar**
  nesses casos.
- Correções possíveis (a decidir pelo grupo, fora da parte da Pessoa A):
  1. `heuristicaCantos` = mínimo de `ceil(k/4)` entre as 24 orientações do cubo
     resolvido. Fica admissível e consistente, mantém a decisão "qualquer cubo
     montado é solução" e reusa `aplicarMovimento`.
  2. `ehEstadoObjetivo` comparar com `estadoResolvido()` exato. É mais simples,
     mas reverte a decisão abaixo.
  - Enquanto nada disso for feito: testar `astar <= bfs + 2` e
    `bfs == iddfs`.

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

## Inspiração externa: Sebastian Lague — Rubik's Cube (C#/Unity)

Vídeo: youtube.com/watch?v=fy0HRViXZnE · Repo: github.com/SebLague/Rubiks-Cube
(cubo 3x3, C#/Unity — não dá pra copiar código direto, mas duas ideias de
design valem a pena registrar).

**O que ele fez**: estado do cubo = dois `ulong` (`EdgeState`/`CornerState`),
cada cubelet empacotado em 5 bits (ID + orientação); cada movimento é uma
tabela pré-computada de máscaras/rotações de bits (`BitShiftData`), aplicada
com shifts em vez de iterar array. `Equals`/hash viram comparação de 1-2
inteiros. Ele tem 3 solvers independentes (`GreedySolver`, `CFOPSolver`,
`DominoSolver`) e **cada um com seu próprio laço de busca** — não existe um
laço genérico compartilhado entre eles.

**O que NÃO vamos aproveitar (e por quê)**:
- Reescrever `EstadoCubo` pra bit-packing: ganho de performance real, mas é
  uma reescrita arriscada de algo que já está implementado, testado (34
  assertions) e a poucos dias do prazo — o 2x2 tem só 8 cantos, então o
  `std::array<Cor,24>` atual não é gargalo que justifique o risco agora.
- Solvers sem laço genérico compartilhado: é o oposto do que o enunciado
  pede pra gente — BFS/IDDFS/A* **precisam** compartilhar literalmente
  `buscaGenerica` (-6 pts se não compartilharem). Não seguir esse padrão dele
  aqui é intencional, não descuido.

**O que trouxemos pro nosso `gerarFilhos`/`Sucessora.cpp`** — duas podas de
busca do `GreedySolver.Search`, mais fortes que a poda antiga (que só
bloqueava o inverso exato do último movimento). **Implementadas em
16-09-2026**, ver detalhes em "Progresso":
1. **Nunca girar a mesma face duas vezes seguidas**: dois giros consecutivos
   na mesma face sempre se reduzem a um giro só (ou à identidade) — então
   gerar `mov.face == atual->movimentoAplicado.face` é sempre um caminho
   subótimo, não só quando é o inverso exato. Poda estritamente mais forte
   que a antiga (que só cortava 1 dos 3 sentidos por face; essa corta os 3).
2. **Faces opostas comutam, então só gera uma ordem**: girar U depois D dá
   o mesmo estado que D depois U, então explorar as duas ordens é trabalho
   duplicado. Resolvido descartando `mov.face == oposta(ultimaFace)` quando
   `indice(mov.face) > indice(ultimaFace)` — mantém só uma ordem canônica.
