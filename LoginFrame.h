#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <wx/wx.h>
#include <cryptopp/sha.h>
#include <iostream>
#include <fstream>
#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include <tuple>
#include "Messages.hpp"

class LoginFrame : public wxFrame
{
public:
    LoginFrame(const wxString &title);

private:
    wxTextCtrl *ID_text;
    wxStaticText *status_text;
    wxStaticText *header_text;
    wxStaticText *existing_user_text;
    wxStaticText *new_user_text;

    wxButton *submit_button;
    wxCheckBox *new_user_checkbox;
    wxCheckBox *existing_user_checkbox;

    // function that triggers the server communication logic
    void OnButtonPress(wxCommandEvent &event);
    // function that sends the public value and waits for A B and the Signature.
    wxString SendPV_WaitForResponse(wxSocketClient &socket, const wxString &msg);
    // for now, this function just has to perform the verification on the signature.
    void ProcessServerResponse(const wxString &response);
    // the function that initializes parameters on which the public key is derived
    std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Client_DH_initialize();
    // the function that generates the public key.
    UserToServer_DHPublicValue_Message GenerateClientPK();
};

#endif