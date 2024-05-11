#ifndef MAINFRAME_H // Check if MAINFRAME_H is not defined
#define MAINFRAME_H // Define MAINFRAME_H

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
#include <cryptopp/hex.h>
#include <cstdint>
class LoginFrame;

class MainFrame : public wxFrame
{
public:
	MainFrame(std::string ID, const wxString &title);
	void AccessLoginFrameData();

private:
	std::string ID_text;
	void OnKeyEvent(wxKeyEvent &event);
	void OnButtonEncrypt(wxCommandEvent &event);
	void OnButtonDecrypt(wxCommandEvent &event);
	void OnButtonAllKeys(wxCommandEvent &event);
	void OnButtonClear(wxCommandEvent &event);
	wxTextCtrl *text_box;	 // Member variable for ID text box
	wxTextCtrl *text_box_pw; // Member variable for data text box
	wxScrolledWindow *repl_panel_top_left;
	wxFont font;

private: // HANDLERS
	void ConnectToServer();
	void OnSocketEvent(wxSocketEvent &event);
	void SendDataToServer(std::string data);

private: // cryptographic functions
	std::string encrypt(const std::string &plaintext);
	std::string decrypt(const std::string &encrypted_data);
	std::string HashIDTwice(const std::string &id);
};

#endif // MAINFRAME_H
