#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "Block.h"

class Blockchain {
public:
    Blockchain();

    void addBlock(const std::vector<Transaction>& txs);
    bool isChainValid() const;
    void printChain() const;
    double getMiningTime() const; 

private:
    uint32_t _difficulty;
    std::vector<Block> _chain;

    Block _getLatestBlock() const;
};

#endif