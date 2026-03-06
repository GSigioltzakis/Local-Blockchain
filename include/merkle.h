#ifndef MERKLE_H
#define MERKLE_H

#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include "trans.h"

class MerkleTree {
public:
    // buildRoot: when verbose==true prints the tree layers once to the provided stream; default is no prints
    static std::string buildRoot(const std::vector<Transaction>& transactions, bool verbose = false, std::ostream& out = std::cout); //merkle root

private:
    static std::string hashPair(const std::string& left, const std::string& right); //to hash a pair
};

#endif