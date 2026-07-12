#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "Block.h"

class Blockchain {
public:
    Blockchain();

    void addTransaction(const Transaction& tx);
    void minePendingTransactions(const std::string& minerAddress);
    double getBalance(const std::string& address) const;

    bool isChainValid() const;
    double getMiningTime() const;

private:
    // --- protocol constants ---
    static constexpr int    MAX_TX_PER_BLOCK     = 10;    // mempool cap per block
    static constexpr int    HALVING_INTERVAL      = 100;   // blocks between reward halvings
    static constexpr int    ADJUSTMENT_INTERVAL   = 10;    // blocks between difficulty retargets
    static constexpr int    TARGET_BLOCK_TIME     = 10;    // seconds per block (ideal)
    static constexpr double INITIAL_REWARD        = 50.0;  // coinbase reward at genesis
    static constexpr double MAX_SUPPLY            = 21'000'000.0;

    // --- helpers ---
    double getCurrentReward() const;

    uint32_t _difficulty;
    std::vector<Block> _chain;
    std::vector<Transaction> _pendingTransactions; // mempool
    time_t _lastAdjustmentTime;                     // timestamp at start of current difficulty window
};

#endif
