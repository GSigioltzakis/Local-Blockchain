<p align="center">
  <img src="finalBC.png" width="600" height="700">
</p>

# Simple blockchain creation - Giorgos Sigioltzakis

## 1. mining a block
from the files block.cpp/h:
1. We create a constructor Block() with the block info while catching a timestamp in Unix.
2. With calculateHash() and using SHA-256 algorithm from the openssl/sha.h library, we create the blocks metadata. (we turn the data->hash but not reversible)
3. We "mine" a block using proof of work (pc power) and calculating a hash until we finde 000XXXX...XXX, 4 zeros. Then we can say that we have a valid block.

## 2. Ledger Managment, the blockchain
from the files `blockchain.cpp/h`:
1. We create a constructor Blockchain(), which we give "birth" to lets say block 0 etc..., to start the chain.
2. With `addBlock()` we crete the block (new blocks), which it retrieves the hash's recent block of the chain using "prevHash" from 1.2->`calculateHash()` and then "mine".
3. `isChainValid()`: we check 2 factors: if the block's hash match the actual data and if the prevHashes are linked correctly.

## 3. Small Transactions & The Merkle Tree
From the files `trans.cpp` and `trans.h`:

1. **The Transaction Struct:** We created a `Transaction` struct with sender and receiver variables, alongside a helper function toString(). This turns the transaction into a string so it can be securely hashed by the block.
2. **Merkle Tree Implementation:** We created a new utility hash function to convert transactions into strings and store them in a tree structure. Starting with all the initial transaction hashes at the bottom level of the tree, we hash the pairs of items together. We continue going up level by level, combining the hashes until we reach 1 final hash named **Mroot (Merkle Root)**. This root proves that all transactions are included in the block; any change to a single transaction would drastically change the root hash, making it easy to detect tampering.

**Implementation Checklist:**
- [x] In `block.h`, replace std::string data with a vector used for transactions.
- [x] Implement the Merkle Tree where the root is stored directly in the block header.
- [x] Implement `merkle.cpp` and `merkle.h` and link them to the main project and Makefile.
---
### Performance Notes & Throttling
* **Does flushing slow down mining?** Yes. Printing and flushing to the terminal on every single hash trial creates extra I/O work and noticeably increases elapsed time, especially on consoles. Throttling the updates (as implemented) keeps useful visual feedback while minimizing this overhead.
* **The Math Behind Difficulty & Printing:**
  - Difficulty 4 → ~65k attempts
  - Difficulty 5 → ~1.05M attempts
  - Difficulty 6 → ~16.8M attempts

  With PRINT_EVERY = 1000, the number of prints ≈ (attempts / 1000). If each print+flush costs **p** milliseconds, the added time is roughly(attempts / 1000) * p.
  - If p = 0.5 ms, difficulty 6 adds ≈ 8.4s to the total mining time.
  - If p = 0.1 ms, difficulty 6 adds ≈ 1.68s to the total mining time.

## 4. Signature Verifications - ECDSA
(small note: i use ECDSA signature and address generation instead of RSA only for the space (byte wise))
Before implementing anything, must understand how the openssl library works. Ive generated a small test file to implement a single signature using various libraries from openssl. In general tho, the steps for a correct signature verifications are:
1. We have: a constant *prime* number `n` and a generator point `G` (where G={x, y}). 
2. From those two now we need to generate a: `privKey` and a `pubKey`. The *private key* is not that hard to create as with a mathimatical "formula" we generate it as a random integer in the range of [1, n-1]. The *public key* is a bit more complex because we add an extra step. To find the public key we must multiply the private key with the generator point G: **`pubKey = privKey * G`**. That is called EC point multiplication (EC:Elliptic Curve).

---
3. Now for the ECDSA sign we must:
    - Using SHA256 calculate the message's hash, so we have **`h = hash(msg)`**
    - In the range of [2, n-2] we find a random (secure lets say) **k** for calculating random point, signature proofs ect..., relative prime to n-1.
    - Calculating the **R**andom point **R = k * G** (R is on the 'x' coordinate) -> **`r = R.x`**
    - Calculating the signature proof: **`s = k^-1(h + r * privKey)(mod n)`**.
    - *The calculated signature {r, s} is a pair of integers, each in the range [1...n-1]. It encodes the random point R = k * G, along with a proof s, confirming that the signer knows the message h and the private key privKey. The proof s is by idea verifiable using the corresponding pubKey.*
    ### Signature verification
    - **Verification:** The network takes the $\{r, s\}$ pair, the message hash $h$, and the sender's pubKey. Using curve mathematics, the network reverses the operation to find the point $R$. If the $x$-coordinate of the recovered point matches the $r$ from the signature, it is verified as TRUE.
---
4. ### References
    * [ECDSA: Signing and Verifying Messages](https://cryptobook.nakov.com/digital-signatures/ecdsa-sign-verify-messages)
    * [ElGamal Signature Scheme (Wikipedia)](https://en.wikipedia.org/wiki/ElGamal_signature_scheme)
    # Output of: 12/3/2026
    ```
    small signature: (in Hex)
        Private Key (KEEP SECRET):
    556E174F1739B738BE090F7BB9A6BC33F6E4086FDAF8FFD6B92157A014CAED8D

        Public Key (YOUR ADDRESS):
    04AB9F9F7F8E24084E27B43BE8B0DBBD7EF88DD6B99171D643853973A3EF62A57AB238A8BC1722971BE6F9032051BB421A6905749EDC1AA9DA8D17E67A66821B7C
    ```
5. ### code implementation
- [x] In `trans.cpp` first of all we need encryption as we convert the transaction's string, hashed and we convert that hash to hex due to OpenSSL library.
- [x] Also in `trans.h` 2 new methods have been added to the constructor of the transactions. Those methods are: *sign()* and (newly implemeted), *isValid()* methods.
- [x] ***sign()***,
    *   Takes the user's raw Private Key (in Hex format) and converts it into an OpenSSL `BIGNUM`.
    *   Signs the SHA-256 hash of the transaction using the `secp256k1` curve.
    *   Extracts the mathematical `r` and `s` components from the generated ECDSA signature.
    *   Pads both `r` and `s` to exactly 64 characters to prevent parsing errors, concatenating them into a final **128-character hex string**.
- [x] ***isValid()***,
    *   **Sanity Checks:** Rejects empty addresses, zero/negative amounts, and invalid signature lengths (must be exactly 128 characters, representing 64 bytes or 512 bits).
    *   **System Bypass:** Recognizes `"System"` as a valid sender without a signature to natively support Block Mining Rewards.
    *   **Mathematical Proof:** Re-hashes the current transaction data and converts the `sender_addr` (which acts as the Public Key) back into an Elliptic Curve Point (`EC_POINT`). It then splits the 128-char signature back into `r` and `s`.
    *   Uses `ECDSA_do_verify` to mathematically prove that the holder of the corresponding Private Key authorized this exact transaction hash.
- [x] At last, each time we run the "network", a .log file appears (similar to the merklee.log) that shows us the privated and public keys of the invisible people that make the transactions. NOTE: that these transactions are not being saved anywhere nor these people have the same addresses when we rerun the "network" (30/4/2026). (to be implemeted later.)

# Output of: 30/4/2026 (updated from above to permanently stay in codebase) .log file
```
=== WALLET: Seshy ===
Private Key (SECRET) : AF22C74FEE285A7550B148A667D2633B33F6916C4330C66FEA6B3E4A25B665AC
Public Key  (ADDRESS): 04A1F2C1763402C5B91DE878B92BAA535EC30EEB983232EFA4C21...128 chars in hex

=== WALLET: Bob ===
Private Key (SECRET) : FA29D90D04720EA52DE769C37B5156C4E2CA34FF5916793BD12F2254B605233C
Public Key  (ADDRESS): 043434748F378A4654745FE6F96F2BBE6738CCDDCE2A63F7ADF36...128 chars in hex

=== WALLET: Amy ===
Private Key (SECRET) : E272D16F35A01EC9D82990B32708896E04654F80A966EAD4C86D6E8135271117
Public Key  (ADDRESS): 0419BFB63532DF1CD58F2D3EBBA30ABC27D8CF98AF565DF114914...128 chars in hex
.
.
.
```
---
## Final touches for the "way-of-bank"
1) Correct Mempool implementation in `blockchain.cpp` constructor. We will add/correct the current so: someone "sends" points to someone, first those point go to the mempool that we gather the *transactions*. So in the class Blockchain we will add a vector for pendingTransactions.
2) Right now as of **12/5** when someone sends points, that goes through imidiatly without us knowing if that person has that amount or not. Here comes the *Signatures* and the isValid() function in `trans.h/cpp`. If the signature is true, the user has the amount, else not. So for that we need to have another new function in blockchain.cpp to `getBalance(address)`.
3) In addition of the (2) above, we will also need t verify the transaction *before* the mining. So for that we will need to expand the addBlock() to see with each transaction (lets say a token tx), if tx.isValid() is true and if the balance of the sender >= amount. (because else he wont havee the points to send.)
4) The "minting". We also need to expand even more the addBlock() so the **miner** takes the address of that person. So, in the end that person will be rewarded with X points.(lets say for each successfull mine -> 2.5 points)

---

## Output of: 18/2/2026
## using SHA-256:
```
./blockchain
Mining block 1...
Block Mined! Hash: 00009cd1fd17d0b6d825d5b0b76715032c674184521294e20c7c1b154048e12a
Mining block 2...
Block Mined! Hash: 0000feda1a412a6ea8ec930017396f3a8046c890ed0841b03a0b6f2c55906f63
Is chain valid? Yes
```
## using a small hash function:
```
./blockchain
Mining block 1...
Block Mined! Hash: 0000c818325d846f
Mining block 2...
Block Mined! Hash: 0000fc9423493e7c
Is chain valid? Yes
```

## Output of: 6/3/2026
```
time ./al #instead of ./Blockchain, for ease
Mining block[1]...
Block MINED with hash: 0000154da5660b83d08cbf7fbafd3ab35e5709a5fcc279387e6e00c519fc0864
Mining block[2]...
Block MINED with hash: 00006841d57ae68c616bb6c259b03c60f4e32eaa63f570cf3c5fc534d764fdcf
Mining block[3]...
Block MINED with hash: 0000c00cba1cab678a7bdec652aeab085f6fb10a3e8b866efa61bb241d4b5167
Mining block[4]...
Block MINED with hash: 00006b193844c3c166d20e9f89e26b2c87208e88529e54252d0f2306b3c19d72
Valid chain: True
Total minutes to mine (start→finish): 0.05 minutes.

real    0m3.682s
user    0m3.652s
sys     0m0.021s
```

# Output of 13/5/26:
# FINAL skeleton output. A run of the whole blockchain. difficulty = 4 (for speed).
### In `merkle_trees.log` is the tree visualation.
```
Mining block[1]...
Block MINED with hash: 000024acbade235c18dbab0641cbc6fd87b5c8709a2dab98bf2b38a8006549dd4c4062cec01ba
Mining block[2]...
Block MINED with hash: 0000beaaf0ea8480ae09f6fd1739cbbff43dcfc6730ced0518eb72f6a4804aa989e0bf1972d2e
Mining block[3]...
Block MINED with hash: 000015bf8b0a768362aabe138b624afe7472f9fbbab41c2f025d1dd093f6c59f21d3b2578b5ec
Mining block[4]...
Block MINED with hash: 00000887bcd2bbbafd36b3a81951a0cbb558e58533d6eb46714deef98a677a695837e4c6e61d4
-----------------------------------------------------------------
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
Mining block[5]...
Block MINED with hash: 0000aa3ab0ba27f281fde49410138845355d30b07605ca1d033ab7b96e7ca696c1773585577c2
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
Mining block[6]...
Block MINED with hash: 00009f60d985a0dd76a68f904216740a5b2179125b20a348e63d30808932a1546522e0c447e26
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
Mining block[7]...
Block MINED with hash: 00003ee8606b52f99ec74db850a9014f2a08da1aac757355940a22aa3fc019ba17c4074a8d204
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
[MEMPOOL] Transaction added successfully!
Mining block[8]...
Block MINED with hash: 00000b2666c1476fd2cb69bacbf3737fa27457301a201d9ab3dcf5b27a0d83ee5c9a2884f00ad
Valid chain: True
Total minutes to mine (start->finish): 0.133333 minutes.
```
### Execution Analysis

The final terminal output demonstrates the complete lifecycle of the blockchain's state machine and consensus rules:

1. **Currency Issuance (Blocks 1-4):** The network initializes by mining four blocks with an empty mempool. This executes the system's sole minting mechanism, distributing the initial 50-coin block rewards to fund the primary participants.
2. **State Verification & Mempool Queuing:** As users initiate transfers, the system mathematically verifies the ECDSA signatures and evaluates the senders' balances. The `[MEMPOOL] Transaction added successfully!` logs confirm the network successfully authorizes chained transactions using zero-confirmation mempool state logic.
3. **Consensus & Finality (Blocks 5-8):** The engine processes the pending mempool transactions, computes the new Merkle Root, and executes the SHA-256 Proof of Work. Discovering the target hash (`0000...`) cryptographically locks the transactions and the new miner rewards into the ledger.
4. **Integrity Validation:** The final `Valid chain: True` output verifies the structural integrity of the entire network, confirming that no block data has been tampered with and all cryptographic links (`prevHash`) are securely intact.
