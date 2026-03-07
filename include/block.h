#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include "trans.h"

class Block {
public:
    int index;
    long long timestamp;
    std::vector<Transaction> transactions; //transactions included in this block (Merkle root will be used for hashing)
    std::string prevHash;
    std::string hash;
    int nonce; //proof of work

    Block(int idx, const std::vector<Transaction>& txs, const std::string& prev);
    
    std::string calculateHash() const;
    
    void mineBlock(int difficulty); 
    // print merkle tree (one-time visual). Doesn't change hashing behavior.
    void printMerkleTree() const;
};

#endif