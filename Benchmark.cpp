#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "AVLRouterTree.h"
#include "RedBlackRouterTree.h"

using namespace std;

int main() {

    const int TOTAL_RULES = 100000;
    const int DELETE_PERCENT = 20;

    int deleteAmount =
        TOTAL_RULES * DELETE_PERCENT / 100;

    srand(42);

    vector<PacketRule> rules;

    cout << "Generating packet rules..."
         << endl;

    for (int i = 1; i <= TOTAL_RULES; i++) {

        rules.push_back(
            PacketRule(
                i,
                "192.168.0.1",
                "10.0.0.1",
                rand() % 100
            )
        );
    }

    cout << "Rules generated: "
         << rules.size()
         << endl << endl;

    AVLRouterTree avl;
    RedBlackRouterTree rb;

    ofstream file("results.csv");

    file << "Structure,"
         << "InsertTime(ns),"
         << "SearchTime(ns),"
         << "DeleteTime(ns),"
         << "Height,"
         << "Rotations,"
         << "Valid"
         << endl;

    cout << "=============================="
         << endl;

    cout << "AVL INSERT TEST"
         << endl;

    cout << "=============================="
         << endl;

    auto startAVL =
        chrono::high_resolution_clock::now();

    for (auto &rule : rules) {

        avl.insert(rule);
    }

    auto endAVL =
        chrono::high_resolution_clock::now();

    auto avlInsertTime =
        chrono::duration_cast<
            chrono::nanoseconds>(
                endAVL - startAVL
        ).count();

    cout << "AVL Insert Time: "
         << avlInsertTime
         << " ns"
         << endl;

    volatile int avlChecksum = 0;

    auto avlSearchStart =
        chrono::high_resolution_clock::now();

    for (int i = 1; i <= TOTAL_RULES; i++) {

        PacketRule* result =
            avl.search(i);

        if (result != nullptr) {

            avlChecksum += result->priority;
        }
    }

    auto avlSearchEnd =
        chrono::high_resolution_clock::now();

    auto avlSearchTime =
        chrono::duration_cast<
            chrono::nanoseconds>(
                avlSearchEnd - avlSearchStart
        ).count();

    cout << "AVL Search Time: "
         << avlSearchTime
         << " ns"
         << endl;

    auto avlDeleteStart =
        chrono::high_resolution_clock::now();

    for (int i = 1; i <= deleteAmount; i++) {

        avl.deleteRule(i);
    }

    auto avlDeleteEnd =
        chrono::high_resolution_clock::now();

    auto avlDeleteTime =
        chrono::duration_cast<
            chrono::nanoseconds>(
                avlDeleteEnd - avlDeleteStart
        ).count();

    cout << "AVL Delete Time: "
         << avlDeleteTime
         << " ns"
         << endl;

    cout << "AVL Height: "
         << avl.height()
         << endl;

    cout << "AVL Rotations: "
         << avl.getRotations()
         << endl;

    cout << "AVL Valid: ";

    if (avl.validateAVL())
        cout << "YES";
    else
        cout << "NO";

    cout << endl;

    cout << "AVL Checksum: "
         << avlChecksum
         << endl << endl;

    file << "AVL,"
         << avlInsertTime << ","
         << avlSearchTime << ","
         << avlDeleteTime << ","
         << avl.height() << ","
         << avl.getRotations() << ","
         << (avl.validateAVL() ? "YES" : "NO")
         << endl;

    cout << "=============================="
         << endl;

    cout << "RED BLACK INSERT TEST"
         << endl;

    cout << "=============================="
         << endl;

    auto startRB =
        chrono::high_resolution_clock::now();

    for (auto &rule : rules) {

        rb.insert(rule);
    }

    auto endRB =
        chrono::high_resolution_clock::now();

    auto rbInsertTime =
        chrono::duration_cast<
            chrono::nanoseconds>(
                endRB - startRB
        ).count();

    cout << "RB Insert Time: "
         << rbInsertTime
         << " ns"
         << endl;

    volatile int rbChecksum = 0;

    auto rbSearchStart =
        chrono::high_resolution_clock::now();

    for (int i = 1; i <= TOTAL_RULES; i++) {

        PacketRule* result =
            rb.search(i);

        if (result != nullptr) {

            rbChecksum += result->priority;
        }
    }

    auto rbSearchEnd =
        chrono::high_resolution_clock::now();

    auto rbSearchTime =
        chrono::duration_cast<
            chrono::nanoseconds>(
                rbSearchEnd - rbSearchStart
        ).count();

    cout << "RB Search Time: "
         << rbSearchTime
         << " ns"
         << endl;

    auto rbDeleteStart =
        chrono::high_resolution_clock::now();

    for (int i = 1; i <= deleteAmount; i++) {

        rb.deleteRule(i);
    }

    auto rbDeleteEnd =
        chrono::high_resolution_clock::now();

    auto rbDeleteTime =
        chrono::duration_cast<
            chrono::nanoseconds>(
                rbDeleteEnd - rbDeleteStart
        ).count();

    cout << "RB Delete Time: "
         << rbDeleteTime
         << " ns"
         << endl;

    cout << "RB Height: "
         << rb.height()
         << endl;

    cout << "RB Rotations: "
         << rb.getRotations()
         << endl;

    cout << "RB Valid: ";

    if (rb.validateRedBlack())
        cout << "YES";
    else
        cout << "NO";

    cout << endl;

    cout << "RB Checksum: "
         << rbChecksum
         << endl << endl;

    file << "RedBlack,"
         << rbInsertTime << ","
         << rbSearchTime << ","
         << rbDeleteTime << ","
         << rb.height() << ","
         << rb.getRotations() << ","
         << (rb.validateRedBlack() ? "YES" : "NO")
         << endl;

    file.close();

    cout << "=============================="
         << endl;

    cout << "BENCHMARK FINISHED"
         << endl;

    cout << "=============================="
         << endl;

    cout << "Results exported to results.csv"
         << endl;

    return 0;
}