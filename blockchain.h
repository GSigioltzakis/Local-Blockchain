#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "Block.h"

class Blockchain {
public:
    Blockchain();

    void addBlock(std::string data);
    bool isChainValid() const;
    void printChain() const;
    double getMiningTime() const; 

private:
    uint32_t _difficulty;
    std::vector<Block> _chain;

    Block _getLatestBlock() const;
};

#endif