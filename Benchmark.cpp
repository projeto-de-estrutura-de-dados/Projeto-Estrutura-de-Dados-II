#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "AVLRouterTree.h"
#include "RedBlackRouterTree.h"

using namespace std;

int main() {

  vector<PacketRule> rules;

  for (int i = 1; i <= 100000; i++) {

    rules.push_back(PacketRule(i, "192.168.0.1", "10.0.0.1", rand() % 100));
  }
  AVLRouterTree avl;
  RedBlackRouterTree rb;

  auto start = chrono::high_resolution_clock::now();
  auto end = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::nanoseconds>(end - start).count();

  auto stratAVL = chrono::high_resolution_clock::now();

  for (auto &rule : rules) {
    avl.insert(rule);
  }

  auto endAVL = chrono::high_resolution_clock::now();

  auto avlInsertTime =
      chrono::duration_cast<chrono::nanoseconds>(endAVL - startAVL).count();

  cout << "AVL Insert Time:" << avlInsertTime << " ns" << endl;

  auto startRB = chrono::high_resolution_clock::now();

  for (auto &rule : rules) {
    rb.insert(rule);
  }

  auto endRB = chrono::high_resolution_clock::now();

  auto searchStart = chrono::high_resolution_clock::now();

  for (int i = 1; i <= 100000; i++) {
    avl.search(i);
  }

  auto searchEnd = chrono::high_resolution_clock::now();

  int deleteAmount = 100000 * 0.2;

  for (int i = 1; i <= deleteAmount; i++) {
    avl.deleteRule(i);
  }

  avl.validateAVL();
  rb.validateRedBlack();

  cout << "AVL Height: " << avl.height() << endl;

  cout << "AVL Rotations: " << avl.getRotations() << endl;

  ofstream file("results.csv");
  file << "Estrutura,Tempo\n";
  file << "AVL," << avlInsertTime << "\n";

  file << "RedBlack," << rbInsertTime << "\n";
}
