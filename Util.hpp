#pragma once

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <cstddef>

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/integer.h>
#include <cryptopp/misc.h>
#include <cryptopp/rsa.h>
#include <cryptopp/queue.h>
#include <cryptopp/base64.h>

// String <=> Vec<char>.
std::string chvec2str(std::vector<unsigned char> data);
std::vector<unsigned char> str2chvec(std::string s);

// SecByteBlock <=> Integer.
CryptoPP::Integer byteblock_to_integer(const CryptoPP::SecByteBlock &block);
CryptoPP::SecByteBlock integer_to_byteblock(const CryptoPP::Integer &x);

// SecByteBlock <=> string.
std::string byteblock_to_string(const CryptoPP::SecByteBlock &block);
CryptoPP::SecByteBlock string_to_byteblock(const std::string &s);

// Printers.
void print_string_as_hex(std::string str);
void print_key_as_int(const CryptoPP::SecByteBlock &block);
void print_key_as_hex(const CryptoPP::SecByteBlock &block);

// Splitter.
std::vector<std::string> string_split(std::string str, char delimiter);

// RSA PK=>SecbyteBlock
CryptoPP::SecByteBlock RSA_PublicKey_To_SecByteBlock(const CryptoPP::RSA::PublicKey &pk);

// RSA SK=>SecByteBlock
CryptoPP::SecByteBlock RSA_SecretKey_To_SecByteBlock(const CryptoPP::RSA::PrivateKey &pk);

// SecByteBlock => RSA pk
CryptoPP::RSA::PublicKey SecByteBlock_To_RSA_Public_Key(const CryptoPP::SecByteBlock &rsa_pk);

// SecByteBlock => RSA sk
CryptoPP::RSA::PrivateKey SecByteBlock_To_RSA_Secret_Key(const CryptoPP::SecByteBlock &rsa_sk);

// secByteBlock => base64 string
std::string Base64Encode(const CryptoPP::SecByteBlock &bloq);

// base64 string => secByteBlock
CryptoPP::SecByteBlock Base64Decode(const std::string &encodedString);