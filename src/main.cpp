#include "../include/blockchain.h"
#include "../include/wallet.h"
#include <iostream>
#include <vector>

int main() {
    Blockchain test;

    // Create wallets for all participants so they have valid ECDSA Public/Private Keys
    Wallet seshy("Seshy"), bob("Bob"), amy("Amy"), zed("Zed");
    Wallet carol("Carol"), dave("Dave"), eve("Eve"), frank("Frank");
    Wallet gina("Gina"), hank("Hank"), ivy("Ivy"), jake("Jake");
    Wallet lena("Lena"), moe("Moe"), nina("Nina"), oscar("Oscar");

    // BLOCK 1
    std::vector<Transaction> b1_txs;
    b1_txs.push_back(seshy.sendMoney(bob.publicKey, 2.0, "Seshy sends 2 X to Bob"));
    b1_txs.push_back(bob.sendMoney(amy.publicKey, 1.5, "Bob sends 1.5 X to Amy"));
    b1_txs.push_back(amy.sendMoney(zed.publicKey, 0.75, "Amy sends 0.75 X to Zed"));
    b1_txs.push_back(zed.sendMoney(seshy.publicKey, 0.25, "Zed sends 0.25 X to Seshy"));
    test.addBlock(b1_txs);

    // BLOCK 2
    std::vector<Transaction> b2_txs;
    b2_txs.push_back(carol.sendMoney(dave.publicKey, 3.0, "Carol sends 3 X to Dave"));
    b2_txs.push_back(dave.sendMoney(eve.publicKey, 2.0, "Dave sends 2 X to Eve"));
    b2_txs.push_back(eve.sendMoney(frank.publicKey, 1.0, "Eve sends 1 X to Frank"));
    b2_txs.push_back(frank.sendMoney(carol.publicKey, 0.5, "Frank sends 0.5 X to Carol"));
    test.addBlock(b2_txs);

    // BLOCK 3
    std::vector<Transaction> b3_txs;
    b3_txs.push_back(gina.sendMoney(hank.publicKey, 4.0, "Gina sends 4 X to Hank"));
    b3_txs.push_back(hank.sendMoney(ivy.publicKey, 2.5, "Hank sends 2.5 X to Ivy"));
    b3_txs.push_back(ivy.sendMoney(jake.publicKey, 1.25, "Ivy sends 1.25 X to Jake"));
    b3_txs.push_back(jake.sendMoney(gina.publicKey, 0.75, "Jake sends 0.75 X to Gina"));
    test.addBlock(b3_txs);

    // BLOCK 4
    std::vector<Transaction> b4_txs;
    b4_txs.push_back(lena.sendMoney(moe.publicKey, 5.0, "Lena sends 5 X to Moe"));
    b4_txs.push_back(moe.sendMoney(nina.publicKey, 2.2, "Moe sends 2.2 X to Nina"));
    b4_txs.push_back(nina.sendMoney(oscar.publicKey, 1.1, "Nina sends 1.1 X to Oscar"));
    b4_txs.push_back(oscar.sendMoney(lena.publicKey, 0.9, "Oscar sends 0.9 X to Lena"));
    test.addBlock(b4_txs);

    std::cout << "Valid chain: " << (test.isChainValid() ? "True" : "False") << std::endl;
    std::cout << "Total minutes to mine (start->finish): " << test.getMiningTime() << " minutes." << std::endl;

    return 0;
}