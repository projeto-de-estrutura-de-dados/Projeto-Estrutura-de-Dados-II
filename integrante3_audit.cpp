#include "AVLRouterTree.h"
#include "RedBlackRouterTree.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::string toString(int value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

PacketRule makeRule(int id) {
    return PacketRule(
        id,
        "10.0.0." + toString((id % 254) + 1),
        "192.168.1." + toString(((id * 3) % 254) + 1),
        (id % 100) + 1
    );
}

template <typename Tree>
void insertOrdered(Tree& tree, int total) {
    for (int id = 1; id <= total; ++id) {
        tree.insert(makeRule(id));
    }
}

template <typename Tree>
void deleteTwentyPercent(Tree& tree, int total) {
    for (int id = 5; id <= total; id += 5) {
        tree.deleteRule(id);
    }
}

bool validateSearches(AVLRouterTree& avl, RedBlackRouterTree& rbt, int total, std::string& message) {
    std::vector<int> probes;
    probes.push_back(1);
    probes.push_back(total / 4);
    probes.push_back(total / 2);
    probes.push_back(total - 1);
    probes.push_back(total);

    for (std::size_t i = 0; i < probes.size(); ++i) {
        const int id = probes[i];

        if (avl.search(id) == NULL) {
            message = "AVL falhou na busca da chave " + toString(id) + ".";
            return false;
        }

        if (rbt.search(id) == NULL) {
            message = "Red-Black falhou na busca da chave " + toString(id) + ".";
            return false;
        }
    }

    return true;
}

void printMetricRow(
    const std::string& name,
    bool valid,
    int nodes,
    int height,
    int rotations,
    const std::string& notes
) {
    std::cout << std::left << std::setw(14) << name
              << std::setw(10) << (valid ? "OK" : "FALHOU")
              << std::setw(10) << nodes
              << std::setw(10) << height
              << std::setw(12) << rotations
              << notes << '\n';
}

}  // namespace

int main() {
    const int kTotalRules = 100000;
    const int kExpectedAfterDelete = kTotalRules - (kTotalRules / 5);

    AVLRouterTree avl;
    RedBlackRouterTree rbt;

    insertOrdered(avl, kTotalRules);
    insertOrdered(rbt, kTotalRules);

    std::string searchMessage;
    const bool searchesOk = validateSearches(avl, rbt, kTotalRules, searchMessage);

    std::string avlInsertError;
    std::string rbtInsertError;

    const bool avlValidAfterInsert = avl.validateAVL(avlInsertError);
    const bool rbtValidAfterInsert = rbt.validateRedBlack(rbtInsertError);

    deleteTwentyPercent(avl, kTotalRules);
    deleteTwentyPercent(rbt, kTotalRules);

    std::string avlDeleteError;
    std::string rbtDeleteError;

    const bool avlValidAfterDelete = avl.validateAVL(avlDeleteError);
    const bool rbtValidAfterDelete = rbt.validateRedBlack(rbtDeleteError);

    const bool avlNodeCountOk = avl.countNodes() == kExpectedAfterDelete;
    const bool rbtNodeCountOk = rbt.countNodes() == kExpectedAfterDelete;

    std::cout << "Auditoria do Integrante 3\n";
    std::cout << "Carga ordenada: " << kTotalRules << " regras\n";
    std::cout << "Remocao aplicada: 20% dos nos (ids multiplos de 5)\n\n";

    std::cout << std::left << std::setw(14) << "Estrutura"
              << std::setw(10) << "Status"
              << std::setw(10) << "Nos"
              << std::setw(10) << "Altura"
              << std::setw(12) << "Rotacoes"
              << "Observacao\n";
    std::cout << std::string(78, '-') << '\n';

    printMetricRow(
        "AVL insert",
        avlValidAfterInsert,
        kTotalRules,
        avl.height(),
        avl.getRotations(),
        avlValidAfterInsert ? "Invariantes preservadas apos insercao." : avlInsertError
    );

    printMetricRow(
        "RBT insert",
        rbtValidAfterInsert,
        kTotalRules,
        rbt.height(),
        rbt.getRotations(),
        rbtValidAfterInsert ? "Black-height=" + toString(rbt.blackHeight()) + "." : rbtInsertError
    );

    printMetricRow(
        "AVL delete",
        avlValidAfterDelete && avlNodeCountOk,
        avl.countNodes(),
        avl.height(),
        avl.getRotations(),
        (avlValidAfterDelete && avlNodeCountOk)
            ? "Estrutura integra apos remover 20%."
            : (avlNodeCountOk ? avlDeleteError : "Quantidade de nos incorreta apos delete.")
    );

    printMetricRow(
        "RBT delete",
        rbtValidAfterDelete && rbtNodeCountOk,
        rbt.countNodes(),
        rbt.height(),
        rbt.getRotations(),
        (rbtValidAfterDelete && rbtNodeCountOk)
            ? "Estrutura integra apos remover 20%."
            : (rbtNodeCountOk ? rbtDeleteError : "Quantidade de nos incorreta apos delete.")
    );

    std::cout << '\n';
    std::cout << "Busca de amostras: " << (searchesOk ? "OK" : "FALHOU") << '\n';

    if (!searchesOk) {
        std::cout << searchMessage << '\n';
    }

    std::cout << "Rotacoes totais AVL: " << avl.getRotations() << '\n';
    std::cout << "Rotacoes totais Red-Black: " << rbt.getRotations() << '\n';

    const bool overallSuccess = searchesOk &&
        avlValidAfterInsert &&
        rbtValidAfterInsert &&
        avlValidAfterDelete &&
        avlNodeCountOk &&
        rbtValidAfterDelete &&
        rbtNodeCountOk;

    if (!overallSuccess) {
        std::cout << "\nConclusao: a auditoria encontrou falhas estruturais que precisam de correcao.\n";
        return 1;
    }

    std::cout << "\nConclusao: ambas as estruturas passaram nos testes de auditoria definidos para o Integrante 3.\n";
    return 0;
}
