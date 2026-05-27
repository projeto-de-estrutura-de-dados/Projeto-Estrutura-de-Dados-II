# Projeto Estrutura de Dados II

## Introducao

Este repositorio registra a evolucao de um projeto desenvolvido na disciplina de Estrutura de Dados II, com foco na implementacao manual de estruturas de busca balanceadas.

O projeto foi construido em C++ e se concentrou em duas frentes principais:

- implementacao da arvore AVL
- implementacao da arvore Red-Black

Ao longo do desenvolvimento, o repositorio tambem recebeu ajustes de integracao entre branches, validacoes estruturais e uma etapa de auditoria tecnica para revisar a corretude das operacoes de delecao e rebalanceamento.

## Objetivos

O objetivo principal do projeto foi estudar, implementar e validar estruturas de dados balanceadas para operacoes de:

- insercao
- busca
- remocao
- rebalanceamento
- validacao de invariantes

Tambem foi adotado um modelo de trabalho em equipe com uso de branches, pull requests e code review, aproximando a atividade de uma dinamica real de desenvolvimento colaborativo.

## Estrutura do repositorio

- `PacketRule.h`: classe base com os dados da regra de roteamento.
- `AVLRouterTree.h`: implementacao da arvore AVL.
- `RedBlackRouterTree.h`: implementacao da arvore Red-Black.
- `integrante3_audit.cpp`: auditoria automatizada usada na etapa de QA.
- `INTEGRANTE3_REVIEW.md`: registro tecnico da revisao do Integrante 3.
- `README.md`: documentacao geral do projeto.

## Funcionalidades implementadas

### PacketRule

- armazenamento de `id`
- armazenamento de `sourceIp`
- armazenamento de `destinationIp`
- armazenamento de `priority`

### AVL

- insercao ordenada por chave
- busca por `id`
- remocao com rebalanceamento
- calculo de altura
- calculo de fator de balanceamento
- rotacoes simples e duplas
- validacao estrutural da arvore
- contador de rotacoes

### Red-Black

- insercao com recoloracao e rotacoes
- busca por `id`
- remocao com rebalanceamento
- tratamento de `transplant`
- validacao de raiz preta
- validacao de nos vermelhos
- validacao de `black-height`
- contador de rotacoes

### QA e auditoria tecnica

- verificacao automatizada das invariantes
- teste com carga ordenada
- remocao de 20% dos nos
- verificacao de buscas apos insercao e delecao
- apoio ao code review tecnico

## Principais dificuldades encontradas

Com base no historico dos commits e no processo de auditoria, as principais dificuldades do projeto foram:

- implementar a remocao da AVL sem quebrar altura e fator de balanceamento
- manter a coerencia entre rebalanceamento e atualizacao de ponteiros
- implementar `fixDelete` da Red-Black corretamente
- tratar casos de `sibling` nulo na delecao da Red-Black
- corrigir violacoes de `black-height` apos remover nos pretos
- validar a estrutura de forma completa, e nao apenas por buscas simples
- integrar mudancas de multiplas branches sem perder arquivos importantes como `PacketRule.h` e `AVLRouterTree.h`

## Evolucao do projeto por dia e commits

### 2026-04-29

- `5c955d2` - `isac2306` - `Initial commit`
- `d8d6e6e` - `vyctor_gabriel11` - `classe PacketRule`
- `b8c216a` - `vyctor_gabriel11` - `avl inicio`

Contribuicoes do dia:

- criacao da base do repositorio
- definicao da classe `PacketRule`
- inicio da estrutura AVL

### 2026-04-30

- `bfcc056` - `vyctor_gabriel11` - `adiciona insercao e estrutura de altura com balanceamento na AVL`

Contribuicoes do dia:

- implementacao inicial da insercao
- montagem da base para controle de altura e balanceamento na AVL

### 2026-05-04

- `561a59e` - `Mardson Varela Lima` - `Merge pull request #1 from vgt321/Cria-estrutura-base-da-AVL`

Contribuicoes do dia:

- integracao da primeira etapa da AVL ao fluxo principal do repositorio

### 2026-05-05

- `be31d49` - `vyctor_gabriel11` - `aplicacao da rotacao`
- `a9edf5e` - `vyctor_gabriel11` - `implementacao da search`

Contribuicoes do dia:

- implementacao das rotacoes da AVL
- adicao da operacao de busca

### 2026-05-06

- `159422a` - `vyctor_gabriel11` - `Implementa remocao com rebalanceamento na AVL`
- `95529c3` - `vyctor_gabriel11` - `Delete AVLRouterTree.h`
- `a41e934` - `vyctor_gabriel11` - `Delete PacketRule.h`
- `3584de5` - `isac2306` - `Update README with project details and functionalities`
- `a47b123` - `isac2306` - `Revise README with project overview and updates`

Contribuicoes do dia:

- implementacao da remocao na AVL
- reorganizacao do repositorio na branch principal
- atualizacao inicial da documentacao

### 2026-05-08

- `04828cf` - `vyctor_gabriel11` - `Adiciona validacao da AVL`

Contribuicoes do dia:

- adicao da primeira camada de validacao estrutural da AVL

### 2026-05-13

- `026fd5d` - `vyctor_gabriel11` - `Finaliza AVL com remocao validacao e altura`
- `e154028` - `vyctor_gabriel11` - `fix: mantendo AVLRouterTree.h e resolvendo conflito de delecao`
- `30d9c46` - `vyctor_gabriel11` - `packetrule fix: recuperando PacketRule necessario para o projeto`

Contribuicoes do dia:

- fechamento da etapa AVL
- resolucao de conflitos entre branches
- recuperacao de arquivos essenciais para o projeto

### 2026-05-20

- `d57c993` - `vyctor_gabriel11` - `Cria estrutura base da Red-Black`

Contribuicoes do dia:

- inicio da implementacao da arvore Red-Black

### 2026-05-26

- `09c0dec` - `vyctor_gabriel11` - `Update RedBlackRouterTree.h`
- `2cd0108` - `vyctor_gabriel11` - `Implementa insercao e balanceamento da Red-Black`
- `eafcb8c` - `vyctor_gabriel11` - `Implementa busca e remocao inicial da Red-Black`
- `952ebed` - `vyctor_gabriel11` - `Implementa fixDelete e rebalanceamento da Red-Black`
- `e997e4d` - `vyctor_gabriel11` - `Finaliza implementacao da Red-Black`
- `b49b4f5` - `isac2306` - `Add integrante 3 audit and review`
- `5e3dcdf` - `vyctor_gabriel11` - `refactor: complete Red-Black tree implementation with validation`

Contribuicoes do dia:

- evolucao completa da Red-Black
- implementacao de insercao, busca, remocao e rebalanceamento
- refatoracao da implementacao
- entrada da etapa de auditoria tecnica e QA

### 2026-05-27

- `2580816` - `vyctor_gabriel11` - `fix(redblack): repair delete balancing, null sibling cases and black-height validation`
- `735bb8f` - `vyctor_gabriel11` - `Merge pull request #2 from projeto-de-estrutura-de-dados/balanceameno-e-altura-AVL`

Contribuicoes do dia:

- correcao dos pontos mais sensiveis da delecao da Red-Black
- ajuste de `black-height`
- tratamento de `sibling` nulo
- merge da branch da AVL na `main`

## Analise do historico

O historico dos commits permite observar com clareza a progressao do trabalho:

1. o projeto comecou pela modelagem dos dados e pela AVL
2. a AVL amadureceu com insercao, busca, remocao, validacao e altura
3. depois a equipe iniciou a Red-Black, que exigiu mais iteracoes por causa da complexidade da delecao
4. a fase de QA encontrou problemas reais na remocao da Red-Black
5. os commits finais foram dedicados a corrigir esses problemas e estabilizar a estrutura

## Conclusao

Este repositorio nao apresenta apenas o resultado final do projeto, mas tambem evidencia o processo de desenvolvimento:

- construcao incremental
- correcoes tecnicas
- integracao entre branches
- code review
- auditoria estrutural

Dessa forma, o repositorio funciona tanto como entrega academica quanto como registro da evolucao tecnica e colaborativa da equipe na implementacao de estruturas de dados balanceadas.
