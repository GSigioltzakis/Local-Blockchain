#include "../include/blockchain.h"
#include "../include/wallet.h"
#include <iostream>

int main() {
    Blockchain test;

    // Create wallets for all participants
    Wallet seshy("Seshy"), bob("Bob"), amy("Amy"), zed("Zed");
    Wallet carol("Carol"), dave("Dave"), eve("Eve"), frank("Frank");
    Wallet gina("Gina"), hank("Hank"), ivy("Ivy"), jake("Jake");
    Wallet lena("Lena"), moe("Moe"), nina("Nina"), oscar("Oscar");

    // Pre-fund the initial senders via block rewards so they pass the balance check
    test.minePendingTransactions(seshy.publicKey);
    test.minePendingTransactions(carol.publicKey);
    test.minePendingTransactions(gina.publicKey);
    test.minePendingTransactions(lena.publicKey);
    std::cout << "-----------------------------------------------------------------" << std::endl;
    // BLOCK 1
    test.addTransaction(seshy.sendMoney(bob.publicKey, 2.0, "Seshy sends 2 X to Bob"));
    test.addTransaction(bob.sendMoney(amy.publicKey, 1.5, "Bob sends 1.5 X to Amy"));
    test.addTransaction(amy.sendMoney(zed.publicKey, 0.75, "Amy sends 0.75 X to Zed"));
    test.addTransaction(zed.sendMoney(seshy.publicKey, 0.25, "Zed sends 0.25 X to Seshy"));
    test.minePendingTransactions(seshy.publicKey); // Seshy acts as the miner for this block

    // BLOCK 2
    test.addTransaction(carol.sendMoney(dave.publicKey, 3.0, "Carol sends 3 X to Dave"));
    test.addTransaction(dave.sendMoney(eve.publicKey, 2.0, "Dave sends 2 X to Eve"));
    test.addTransaction(eve.sendMoney(frank.publicKey, 1.0, "Eve sends 1 X to Frank"));
    test.addTransaction(frank.sendMoney(carol.publicKey, 0.5, "Frank sends 0.5 X to Carol"));
    test.minePendingTransactions(carol.publicKey);

    // BLOCK 3
    test.addTransaction(gina.sendMoney(hank.publicKey, 4.0, "Gina sends 4 X to Hank"));
    test.addTransaction(hank.sendMoney(ivy.publicKey, 2.5, "Hank sends 2.5 X to Ivy"));
    test.addTransaction(ivy.sendMoney(jake.publicKey, 1.25, "Ivy sends 1.25 X to Jake"));
    test.addTransaction(jake.sendMoney(gina.publicKey, 0.75, "Jake sends 0.75 X to Gina"));
    test.minePendingTransactions(gina.publicKey);

    // BLOCK 4
    test.addTransaction(lena.sendMoney(moe.publicKey, 5.0, "Lena sends 5 X to Moe"));
    test.addTransaction(moe.sendMoney(nina.publicKey, 2.2, "Moe sends 2.2 X to Nina"));
    test.addTransaction(nina.sendMoney(oscar.publicKey, 1.1, "Nina sends 1.1 X to Oscar"));
    test.addTransaction(oscar.sendMoney(lena.publicKey, 0.9, "Oscar sends 0.9 X to Lena"));
    test.minePendingTransactions(lena.publicKey);

    std::cout << "Valid chain: " << (test.isChainValid() ? "True" : "False") << std::endl;
    std::cout << "Total minutes to mine (start->finish): " << test.getMiningTime() << " minutes." << std::endl;

    return 0;
}