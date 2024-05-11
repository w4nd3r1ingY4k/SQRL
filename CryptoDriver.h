#ifndef CRYPTO_DRIVER_H
#define CRYPTO_DRIVER_H

#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

class CryptoDriver
{
public:
    CryptoDriver();
    void GenerateKeyPair();
    std::string GetPublicKey();
    CryptoPP::SecByteBlock ComputeSharedSecret();
};
#endif
