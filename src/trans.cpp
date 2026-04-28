#include "../include/trans.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>


std::string Transaction::toString() const {
    std::stringstream ss;
    ss << sender_addr 
       << recipient_addr 
       << std::fixed << std::setprecision(8) << amount //fixed and setprecicion to see 8 digits for a better amount
       << dataMsg;
       
    return ss.str();
}

bool Transaction::isValid() const {
    if (sender_addr.empty() || recipient_addr.empty() || amount <= 0) {
        return false;
    }
    return true; 
}