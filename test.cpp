#include <iostream>
#include <openssl/ec.h>      // Elliptic Curve functions
#include <openssl/obj_mac.h> // Object identifiers (for NID_secp256k1)
#include <openssl/bn.h>      // BigNumber functions (to print the keys)

int main() {
    // 1. Initialize the Elliptic Curve object using Bitcoin's curve
    EC_KEY* keyPair = EC_KEY_new_by_curve_name(NID_secp256k1);
    
    if (keyPair == nullptr) {
        std::cerr << "Error creating curve object." << std::endl;
        return 1;
    }

    // 2. Generate the Private and Public Keys mathematically
    if (!EC_KEY_generate_key(keyPair)) {
        std::cerr << "Failed to generate keys!" << std::endl;
        EC_KEY_free(keyPair);
        return 1;
    }

    // 3. Extract the Private Key (a giant random number)
    const BIGNUM* privKey = EC_KEY_get0_private_key(keyPair);
    char* privKeyHex = BN_bn2hex(privKey); // Convert to Hex string

    // 4. Extract the Public Key (a point on the 2D elliptic curve graph)
    const EC_GROUP* group = EC_KEY_get0_group(keyPair);
    const EC_POINT* pubKey = EC_KEY_get0_public_key(keyPair);
    
    // We use "UNCOMPRESSED" format, which starts with '04' and includes both X and Y coordinates
    char* pubKeyHex = EC_POINT_point2hex(group, pubKey, POINT_CONVERSION_UNCOMPRESSED, nullptr);

    // 5. Print the results
    std::cout << "\n=== NEW WALLET GENERATED ===\n";
    std::cout << "Private Key (KEEP SECRET): \n" << privKeyHex << "\n\n";
    std::cout << "Public Key (YOUR ADDRESS): \n" << pubKeyHex << "\n";
    std::cout << "============================\n\n";

    // 6. Clean up OpenSSL memory allocations (Classic C-style cleanup)
    OPENSSL_free(privKeyHex);
    OPENSSL_free(pubKeyHex);
    EC_KEY_free(keyPair);

    return 0;
}