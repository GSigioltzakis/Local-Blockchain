#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct Transaction {
    std::string sender;
    std::string receiver;
    double amount;

    //helper to turn the transaction into a string for hashing
    std::string toString() const {
        return sender + receiver + std::to_string(amount);
    }
};

#endif