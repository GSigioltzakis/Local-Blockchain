#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct Transaction { //will change to a class later, for now we just use a struct to represent a transaction (maybe)
    std::string sender;
    std::string receiver;
    double amount;

    //helper to turn the transaction into a string for hashing
    std::string toString() const {
        return sender + receiver + std::to_string(amount);
    }
};

#endif