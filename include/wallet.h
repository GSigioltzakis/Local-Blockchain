#ifndef WALLET_H
#define WALLET_H

#include <string>
#include "trans.h"

class Wallet {
public:
    std::string privateKey;
    std::string publicKey;
    std::string ownerName;
    Wallet(const std::string& name= "Null");
    
    Transaction sendMoney(const std::string& recipient_address, double amount, const std::string& data);
};

#endif