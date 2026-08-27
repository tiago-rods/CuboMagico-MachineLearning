mplementar um simulador de jogo de cubo mágico 2x2x2, que além de permitir que o jogador jogue, também possa mostrar a solução do problema usando IA. Deve ser desenvolvido em C ou C++

O trabalho é em grupos (grupo T1). O trabalho deverá ser entregue até o meio-dia do dia 24/09/2026 (ou 25 para a turma de sexta-feira). Sob penalidade padrão de 1 ponto por dia de atraso.

A nota será dada após arguição com o grupo (a ordem da arguição é definida pela ordem de entrega o CANVAS).

Deve ser implementado de tal maneira que fiquem evidentes:

    Estado
    Função Sucessora
    Função avaliadora

Além disso, deve ter uma interface que permita a visualização do estado atual do cubo (essa visualização pode ou não ser idêntica à estrutura utilizada para representar o Estado). A interface pode ser simples, usando o prompt de comando, mas deve ser suficiente para permitir um entendimento e manipulação do cubo pelo usuário.

O puzzle deve ser resolvido por IA de 3 maneiras:

    Busca em Largura
    Busca em Profundidade Limitada (Iterativa)
    Busca A* (escolham a heurística)

Para a implementação as buscas devem ser utilizadas estruturas de dados (podem ser usadas bibliotecas de terceiros para as estruturas de dados, na dúvida conversem comigo) e implementação iterativa (i.e. sem uso de recursão). O seguinte laço deve estar presente:

    Adicionar estado na estrutura
    Enquanto a estrutura não estiver vazia:
        Remover próximo estado da estrutura
        Avaliar estado
            SE estado final -> mostrar solução e encerrar o programa
        Adicionar estados seguintes na estrutura
    Retornar "Sem solução"

Requisitos:

    Interface gráfica conforme descrito acima;
    O laço descrito acima DEVE ser implementado de modo a não precisar ser alterado independentemente da estrutura de dados utilizada;
    O usuário deve ter a opção de jogar ou escolher qual das IAs solucionará problema;
    O programa deve indicar a quantidade de estados visitados antes de encontrar a solução;
    O programa deve mostrar o conjunto de passos que soluciona o problema de forma intuitiva.

extras: fiquem à vontade para implementar funções extras como escolha da heurística, interface 3D, etc.

Avaliação:

    Implementar e explicar, Busca em Largura e Profundidade Limitada (3pts)
    Implementar e explicar Busca A*, incluindo a heurística escolhida (4Pts)
    Penalidade por não ter o laço (-6pts)
    Interface amigável (2pt): Devemos ser capazes de ver e interagir com o jogo intuitivamente
    O jogo pode ser jogado (1pt): Todos os requisitos necessários para que o jogo funcione estão implementados.
