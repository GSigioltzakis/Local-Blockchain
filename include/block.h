#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <ctime>

class Block {
public:
    int index;
    long long timestamp;
    std::string data; //must change to a vector of transactions later, for now we just use a string to represent the data in the block
    std::string prevHash;
    std::string hash;
    int nonce; //proof of work

    Block(int idx, const std::string& d, const std::string& prev);
    
    std::string calculateHash() const;
    
    void mineBlock(int difficulty); 
};

#endif