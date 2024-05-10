#include "Util.hpp"

/**
 * Convert char vec to string.
 */
std::string chvec2str(std::vector<unsigned char> data)
{
    std::string s(data.begin(), data.end());
    return s;
}

/**
 * Convert string to char vec.
 */
std::vector<unsigned char> str2chvec(std::string s)
{
    std::vector<unsigned char> v(s.begin(), s.end());
    return v;
}

/**
 * Converts a byte block into an integer.
 */
CryptoPP::Integer byteblock_to_integer(const CryptoPP::SecByteBlock &block)
{
    return CryptoPP::Integer(block, block.size());
}

/**
 * Converts an integer into a byte block.
 */
CryptoPP::SecByteBlock integer_to_byteblock(const CryptoPP::Integer &x)
{
    size_t encodedSize = x.MinEncodedSize(CryptoPP::Integer::UNSIGNED);
    CryptoPP::SecByteBlock bytes(encodedSize);
    x.Encode(bytes.BytePtr(), encodedSize, CryptoPP::Integer::UNSIGNED);
    return bytes;
}

/**
 * Converts a byte block into a string.
 */
std::string byteblock_to_string(const CryptoPP::SecByteBlock &block)
{
    return std::string(block.begin(), block.end());
}

/**
 * Converts a string into a byte block.
 */
CryptoPP::SecByteBlock string_to_byteblock(const std::string &s)
{
    CryptoPP::SecByteBlock block(reinterpret_cast<const CryptoPP::byte *>(&s[0]), s.size());
    return block;
}

/**
 * Given a string, it prints its hex representation of the raw bytes it
 * contains. Used for debugging.
 */
void print_string_as_hex(std::string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        std::cout << std::hex << std::setfill('0') << std::setw(2)
                  << static_cast<int>(str[i]) << " ";
    }
    std::cout << std::endl;
}

/**
 * Prints contents as integer
 */
void print_key_as_int(const CryptoPP::SecByteBlock &block)
{
    std::cout << byteblock_to_integer(block) << std::endl;
}

/**
 * Prints contents as hex.
 */
void print_key_as_hex(const CryptoPP::SecByteBlock &block)
{
    std::string result;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));

    encoder.Put(block, block.size());
    encoder.MessageEnd();

    std::cout << result << std::endl;
}

/**
 * Split a string.
 */
std::vector<std::string> string_split(std::string str, char delimiter)
{
    std::vector<std::string> result;
    // construct a stream from the string
    std::stringstream ss(str);
    std::string s;
    while (std::getline(ss, s, delimiter))
    {
        result.push_back(s);
    }
    return result;
}

/**
 * Convert an rsa public key into a sec byte block
 */
CryptoPP::SecByteBlock RSA_PublicKey_To_SecByteBlock(const CryptoPP::RSA::PublicKey &pk)
{
    CryptoPP::ByteQueue queue;
    pk.Save(queue);

    CryptoPP::SecByteBlock byteBlock(queue.CurrentSize());
    queue.Get(byteBlock, byteBlock.size());

    return byteBlock;
}

/**
 * converts a sec byte block back into an rsa public key
 */
CryptoPP::RSA::PublicKey SecByteBlock_To_RSA_Public_Key(const CryptoPP::SecByteBlock &rsa_pk)
{
    CryptoPP::ByteQueue queue;
    queue.Put(rsa_pk, rsa_pk.size());
    queue.MessageEnd();

    CryptoPP::RSA::PublicKey publicKey;

    publicKey.Load(queue);

    // error check?

    return publicKey;
}

/**
 * converts an RSA private key to a sec byte block
 */
CryptoPP::SecByteBlock RSA_SecretKey_To_SecByteBlock(const CryptoPP::RSA::PrivateKey &sk)
{
    CryptoPP::ByteQueue queue;
    sk.Save(queue);

    CryptoPP::SecByteBlock byteBlock(queue.CurrentSize());
    queue.Get(byteBlock, byteBlock.size());

    return byteBlock;
}

/**
 * converts a secbyte block back into an rsa secret key
 */
CryptoPP::RSA::PrivateKey SecByteBlock_To_RSA_Secret_Key(const CryptoPP::SecByteBlock &rsa_sk)
{
    CryptoPP::ByteQueue queue;
    queue.Put(rsa_sk, rsa_sk.size());
    queue.MessageEnd();

    CryptoPP::RSA::PrivateKey privateKey;

    privateKey.Load(queue);

    return privateKey;
}

/**
 * converts a sec byte block into a base 64 encoded std string
 */
std::string Base64Encode(const CryptoPP::SecByteBlock &bloq)
{
    std::string base64;
    CryptoPP::Base64Encoder encoder;

    encoder.Attach(new CryptoPP::StringSink(base64));
    encoder.Put(bloq, bloq.size());
    encoder.MessageEnd();

    return base64;
}

/**
 * converts a base 64 encoded std string back into a sec byte block
 */
CryptoPP::SecByteBlock Base64Decode(const std::string &encodedString)
{
    CryptoPP::Base64Decoder decoder;
    CryptoPP::SecByteBlock decodedBytes;

    decoder.Put((const CryptoPP::byte *)encodedString.data(), encodedString.size());
    decoder.MessageEnd();

    size_t decodedSize = decoder.MaxRetrievable();
    decodedBytes.resize(decodedSize);
    if (decodedSize)
    {
        decoder.Get(decodedBytes.BytePtr(), decodedSize);
    }

    return decodedBytes;
}
