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

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

private:
	void OnKeyEvent(wxKeyEvent &event);
	void OnButtonEncrypt(wxCommandEvent &event);
	wxString encrypt(wxString id);
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
};
