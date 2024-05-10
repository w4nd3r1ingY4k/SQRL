#include "Server.h"
#include "constants.h"
#include "Util.hpp"
#include "Messages.hpp"

Server::Server() : server(nullptr) {}

Server::~Server()
{
    Stop();
}

bool Server::Start(int port = 3000)
{
    // initialize an address using a port
    wxIPV4address addr;
    addr.Service(port);
    server = new wxSocketServer(addr, wxSOCKET_REUSEADDR);
    if (!server->Ok())
    {
        wxLogMessage("Server not initialized");
        return false;
    }
    server->SetEventHandler(*this, wxID_ANY);
    server->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
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
    if (socket)
    {
        socket->Destroy();
        socket = nullptr;
    }
}

void Server::OnSocketEvent(wxSocketEvent &event)
{
    socket = server->Accept(false);
    socket->SetFlags(wxSOCKET_WAITALL);
    socket->SetNotify(wxSOCKET_CONNECTION | wxSOCKET_INPUT | wxSOCKET_LOST);
    socket->Notify(true);

    switch (event.GetSocketEvent())
    {
    // if connection has jsut been established
    case wxSOCKET_CONNECTION:
        // function that handles socket connection
        Server::Server_HandleSocketConnection();
        break;
    // if data is entering the socket
    case wxSOCKET_INPUT:
        // function that handles incoming input
        Server::Server_HandleSocketInput();
        break;
    // if connection gets lost
    case wxSOCKET_LOST:
        // function that handles a lost connection
        Server::Server_HandleSocketLost();
        break;
    }
}

/*
 * @brief a function that handles attempted connection to the server
 * @ brief generates the
 */
void Server::Server_HandleSocketConnection()
{
    char buffer[513] = {0};
    socket->Read(buffer, sizeof(buffer) - 1); // the first read
    wxLogMessage("SERVER: recieved client public key");
    wxLogMessage("SERVER: buffer contents:\n");
    wxLogMessage(buffer);
    if (buffer[0] != '\0')
    {

        // deserialise the public key and store in a pointer to a secbyteblock
        CryptoPP::SecByteBlock ClientPK = Server::decoder(buffer);
        wxLogMessage("SERVER: client public key deserialised");
        std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> InitializedParams = Server_DH_initialize();
        CryptoPP::SecByteBlock PublicKey = std::get<2>(InitializedParams);
        CryptoPP::SecByteBlock PrivateKey = std::get<1>(InitializedParams);
        // at this point we can set the response struct public key variables
        Server_response.server_public_value = PublicKey;
        Server_response.user_public_value = ClientPK;

        // THIS CODE IS IMPORTANT FOR GENERATION OF NEW RSA PARAMETERS
        /*
                // // after reading the public key, we have to send the rsa stuff
                // std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> ParamsRSA = RSA_generate_keys();
                // CryptoPP::RSA::PrivateKey secretk = ParamsRSA.first;
                // CryptoPP::RSA::PublicKey publick = ParamsRSA.second;

                // CryptoPP::SecByteBlock SBB_pk = RSA_PublicKey_To_SecByteBlock(publick);
                // std::string RSA_pk_str = Base64Encode(SBB_pk);
                // wxString pubk = RSA_pk_str;
                // wxLogMessage("Public RSA Key:\n");
                // wxLogMessage(pubk);

                // CryptoPP::SecByteBlock SBB_sk = RSA_SecretKey_To_SecByteBlock(secretk);
                // std::string RSA_sk_str = Base64Encode(SBB_sk);
                // wxString seck = RSA_sk_str;
                // wxLogMessage("Secret RSA key:\n");
                // wxLogMessage(seck);

        */

        // we serialize the server and client public keys to hex
        std::string Server_pk_hex = SerializeHex(PublicKey);
        std::string Client_pk_hex = SerializeHex(ClientPK);
        std::cout << "SERVER: The server public key pre send is:\n"
                  << Server_pk_hex << std::endl;

        std::string RSA_sign_string = Base64Encode(PublicKey) + Base64Encode(ClientPK);
        std::cout << "SERVER: The rsa sign string is: \n"
                  << RSA_sign_string << std::endl;
        try
        {

            CryptoPP::SecByteBlock decoded_str = Base64Decode(B64_RSA_PRIVATE_KEY);
            CryptoPP::RSA::PrivateKey sk = SecByteBlock_To_RSA_Secret_Key(decoded_str);

            // the signature is signed over the concatenation in hex of public keys
            // std::string Signature = RSA_sign(sk, str2chvec(RSA_sign_string));
            std::string test_str = "hello";
            std::string Signature = RSA_sign(sk, str2chvec(test_str));

            Server_response.server_signature = Signature;
            std::cout << "SERVER: The signature being sent is: \n"
                      << std::endl;
            std::cout << Signature << std::endl;
            std::string sigsize = std::to_string(sizeof(Signature));
            std::cout << sigsize << std::endl;
            // std::cout << "SERVER: the signature in the server is:\n " << ToHexString(Signature.c_str(), sizeof(Signature.c_str())) << std::endl;
            std::vector<CryptoPP::byte> data;
            Server_response.ServerToUser_DHPublicValue_Message::serialize(data);

            int pks = Server_pk_hex.size();
            std::string pkss = std::to_string(pks);
            wxString pkswx = pkss;
            wxLogMessage(pkswx);
            std::string Signature_and_pk = Server_pk_hex + "|" + SerializeHex(string_to_byteblock(Signature));
            // std::string Signature_and_pk = Server_pk_hex + "|" + ToHexString(Signature.c_str(), sizeof(Signature.c_str()));
            std::cout << "UPDATED SEND STR: \n";
            std::cout << Signature_and_pk << std::endl;
            int bytes = data.size();
            std::string size = std::to_string(bytes);
            wxString sz = size + " is the size of the message being sent from server";
            wxLogMessage(sz);
            // socket->Write(Signature_and_pk.c_str(), Signature_and_pk.length());
            std::cout << "#############################" << std::endl;
            std::cout << byteblock_to_string(Server_response.server_public_value) << std::endl;
            std::cout << byteblock_to_string(Server_response.user_public_value) << std::endl;
            std::cout << Server_response.server_signature << std::endl;
            std::cout << "##############################" << std::endl;
            socket->Write(data.data(), data.size());
            wxLogMessage("SERVER: Sent the rsa stuf");
            wxLogMessage("buffer");
        }

        catch (const CryptoPP::Exception &e)
        {
            std::cerr << "Crypto++ exception: " << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Standard exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception caught" << std::endl;
        }
    }
    else
    {
        wxLogMessage("SERVER: failed to recieve client public key");
    }
}

/*
 */
void Server::Server_HandleSocketInput()
{
    // recieve the client public key
    // char buffer[1024] = {0};
    // wxLogMessage("Client public key read in"); // Ensuring the buffer is initialized
    // socket->Read(buffer, sizeof(buffer) - 1);  // the first read
    // wxLogMessage("Client public key read in");
    // if (buffer)
    // {
    //     wxLogMessage("Server recieved message");
    //     // deserialise the public key and store in a pointer to a secbyteblock
    //     *ClientPK = Server::decoder(buffer);
    // }
    // // after reading the public key, we have to send the rsa stuff
    // std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> ParamsRSA = RSA_generate_keys();
    // CryptoPP::RSA::PrivateKey sk = ParamsRSA.first;
    // CryptoPP::RSA::PublicKey pk = ParamsRSA.second;
    // std::string string_pk = SerializePublicKey(pk);
    // socket->Write(string_pk.c_str(), string_pk.length());
    wxLogMessage("No mas?");
}

/**
 * uuuuhh
 */
void Server::Server_HandleSocketLost()
{
}

/*
 *   RSA Key Generation
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
    rsa_public_key.AssignFrom(rsa_private_key);
    std::pair<CryptoPP::RSA::PrivateKey, CryptoPP::RSA::PublicKey> rsa_key_pair(rsa_private_key, rsa_public_key);

    return rsa_key_pair;
}
std::string Server::ToHexString(const char *data, size_t length)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i)
    {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return std::string(ss.str());
}
std::string Server::ToStringHex(const std::string &hex)
{
    if (hex.length() % 2 != 0)
    {
        throw std::runtime_error("Hex string must have an even number of digits");
    }

    std::string output;
    output.reserve(hex.length() / 2);
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
        output.push_back(byte);
    }
    return output;
}
/*
 *   Function to serialize the RSA public key
 */
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
/*
 *   Decodes the public key from a hex string back into a sec byte block
 */
CryptoPP::SecByteBlock Server::decoder(const std::string &hexString)
{
    // Check if the input hex string has an even number of characters
    std::string length = std::to_string(hexString.length());
    wxString x = "SERVER: recieved data in buffer has length " + length;
    wxLogMessage(x);

    if (hexString.length() % 2 != 0)
    {
        wxLogMessage("SERVER: Client key hex is not divisible by 2.");
        throw std::runtime_error("Hex string must have an even number of characters.");
    }
    CryptoPP::HexDecoder decoder;
    CryptoPP::SecByteBlock decodedBytes(hexString.length() / 2); // Allocate space for decoded bytes
    // wxLogMessage("Looking for issue...");

    decoder.Put(reinterpret_cast<const CryptoPP::byte *>(hexString.data()), hexString.size());
    decoder.MessageEnd();
    // wxLogMessage("Looking for issue...2");

    size_t decodedSize = decoder.Get(decodedBytes.data(), decodedBytes.size());
    decodedBytes.resize(decodedSize); // Resize SecByteBlock to actual decoded size
    // wxLogMessage("Looking for issue...3");

    if (decodedSize == 0)
    {
        wxLogMessage("SERVER: decoded size is 0");
        throw std::runtime_error("Failed to decode hex string.");
    }
    // wxLogMessage("Looking for issue...4");

    return decodedBytes;
}

// function to sign a string with an rsa private key
std::string Server::RSA_sign(const CryptoPP::RSA::PrivateKey &signing_key,
                             std::vector<unsigned char> message)
{
    std::string rsa_signature;                 // return string.
    auto msg_str_convert = chvec2str(message); // convert message to a string for the signing process

    // instantiate signing apparatus that the message will be passed through to sign it.
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer rsa_signer(signing_key);

    // create a string source that passes msg_str_convert through the signer filter into rsa_signature
    CryptoPP::StringSource ss(msg_str_convert, true, new CryptoPP::SignerFilter(rng, rsa_signer, new CryptoPP::StringSink(rsa_signature)));

    return rsa_signature;
}
// function to generate the server's public key (and private key)
std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Server::Server_DH_initialize()
{
    CryptoPP::DH DH_obj(DL_P, DL_Q, DL_G);
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock DH_private_key(DH_obj.PrivateKeyLength());
    CryptoPP::SecByteBlock DH_public_key(DH_obj.PublicKeyLength());
    DH_obj.GenerateKeyPair(prng, DH_private_key, DH_public_key);
    return std::make_tuple(DH_obj, DH_private_key, DH_public_key);
}

std::string Server::SerializeHex(const CryptoPP::SecByteBlock &block)
{
    std::string PublicKey_hex;
    CryptoPP::HexEncoder encoder;
    encoder.Attach(new CryptoPP::StringSink(PublicKey_hex));
    encoder.Put(block.BytePtr(), block.size());
    encoder.MessageEnd();
    return PublicKey_hex;
}
