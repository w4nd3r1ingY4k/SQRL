#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <wx/wx.h>
#include "cryptopp/sha.h"
#include <iostream>
#include <fstream>

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

    void OnButtonPress(wxCommandEvent &event);
};

#endif