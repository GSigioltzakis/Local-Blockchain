#include "block.h"
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <functional>

/*
    in the block function, the constructor initializes the block's properties including index, data,
    previous hash, timestamp, nonce, and calculates the initial hash of the block.

    we need the timestamp to record when the block was created, and the nonce is used for the mining 
    process to find a valid hash that meets the difficulty requirement.

*/
Block::Block(int idx, const std::string& d, const std::string& prev) {
    index = idx;
    data = d;
    prevHash = prev;
    timestamp = std::time(nullptr);
    nonce = 0; //nonce is used for mining, it starts at 0 and will be incremented until we find a valid hash.
    hash = calculateHash(); //calling fucntion for hash calculation when block is created
}


/*
    The calculateHash function computes the hash of the block's contents using SHA-256. It combines
    the block's index, timestamp, data, previous hash, and nonce into a single string, which is then
    hashed to produce a unique identifier for the block.
*/
std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << index << timestamp << data << prevHash << nonce;
    std::string input = ss.str(); //the input string is a combination of all the block's properties that we want to hash together

    //---SHA-256 implementation using OpenSSL library
    unsigned char hashResult[SHA256_DIGEST_LENGTH]; //buffer, SHA256_DIGEST_LENGTH is a constant defined in OpenSSL that represents the length of the hash output (32 bytes for SHA-256)
    
    SHA256((unsigned char*)input.c_str(), input.length(), hashResult); //we take 3 parameters: the input data, its length, and a buffer to store the resulting hash
    
    std::stringstream hexStream;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << (int)hashResult[i]; 
        /*
        we convert each byte of the hash result into a hexadecimal string and append
        it to the hexStream. The std::setw(2) and std::setfill('0') ensure that each byte is represented
        as two hexadecimal characters, even if it's less than 16 (0x10). We convert it to hexadecimal because
        it's a common way to represent binary data in a human-readable format, and it makes the hash easier
        to work with and display.
        */
    }
    return hexStream.str();
}
/*
    The mineBlock function implements the proof-of-work algorithm O(2^n), where n is the difficulty level. It continuously increments the nonce
    and recalculates the hash until it finds a hash that starts with a certain number of zeros (defined
    by the difficulty). This process ensures that mining a block requires computational effort, making
    it secure against tampering.
*/
void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');
    //we keep hashing until we find a hash that starts with enough zeros
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        hash = calculateHash();
    }

    printf("Block MINED with Hash: %s\n", hash.c_str());
}