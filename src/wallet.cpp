#include "../include/wallet.h"
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <fstream>

Wallet::Wallet(const std::string& name) : ownerName(name) {
    EC_KEY* keyPair = EC_KEY_new_by_curve_name(NID_secp256k1);
    EC_KEY_generate_key(keyPair);

    const BIGNUM* privKeyBN = EC_KEY_get0_private_key(keyPair);
    char* privKeyHex = BN_bn2hex(privKeyBN);

    const EC_GROUP* group = EC_KEY_get0_group(keyPair);
    const EC_POINT* pubKeyPoint = EC_KEY_get0_public_key(keyPair);
    char* pubKeyHex = EC_POINT_point2hex(group, pubKeyPoint, POINT_CONVERSION_UNCOMPRESSED, nullptr);

    this->privateKey = std::string(privKeyHex);
    this->publicKey = std::string(pubKeyHex);

    // --- LOGGING TO FILE ---
    std::ofstream logFile("wallets.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "=== WALLET: " << this->ownerName << " ===\n";
        logFile << "Private Key (SECRET) : " << this->privateKey << "\n";
        logFile << "Public Key  (ADDRESS): " << this->publicKey << "\n\n";
        logFile.close();
    }

    OPENSSL_free(privKeyHex);
    OPENSSL_free(pubKeyHex);
    EC_KEY_free(keyPair);
}

Transaction Wallet::sendMoney(const std::string& recipient_address, double amount, const std::string& data) {
    Transaction newTx(this->publicKey, recipient_address, amount, data);
    newTx.sign(this->privateKey);
    return newTx;
}