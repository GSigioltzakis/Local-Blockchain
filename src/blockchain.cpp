#include "../include/blockchain.h"
#include <iostream>
#include <cmath>



Blockchain::Blockchain() {
    _chain.emplace_back(Block(0, std::vector<Transaction>{}, "0")); // genesis block
    _difficulty = 4;
    _lastAdjustmentTime = _chain.back().timestamp; // start retarget window at genesis
}


double Blockchain::getBalance(const std::string& address) const {
    double balance = 0.0;
    for (const auto& block : _chain) {
        for (const auto& tx : block.transactions) {
            if (tx.getSender() == address) balance -= tx.getAmount(); 
            if (tx.getRecipient() == address) balance += tx.getAmount(); 
        }
    }

    // unconfirmed balance from mempool
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


// --- reward halving: reward halves every HALVING_INTERVAL blocks ---
double Blockchain::getCurrentReward() const {
    int halvings = _chain.size() / HALVING_INTERVAL;       // how many halvings so far
    double reward = INITIAL_REWARD;
    for (int i = 0; i < halvings; i++) {
        reward /= 2.0;
        if (reward < 0.0001) return 0.0;                   // negligible, also prevents infinite loop
    }

    // supply cap: count total minted so far (coinbase tx = sender "System")
    double totalMinted = 0.0;
    for (const auto& block : _chain) {
        for (const auto& tx : block.transactions) {
            if (tx.getSender() == "System")
                totalMinted += tx.getAmount();
        }
    }
    if (totalMinted + reward > MAX_SUPPLY)
        return 0.0;                                        // cap reached, no more rewards

    return reward;
}


void Blockchain::minePendingTransactions(const std::string& minerAddress) {
    // --- coinbase reward (halving-aware) ---
    double reward = getCurrentReward();
    if (reward > 0.0) {
        Transaction rewardTx("System", minerAddress, reward, "Block Reward");
        _pendingTransactions.insert(_pendingTransactions.begin(), rewardTx);
    }

    // --- mempool throttling: take at most MAX_TX_PER_BLOCK transactions (FIFO) ---
    std::vector<Transaction> blockTxs;
    size_t take = std::min(_pendingTransactions.size(), static_cast<size_t>(MAX_TX_PER_BLOCK));
    for (size_t i = 0; i < take; i++) {
        blockTxs.push_back(_pendingTransactions[i]);
    }

    std::string lastHash = _chain.back().hash;
    Block newBlock(_chain.size(), blockTxs, lastHash);

    // ensure strictly increasing timestamps (rapid mining in same second)
    if (newBlock.timestamp <= _chain.back().timestamp)
        newBlock.timestamp = _chain.back().timestamp + 1;

    std::cout << "Mining block[" << newBlock.index << "]..." << std::endl;
    newBlock.mineBlock(_difficulty);

    _chain.push_back(newBlock);
    _chain.back().printMerkleTree();

    // --- remove mined transactions from mempool (leave the rest) ---
    _pendingTransactions.erase(_pendingTransactions.begin(),
                               _pendingTransactions.begin() + take);

    // --- dynamic difficulty retargeting every ADJUSTMENT_INTERVAL blocks ---
    if (_chain.size() % ADJUSTMENT_INTERVAL == 0 && _chain.size() > 1) {
        time_t now          = newBlock.timestamp;
        double timeTaken    = difftime(now, _lastAdjustmentTime);   // actual seconds for this window
        double expected     = ADJUSTMENT_INTERVAL * TARGET_BLOCK_TIME;

        if (timeTaken > 0.0) {
            int oldDiff = _difficulty;
            // scale difficulty: too fast -> harder, too slow -> easier
            _difficulty = static_cast<uint32_t>(
                std::round(_difficulty * expected / timeTaken)
            );
            if (_difficulty < 1) _difficulty = 1;                   // floor at 1

            std::cout << "[RETARGET] difficulty " << oldDiff
                      << " -> " << _difficulty
                      << "  (window: " << timeTaken << "s, expected: " << expected << "s)\n";
        }

        _lastAdjustmentTime = now;  // start next window
    }
}


bool Blockchain::isChainValid() const {
    time_t now = std::time(nullptr);

    for (size_t i = 1; i < _chain.size(); i++) {
        const Block& currentBlock = _chain[i];
        const Block& prevBlock    = _chain[i - 1];

        // hash integrity
        if (currentBlock.hash != currentBlock.calculateHash()) {
            std::cout << "Invalid Hash at index " << i << std::endl;
            return false;
        }

        // chain linkage
        if (currentBlock.prevHash != prevBlock.hash) {
            std::cout << "Chain link broken at index " << i << std::endl;
            return false;
        }

        // --- timestamp validation ---
        // must be strictly after previous block
        if (currentBlock.timestamp <= prevBlock.timestamp) {
            std::cout << "Timestamp out of order at index " << i << std::endl;
            return false;
        }
        // must not be too far in the future (2-hour tolerance)
        if (difftime(currentBlock.timestamp, now) > 2 * 3600) {
            std::cout << "Timestamp too far in the future at index " << i << std::endl;
            return false;
        }
    }
    return true;
}


double Blockchain::getMiningTime() const {
    if (_chain.size() < 2) return 0.0;

    const Block& first = _chain.front();
    const Block& last  = _chain.back();
    double totalSeconds = difftime(last.timestamp, first.timestamp);
    double totalMinutes = totalSeconds / 60.0;
    return totalMinutes;
}
