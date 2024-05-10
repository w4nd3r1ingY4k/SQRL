#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <wx/socket.h>
#include <wx/wx.h>
#include <cryptopp/sha.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include "Messages.hpp"
#include "Util.hpp"
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
#include "cryptopp/secblock.h"
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cstdint>

class LoginFrame : public wxFrame
{
public:
    LoginFrame(const wxString &title);
    // for the emailing
    std::string challenge_string;
    std::string user_email;
    void updateChallengeString();

private: // UI ELEMENTS
    // the socket that takes in information
    wxSocketClient *socket;

    // UI elements
    wxTextCtrl *ID_text;
    wxTextCtrl *EMAIL_text;
    wxStaticText *status_text;
    wxStaticText *header_text;
    wxStaticText *existing_user_text;
    wxStaticText *new_user_text;
    wxButton *submit_button;
    wxCheckBox *new_user_checkbox;
    wxCheckBox *existing_user_checkbox;
    wxTextCtrl *five_digit_code;
    wxStaticText *instructions;
    wxButton *check_button;
    int counter = 3;

private: // MESSAGE TYPES TO BE POPULATED & CRYPTO VARIABLES
    ServerToUser_DHPublicValue_Message *ServerToUserPVM;

private: // FUNCTIONS
    // function that triggers the server communication logic
    void OnButtonPress(wxCommandEvent &event);
    // function that sends the public value and waits for A B and the Signature.
    wxString SendPV_WaitForResponse(wxSocketClient &socket, const wxString &msg);
    // for now, this function just has to perform the verification on the signature.
    void ProcessServerResponse(const wxString &response);
    // logic for when the new user checkbox is ticked
    void OnNewUserCheck(wxCommandEvent &event);
    //
    void OnExistingUserCheck(wxCommandEvent &event);
    // Socket event handler function
    void OnSocketEvent(wxSocketEvent &event);
    // process rsa key
    // process dh public key
    // process rsa signature

    // handles socket input flag
    void HandleSocketInput();
    // socket connection
    void HandleSocketConnection();
    // socket lost
    void HandleSocketLost();
    // serialize a key to hex(for one sided auth)
    std::string SerializeHex(const CryptoPP::SecByteBlock &block);
    // function to turn a hex string back to a sec block
    CryptoPP::SecByteBlock decoder(const std::string &hexString);
    //
    std::string ToHexString(char *data, size_t length);
    //
    void OnButtonDecrement(wxCommandEvent &event);

private: // CRYPTOGRAPHIC FUNCTIONS
    // the function that initializes parameters on which the public key is derived
    std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Client_DH_initialize();
    // function to verify the signature against the rsa public key
    bool RSA_verify(const CryptoPP::RSA::PublicKey &verification_key,
                    const std::vector<unsigned char> &message,
                    const std::string &signature);
    //
    std::string GenerateRandomSalt(size_t length);
    //
    std::string HashSaltAndID(const std::string &salt, const std::string &id);
};

#endif