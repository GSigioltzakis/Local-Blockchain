#include "Blockchain.h"
#include <iostream>

int main() {
    Blockchain test;

    test.addBlock("Transaction: Seshy sends 2 X to Bob");
    test.addBlock("Transaction: Bob sends 10 X to Amy");
    test.addBlock("Transaction: Amy sends 5 X to Seshy");
    test.addBlock("Transaction: Seshy sends 1 X to Bob");

    std::cout << "Valid chain: " << (test.isChainValid() ? "True" : "False") << std::endl;

    std::cout << "Seconds to mine a block: " << test.getMiningTime() << " minutes." << std::endl;

    return 0;
}