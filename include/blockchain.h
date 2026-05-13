#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "Block.h"

class Blockchain {
public:
    Blockchain();

    void addTransaction(const Transaction& tx);
    void minePendingTransactions(const std::string& minerAddress);
    double getBalance(const std::string& address) const;

    bool isChainValid() const;
    double getMiningTime() const; 

private:
    uint32_t _difficulty;
    std::vector<Block> _chain;
    std::vector<Transaction> _pendingTransactions;//mempool
};

#endif