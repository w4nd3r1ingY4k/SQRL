#include "Server.h"
#include "constants.h"

Server::Server() : server(nullptr) {}

Server::~Server()
{
    Stop();
}

bool Server::Start(int port)
{
    // initialize an address using a port
    wxIPV4address addr;
    addr.Service(port);
    server = new wxSocketServer(addr, wxSOCKET_REUSEADDR);

    // if the server cannot be established, throw an error and return false
    if (!server->Ok())
    {
        wxLogError("Could not listen on the specified port!");
        return false;
    }

    server->SetEventHandler(*this, wxID_ANY);
    server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    server->Notify(true);
    Bind(wxEVT_SOCKET, &Server::OnSocketEvent, this);
    return true;
}

void Server::Stop()
{
    if (server)
    {
        server->Destroy();
        server = nullptr;
    }
}

void Server::OnSocketEvent(wxSocketEvent &event)
{
    wxSocketBase *socket = server->Accept(false);
    if (socket)
    {

        char buffer[1024] = {0};                  // Ensuring the buffer is initialized
        socket->Read(buffer, sizeof(buffer) - 1); // Leave room for null terminator
        wxString msg(buffer, wxConvUTF8);
        wxLogMessage(msg + " Was sent to the server\n\n");
        std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> ParamsRSA = RSA_generate_keys();
        CryptoPP::RSA::PrivateKey sk = ParamsRSA.first;
        CryptoPP::RSA::PublicKey pk = ParamsRSA.second;
        // CryptoPP::SecByteBlock Client_PV((CryptoPP::SecByteBlock)msg, msg.size())
        std::string string_pk = SerializePublicKey(pk);
        if (msg.StartsWith("LOGIN"))
        {
            socket->Write(string_pk.c_str(), string_pk.length());
            socket->Write("OK", 2);
        }
        else
        {
            socket->Write("INVALID", 7);
        }
        socket->Close();
        delete socket;
    }
}

/*
    RSA Key Generation
*/
std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> Server::RSA_generate_keys()
{
    // first, we establish a random pool and an intertible rsa function, so that we can establish parameters with randomness
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, RSA_KEYSIZE);

    // we initialize a public and private key with the aforedefined parameters
    CryptoPP::RSA::PrivateKey rsa_private_key(params);
    CryptoPP::RSA::PublicKey rsa_public_key(params);

    // create a std pair and return it.
    std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> rsa_key_pair(rsa_private_key, rsa_public_key);
    return rsa_key_pair;
}

std::string Server::SerializePublicKey(const CryptoPP::RSA::PublicKey &key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    std::string encoded;
    CryptoPP::Base64Encoder encoder;
    queue.CopyTo(encoder);
    encoder.MessageEnd();

    CryptoPP::word64 size = encoder.MaxRetrievable();
    if (size)
    {
        encoded.resize(size);
        encoder.Get((CryptoPP::byte *)&encoded[0], encoded.size());
    }

    return encoded;
}
