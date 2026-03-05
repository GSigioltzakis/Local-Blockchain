#ifndef MERKLE_H
#define MERKLE_H

#include <vector>
#include <string>
#include "trans.h"

class MerkleTree {
public:
    static std::string buildRoot(const std::vector<Transaction>& transactions); //merkle root

private:
    static std::string hashPair(const std::string& left, const std::string& right); //to hash a pair
};

#endif