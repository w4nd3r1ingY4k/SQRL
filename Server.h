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
    wxSocketServer *server;
    void OnSocketEvent(wxSocketEvent &event);
    // function to initialize the server parameters and public key
    std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Server_DH_initialize();
    // function that takes in the client public key and uses it to create the signature (shared key??)
    ServerToUser_DHPublicValue_Message ResponseToClientPV(
        UserToServer_DHPublicValue_Message clientPK);
    // function that generates RSA pair for one way server authentication
    std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> RSA_generate_keys();
    // function to turn RSA public key to a string
    std::string SerializePublicKey(const CryptoPP::RSA::PublicKey &key);
};

#endif // SERVER_H
