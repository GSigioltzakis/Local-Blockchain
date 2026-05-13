#include "../include/trans.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>

static std::string sha256_tx(const std::string& input) {
    unsigned char hashResult[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hashResult);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hashResult[i];
    return ss.str();
}

std::string Transaction::toString() const {
    std::stringstream ss;
    ss << sender_addr 
       << recipient_addr 
       << std::fixed << std::setprecision(8) << amount 
       << dataMsg;
    return ss.str();
}

//ECDSA SIGNATURE CREATION
std::string Transaction::sign(const std::string& privateKeyHex) {
    std::string txHash = sha256_tx(this->toString());

    EC_KEY* key = EC_KEY_new_by_curve_name(NID_secp256k1);
    
    BIGNUM* privBN = nullptr;
    BN_hex2bn(&privBN, privateKeyHex.c_str());
    EC_KEY_set_private_key(key, privBN);

    ECDSA_SIG* sig = ECDSA_do_sign((const unsigned char*)txHash.c_str(), txHash.length(), key);

    if (sig == nullptr) {
        std::cerr << "Failed to sign transaction!" << std::endl;
    } else {
        const BIGNUM *r, *s;
        ECDSA_SIG_get0(sig, &r, &s);
        
        char* rHex = BN_bn2hex(r); //64 hex chars for r
        char* sHex = BN_bn2hex(s);
        
        std::stringstream sigStream;
        sigStream << std::setw(64) << std::setfill('0') << rHex 
                  << std::setw(64) << std::setfill('0') << sHex;
        
        this->signature = sigStream.str();

        OPENSSL_free(rHex);
        OPENSSL_free(sHex);
        ECDSA_SIG_free(sig);
    }

    BN_free(privBN);
    EC_KEY_free(key);
    
    return this->signature;
}

bool Transaction::isValid() const {
    if (sender_addr.empty() || recipient_addr.empty() || amount <= 0) return false;
    
    if (sender_addr == "System") return true; //"System" because it's the source of block rewards, which don't require signatures

    if (signature.length() != 128) { //128 hex chars = 64 bytes = 512 bits, which is the size of an ECDSA signature (r and s each 32 bytes)
        std::cerr << "Invalid signature length." << std::endl;
        return false;
    }

    std::string txHash = sha256_tx(this->toString());

    EC_KEY* key = EC_KEY_new_by_curve_name(NID_secp256k1); //curve for ECDSA
    if (!key) return false;

    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    EC_POINT* pubPoint = EC_POINT_hex2point(group, sender_addr.c_str(), nullptr, nullptr);
    if (!pubPoint) {
        EC_GROUP_free(group);
        EC_KEY_free(key);
        return false;
    }
    EC_KEY_set_public_key(key, pubPoint);
    std::string rHex = signature.substr(0, 64);
    std::string sHex = signature.substr(64, 64);
    ECDSA_SIG* sig = ECDSA_SIG_new();
    BIGNUM* r = BN_new();
    BIGNUM* s = BN_new();
    BN_hex2bn(&r, rHex.c_str());
    BN_hex2bn(&s, sHex.c_str());
    ECDSA_SIG_set0(sig, r, s);
    int verifyStatus = ECDSA_do_verify((const unsigned char*)txHash.c_str(), txHash.length(), sig, key);

    ECDSA_SIG_free(sig);
    EC_POINT_free(pubPoint);
    EC_GROUP_free(group);
    EC_KEY_free(key);

    return (verifyStatus == 1);
}