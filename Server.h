#ifndef SERVER_H
#define SERVER_H

#include <wx/socket.h>
#include <wx/wx.h>
#include <cryptopp/sha.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include "Messages.hpp"
#include <cryptopp/rsa.h>
#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include "cryptopp/nbtheory.h"
#include "cryptopp/integer.h"
#include "cryptopp/dh.h"
#include "cryptopp/dsa.h"
#include "cryptopp/aes.h"
#include "cryptopp/pssr.h"
#include "cryptopp/modes.h"
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cstdint>

class Server : public wxEvtHandler
{
public:
    Server();
    ~Server();
    bool Start(int port);
    void Stop();

private:
    // the server
    wxSocketServer *server;
    // the socket corresponding to the server
    wxSocketBase *socket;
    // the response struct, to be populated, serialized, sent.
    ServerToUser_DHPublicValue_Message Server_response;
    // on socket event function
    void OnSocketEvent(wxSocketEvent &event);
    // function that takes in the client public key and uses it to create the signature (shared key??)
    ServerToUser_DHPublicValue_Message ResponseToClientPV(
        UserToServer_DHPublicValue_Message clientPK);
    // function that generates RSA pair for one way server authentication
    std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> RSA_generate_keys();

private: // HELPERS
    // function to turn RSA public key to a string
    std::string SerializePublicKey(const CryptoPP::RSA::PublicKey &key);
    // function to turn a hex string back to a sec block
    CryptoPP::SecByteBlock decoder(const std::string &hexString);
    // serialize a key to hex(for one sided auth)
    std::string SerializeHex(const CryptoPP::SecByteBlock &block);
    //
    std::string ToHexString(const char *data, size_t length);
    //
    std::string ToStringHex(const std::string &hex);

private: // HANDLER FUNCTIONS
    // function to handle incoming connection
    void Server_HandleSocketConnection();
    // function to handle incoming socket input
    void Server_HandleSocketInput();
    // function to handle a socket being lost
    void Server_HandleSocketLost();

private: // CRYPTOGRAPHIC FUNCTIONS
    // function to sign a string with an rsa private key
    std::string RSA_sign(const CryptoPP::RSA::PrivateKey &signing_key,
                         std::vector<unsigned char> message);
    // function to generate the server's public key (and private key)
    std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Server_DH_initialize();
};

#endif // SERVER_H
