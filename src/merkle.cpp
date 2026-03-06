#include "merkle.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <ostream>

//helper to perform SHA-256 on a string (same logic as Block::calculateHash)
std::string sha256(const std::string& input) {
    unsigned char hashResult[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hashResult);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hashResult[i];
    return ss.str();
}

/*
The idea of a merkle tree is to take all the transactions in a block, hash them, and then combine
those hashes in pairs and hash again until we get a single hash at the top, which is the Merkle Root (Mroot).
This allows us to efficiently verify that a transaction is included in the block without having to check every transaction.
*/
std::string MerkleTree::buildRoot(const std::vector<Transaction>& transactions, bool verbose, std::ostream& out) {
    if (transactions.empty()) return "";

    std::vector<std::string> currentLayer;
    for (const auto& tx : transactions) {
        currentLayer.push_back(sha256(tx.toString())); //tx(transaction0) is converted to a string and then hashed to get the initial hash for each transaction
    }

    if (verbose) {
        // Print initial layer (leaves)
        out << "Merkle Tree - Layer 0 (leaves):\n";
        for (size_t i = 0; i < currentLayer.size(); ++i) {
            out << "  [" << i << "] " << currentLayer[i] << "\n";
        }
    }

    //if there are still more than one hash in the current layer, we need to combine them in pairs and hash again
    int layer = 1;
    while (currentLayer.size() > 1) {
        std::vector<std::string> nextLayer;
        if (verbose) out << "Merkle Tree - building layer " << layer << " from " << currentLayer.size() << " nodes\n";
        for (size_t i = 0; i < currentLayer.size(); i += 2) {
            std::string left = currentLayer[i];
            std::string right = (i + 1 < currentLayer.size()) ? currentLayer[i + 1] : left;
            std::string parent = sha256(left + right);
            if (verbose) {
                out << "  pair (" << i << ")\n";
                out << "    L: " << left << "\n";
                out << "    R: " << right << "\n";
                out << "    P: " << parent << "\n";
            }
            nextLayer.push_back(parent);
        }
        if (verbose) {
            out << "Merkle Tree - Layer " << layer << ":\n";
            for (size_t j = 0; j < nextLayer.size(); ++j) {
                out << "  [" << j << "] " << nextLayer[j] << "\n";
            }
        }
        currentLayer = nextLayer;
        ++layer;
    } //the root tells us that all transactions are included in the block, and any change to a transaction would change the root hash, making it easy to detect tampering

    return currentLayer[0]; //return the root hash
}