#include "blockchain.h"
#include <iostream>

int main() {
    Blockchain test;

    // create transaction vectors for each block (multiple txs per block to visualize Merkle layers)
    Transaction b1_t1{"Seshy", "Bob", 2.0, "", "Seshy sends 2 X to Bob"};
    Transaction b1_t2{"Bob", "Amy", 1.5, "", "Bob sends 1.5 X to Amy"};
    Transaction b1_t3{"Amy", "Zed", 0.75, "", "Amy sends 0.75 X to Zed"};
    Transaction b1_t4{"Zed", "Seshy", 0.25, "", "Zed sends 0.25 X to Seshy"};
    test.addBlock(std::vector<Transaction>{b1_t1, b1_t2, b1_t3, b1_t4});

    Transaction b2_t1{"Carol", "Dave", 3.0, "", "Carol sends 3 X to Dave"};
    Transaction b2_t2{"Dave", "Eve", 2.0, "", "Dave sends 2 X to Eve"};
    Transaction b2_t3{"Eve", "Frank", 1.0, "", "Eve sends 1 X to Frank"};
    Transaction b2_t4{"Frank", "Carol", 0.5, "", "Frank sends 0.5 X to Carol"};
    test.addBlock(std::vector<Transaction>{b2_t1, b2_t2, b2_t3, b2_t4});

    Transaction b3_t1{"Gina", "Hank", 4.0, "", "Gina sends 4 X to Hank"};
    Transaction b3_t2{"Hank", "Ivy", 2.5, "", "Hank sends 2.5 X to Ivy"};
    Transaction b3_t3{"Ivy", "Jake", 1.25, "", "Ivy sends 1.25 X to Jake"};
    Transaction b3_t4{"Jake", "Gina", 0.75, "", "Jake sends 0.75 X to Gina"};
    test.addBlock(std::vector<Transaction>{b3_t1, b3_t2, b3_t3, b3_t4});

    Transaction b4_t1{"Lena", "Moe", 5.0, "", "Lena sends 5 X to Moe"};
    Transaction b4_t2{"Moe", "Nina", 2.2, "", "Moe sends 2.2 X to Nina"};
    Transaction b4_t3{"Nina", "Oscar", 1.1, "", "Nina sends 1.1 X to Oscar"};
    Transaction b4_t4{"Oscar", "Lena", 0.9, "", "Oscar sends 0.9 X to Lena"};
    test.addBlock(std::vector<Transaction>{b4_t1, b4_t2, b4_t3, b4_t4});

    std::cout << "Valid chain: " << (test.isChainValid() ? "True" : "False") << std::endl;

    std::cout << "Total minutes to mine (start→finish): " << test.getMiningTime() << " minutes." << std::endl;

    return 0;
}