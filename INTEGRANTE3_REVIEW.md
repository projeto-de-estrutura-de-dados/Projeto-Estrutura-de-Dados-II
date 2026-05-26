# Entrega do Integrante 3

## Escopo de auditoria

- Validacao das invariantes da AVL apos insercoes e apos a remocao de 20% dos nos.
- Validacao das invariantes da Red-Black apos insercoes e apos a remocao de 20% dos nos.
- Quantificacao do total de rotacoes executadas por cada estrutura.
- Registro dos principais findings de code review para a equipe.

## Como executar

Compile:

```powershell
& 'C:\Program Files (x86)\Falcon\MinGW\bin\g++.exe' -O2 -Wall -Wextra -pedantic integrante3_audit.cpp -o integrante3_audit.exe
```

Execute:

```powershell
.\integrante3_audit.exe
```

## Findings do code review

1. Critico: `RedBlackRouterTree::deleteRule` chama `fixDelete(root)` em vez de chamar o rebalanceamento a partir do no substituto ou do caso de double-black. Na pratica, a correcao da exclusao nao segue o algoritmo padrao da Red-Black e tende a mascarar violacoes.
2. Critico: `RedBlackRouterTree::deleteNode` faz uma remocao de BST comum, sem transportar cor do no removido nem tratar os casos especificos da Red-Black. Isso compromete black-height, recoloring e rotacoes apos delete.
3. Alto: a validacao anterior da Red-Black era incompleta, porque conferia raiz preta e conflito vermelho-vermelho, mas nao verificava black-height, ordem BST nem consistencia de `parent`.
4. Medio: a validacao anterior da AVL so verificava fator de balanceamento. Um no poderia manter `height` incorreta e ainda assim passar na auditoria.

## Resultado esperado da auditoria

- Se a implementacao da AVL estiver correta, ela deve continuar valida apos a carga ordenada e apos remover 20% das regras.
- A Red-Black deve ser observada com cuidado especial na delecao. Caso a auditoria falhe, isso confirma um problema real de rebalanceamento e nao um falso positivo do teste.
