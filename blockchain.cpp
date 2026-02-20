#include "blockchain.h"
#include <iostream>



Blockchain::Blockchain() {
    _chain.emplace_back(Block(0, {}, "0")); //start with index 0 and a dummy previous hash of "0"
/* 
    Difficulty 4 means the hash must start with "0000", might change later due to cryptographic performance on different machines.
    On a modern laptop, this takes < 1 second.
*/
    _difficulty = 6; //with difficulty 8, it takes around 10-20 seconds to mine a block.
}


/*
    The addBlock function is responsible for adding a new block to the blockchain. It takes the data for the new block,
    retrieves the hash of the last block to link it properly, creates a new block, mines it using the proof-of-work algorithm,
    and finally adds it to the chain.
*/
void Blockchain::addBlock(std::string data) {

    std::string lastHash = _chain.back().hash;
    Block newBlock(_chain.size(), data, lastHash); //take it from the block constructor

    //mine: Proof of Work
    std::cout << "Mining block[" << newBlock.index << "] ..." << std::endl;
    newBlock.mineBlock(_difficulty);

    _chain.push_back(newBlock); //to the vector named _chain, we push the new block that we just mined.
}

bool Blockchain::isChainValid() const {
    for (size_t i = 1; i < _chain.size(); i++) {
        const Block& currentBlock = _chain[i];
        const Block& prevBlock = _chain[i - 1];

        //Has the data within the current block been changed?
        if (currentBlock.hash != currentBlock.calculateHash()) {
            std::cout << "Invalid Hash at index " << i << std::endl;
            return false;
        }

        //Does the current block properly point to the previous block?
        if (currentBlock.prevHash != prevBlock.hash) {
            std::cout << "Chain link broken at index " << i << std::endl;
            return false;
        }
    }
    return true;
}

double Blockchain::getMiningTime() const {
    if (_chain.size() < 2) return 0.0;

    double totalTime = 0.0;
    for (size_t i = 1; i < _chain.size(); i++) {
        const Block& currentBlock = _chain[i];
        const Block& prevBlock = _chain[i - 1];
        totalTime += difftime(currentBlock.timestamp, prevBlock.timestamp);
    }
    double result = totalTime;
    if (totalTime > 60.0) {
        result = totalTime / 60.0;
    }

    return (result * 100.0) / 100.0; 
}