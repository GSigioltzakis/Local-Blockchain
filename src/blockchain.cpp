#include "../include/blockchain.h"
#include <iostream>



Blockchain::Blockchain() {
    _chain.emplace_back(Block(0, std::vector<Transaction>{}, "0")); //start with index 0 and a dummy previous hash of "0"
/* 
    Difficulty 4 means the hash must start with "0000", might change later due to cryptographic performance on different machines.
    On a modern laptop, this takes < 1 second.
*/
    _difficulty = 4; //with difficulty 8, it takes around 10-20 seconds to mine a block.
}


/*
    we will create 3 new methods:
    1) getBalance(address) - to calculate the balance of a given address by iterating through all transactions in the chain.
    2) addTransaction(tx) - to add a new transaction to the mempool (pending transactions list). This will also validate the 
    transaction before adding.
    3) minePendingTransactions(minerAddress) - to create a new block with all pending transactions, mine it, and add it to the chain.
*/
double Blockchain::getBalance(const std::string& address) const {
    double balance = 0.0;
    for (const auto& block : _chain) {
        for (const auto& tx : block.transactions) {
            if (tx.getSender() == address) balance -= tx.getAmount(); 
            if (tx.getRecipient() == address) balance += tx.getAmount(); 
        }
    }

    //Calculate unconfirmed balance from Mempool (Future RAM/Cache Layer)
    for (const auto& tx : _pendingTransactions) {
        if (tx.getSender() == address) balance -= tx.getAmount(); 
        if (tx.getRecipient() == address) balance += tx.getAmount(); 
    }
    return balance;
}

void Blockchain::addTransaction(const Transaction& tx) {
    if (!tx.isValid()) {
        std::cout << "[Declined] failed validation!" << std::endl;
        return;
    }
    
    if (tx.getSender() != "System") {
        double currentBalance = getBalance(tx.getSender());
        if (currentBalance < tx.getAmount()) {
            std::cout << "[Declined] Insufficient balance! (Sending: " 
                      << tx.getAmount() << " | Has: " << currentBalance << ")\n";
            return;
        }
    }

    _pendingTransactions.push_back(tx);
    std::cout << "[MEMPOOL] Transaction added successfully!" << std::endl;
}

void Blockchain::minePendingTransactions(const std::string& minerAddress) {
    Transaction rewardTx("System", minerAddress, 50.0, "Block Reward");
    _pendingTransactions.push_back(rewardTx);

    std::string lastHash = _chain.back().hash;
    Block newBlock(_chain.size(), _pendingTransactions, lastHash);

    std::cout << "Mining block[" << newBlock.index << "]..." << std::endl;
    newBlock.mineBlock(_difficulty);

    _chain.push_back(newBlock);
    _chain.back().printMerkleTree();
    
    _pendingTransactions.clear();
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

    // total time from genesis (index 0) to last block, in minutes
    const Block& first = _chain.front();
    const Block& last = _chain.back();
    double totalSeconds = difftime(last.timestamp, first.timestamp);
    double totalMinutes = totalSeconds / 60.0;
    return totalMinutes;
}