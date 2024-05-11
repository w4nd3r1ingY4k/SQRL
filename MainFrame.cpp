#include <wx/wx.h>
#include "MainFrame.h"
#include <iostream>
#include "CryptoDriver.h"
#include <wx/socket.h>
#include "LoginFrame.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

MainFrame::MainFrame(std::string ID, const wxString &title) : wxFrame(nullptr, wxID_ANY, title), ID_text(ID)
{
	this->SetBackgroundColour(wxColour(47, 79, 79));
	// Panel for REPL display with scrolling capability
	wxPanel *panel_top_left = new wxPanel(this, wxID_ANY);
	panel_top_left->SetBackgroundColour(wxColor(50, 100, 100));

	// Scrolled window within the panel
	repl_panel_top_left = new wxScrolledWindow(panel_top_left, wxID_ANY, wxDefaultPosition, wxSize(150, 160), wxBORDER_SIMPLE);
	// repl_panel_top_left->SetScrollRate(5, 5);
	repl_panel_top_left->SetBackgroundColour(wxColor(65, 100, 100));

	// Panel for text input and submission
	wxPanel *panel_bottom_left = new wxPanel(this, wxID_ANY);

	panel_bottom_left->SetBackgroundColour(wxColor(50, 100, 100));

	// Text controls and buttons
	text_box = new wxTextCtrl(panel_bottom_left, wxID_ANY, wxT("Enter ID here!"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	text_box_pw = new wxTextCtrl(panel_bottom_left, wxID_ANY, wxT("Enter Data here!"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxButton *submit_button = new wxButton(panel_bottom_left, wxID_ANY, ("Encrypt!"), wxDefaultPosition, wxDefaultSize);
	wxButton *allkeys_button = new wxButton(panel_bottom_left, wxID_ANY, ("Key Search!"), wxDefaultPosition, wxDefaultSize);
	wxButton *clear_button = new wxButton(panel_bottom_left, wxID_ANY, ("Clear!"), wxDefaultPosition, wxDefaultSize);
	wxButton *decrypt_button = new wxButton(panel_bottom_left, wxID_ANY, "Decrypt!", wxDefaultPosition, wxDefaultSize);

	// Right side panel for customization (not specified in detail in provided code)
	wxPanel *right_side = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(150, 30));
	right_side->SetBackgroundColour(wxColour(50, 100, 100));

	// Sizers for layout management
	wxBoxSizer *top_left_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_left = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontal_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *buttons_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *textbox_elt_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *repl_sizer = new wxBoxSizer(wxVERTICAL);

	// Set sizer for the scrolled window to manage the contents
	repl_panel_top_left->SetSizer(repl_sizer);

	// Add text box and buttons to the bottom left panel sizer
	buttons_sizer->Add(submit_button, 1, wxEXPAND | wxALL);
	buttons_sizer->AddSpacer(5);
	buttons_sizer->Add(decrypt_button, 1, wxEXPAND | wxALL);
	buttons_sizer->AddSpacer(5);
	buttons_sizer->Add(allkeys_button, 1, wxEXPAND | wxALL);
	buttons_sizer->AddSpacer(5);
	buttons_sizer->Add(clear_button, 1, wxEXPAND | wxALL);

	textbox_elt_sizer->Add(text_box, 1, wxEXPAND | wxRIGHT | wxTOP, 3);
	textbox_elt_sizer->Add(text_box_pw, 1, wxEXPAND | wxRIGHT | wxTOP, 3);
	textbox_elt_sizer->Add(buttons_sizer, 1, wxEXPAND | wxALL, 3);
	panel_bottom_left->SetSizer(textbox_elt_sizer);

	// Set sizer for the top left panel to manage the scrolled window and static text
	wxStaticText *repl_text = new wxStaticText(panel_top_left, wxID_ANY,
											   "Welcome to SQRL v.0.1.0!\nWith SQRL, you can encrypt sensitive data locally\nwith customizable parameters.");
	font = repl_text->GetFont();
	font.SetPointSize(9);
	repl_text->SetFont(font);
	repl_text->SetForegroundColour(*wxWHITE);
	top_left_sizer->Add(repl_text, 0, wxEXPAND | wxALL, 5);
	top_left_sizer->Add(repl_panel_top_left, 1, wxEXPAND | wxALL, 5);
	panel_top_left->SetSizer(top_left_sizer);

	// Add left and right panels to the main frame sizer
	sizer_left->Add(panel_top_left, 0, wxEXPAND | wxALL, 5);
	sizer_left->Add(panel_bottom_left, 1, wxEXPAND | wxALL, 5);
	horizontal_sizer->Add(sizer_left, 1, wxEXPAND | wxALL, 5);
	horizontal_sizer->Add(right_side, 0, wxEXPAND | wxALL, 5);
	this->SetSizer(horizontal_sizer);

	// Bindings
	submit_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonEncrypt, this);
	decrypt_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonDecrypt, this);
	clear_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClear, this);

	// Finalize the layout and show the status bar
	this->SetSizerAndFit(horizontal_sizer);
	this->CreateStatusBar();
}
//
std::string MainFrame::HashIDTwice(const std::string &id)
{
	// Concatenate salt and ID
	std::string saltedInput = id;

	// Prepare the hash object
	CryptoPP::SHA256 hash;

	std::string digest;
	CryptoPP::StringSource ss(saltedInput, true,
							  new CryptoPP::HashFilter(hash,
													   new CryptoPP::HexEncoder(
														   new CryptoPP::StringSink(digest))));

	return digest;
}

// implementation of OnKeyEvent as defined in MainFrame.h
void MainFrame::OnKeyEvent(wxKeyEvent &event)
{
	wxChar keyChar = event.GetUnicodeKey();
	wxLogStatus("key event: %c", keyChar);
}
/**
 *
 */
std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;

	while (std::getline(iss, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}
/**
 * take the id
 * iterate through file until you find the line with id as id
 * delimit to get the encrypted text on its own
 * decrypt that
 * return decrypted text to the console.
 */
void MainFrame::OnButtonDecrypt(wxCommandEvent &event)
{
	wxString id_text = text_box->GetValue();
	std::string id_text_str(id_text);
	std::string filename = HashIDTwice(ID_text) + ".txt";
	std::ifstream file(filename);
	std::string line;
	std::string decrypted_data;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::vector<std::string> tokens = split(line, '|');
			if (tokens.size() == 2)
			{
				std::string id = tokens[0];
				std::string encrypted_data = tokens[1];
				if (id_text_str == id)
				{
					decrypted_data = decrypt(encrypted_data);
					std::cout << decrypted_data << std::endl;
					break;
				}
			}
		}
	}
	if (!decrypted_data.empty())
	{
		wxTextCtrl *idTextCtrl = new wxTextCtrl(repl_panel_top_left, wxID_ANY,
												"Data with ID: " + id_text + " [DECRYPTED] -> " + wxString(decrypted_data.c_str(), wxConvUTF8),
												wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxBORDER_NONE);
		idTextCtrl->SetBackgroundColour(repl_panel_top_left->GetBackgroundColour());
		idTextCtrl->SetForegroundColour(wxColour(230, 230, 230));
		font.SetPointSize(10);
		idTextCtrl->SetFont(font);

		wxSizer *sizer = repl_panel_top_left->GetSizer();
		sizer->Add(idTextCtrl, 0, wxEXPAND | wxALL, 5);
		repl_panel_top_left->FitInside();
		repl_panel_top_left->Layout();
	}
	else
	{
		wxMessageBox("No data found or failed to decrypt.", "Decryption", wxOK | wxICON_ERROR);
	}
}

// /*
// 	On button press, the encrypted text file is iterated through and every key is returned for ease of decryption
// */

void MainFrame::OnButtonAllKeys(wxCommandEvent &event)
{
}
/*
	on button clear clears every element from the main text box.
*/
void MainFrame::OnButtonClear(wxCommandEvent &event)
{
	wxSizer *sizer = repl_panel_top_left->GetSizer();
	int num_items = sizer->GetItemCount();
	for (int i = 0; i < num_items; i++)
	{
		wxSizerItem *item = sizer->GetItem((size_t)0);
		if (item)
		{
			sizer->Remove(0);
		}

		repl_panel_top_left->Layout();
	}
}
/**
 * decryption scheme
 */
std::string MainFrame::decrypt(const std::string &encrypted_data)
{
	std::cout << "decrypting...";
	using namespace CryptoPP;
	byte key[AES::DEFAULT_KEYLENGTH];
	byte iv[AES::BLOCKSIZE];
	memset(key, 0x01, AES::DEFAULT_KEYLENGTH); // Same example key
	memset(iv, 0x01, AES::BLOCKSIZE);		   // Same example IV

	std::string plaintext;

	try
	{
		CBC_Mode<AES>::Decryption dec;
		dec.SetKeyWithIV(key, sizeof(key), iv);

		// The StreamTransformationFilter removes
		// padding as required.
		StringSource ss(encrypted_data, true,
						new StreamTransformationFilter(dec,
													   new StringSink(plaintext)) // StreamTransformationFilter
		);																		  // StringSource
	}
	catch (const CryptoPP::Exception &e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return plaintext;
}
/**
 * encryption scheme
 */
std::string MainFrame::encrypt(const std::string &plaintext)
{
	using namespace CryptoPP;
	byte key[AES::DEFAULT_KEYLENGTH];
	byte iv[AES::BLOCKSIZE];
	memset(key, 0x01, AES::DEFAULT_KEYLENGTH); // Example key
	memset(iv, 0x01, AES::BLOCKSIZE);		   // Example IV

	std::string ciphertext;

	try
	{
		CBC_Mode<AES>::Encryption enc;
		enc.SetKeyWithIV(key, sizeof(key), iv);

		// The StreamTransformationFilter adds padding
		// as required. ECB and CBC Mode must be padded
		// to the block size of the cipher.
		StringSource ss(plaintext, true,
						new StreamTransformationFilter(enc,
													   new StringSink(ciphertext)) // StreamTransformationFilter
		);																		   // StringSource
	}
	catch (const CryptoPP::Exception &e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return ciphertext;
}

void MainFrame::OnButtonEncrypt(wxCommandEvent &event)
{
	wxString idText = text_box->GetValue();
	// apply the function here
	wxString dataText = text_box_pw->GetValue();
	std::string data_text_str(dataText);

	// Use a wxTextCtrl with wxTE_READONLY to allow text selection
	wxTextCtrl *idTextCtrl = new wxTextCtrl(repl_panel_top_left, wxID_ANY, "Data with ID: " + idText + " [ENCRYPTED]", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxBORDER_NONE);

	// Set the background color to match the scrolled window
	idTextCtrl->SetBackgroundColour(repl_panel_top_left->GetBackgroundColour());
	idTextCtrl->SetForegroundColour(wxColour(230, 230, 230));
	font.SetPointSize(10);
	idTextCtrl->SetFont(font);

	// Add the read-only text controls to the scrolled window's sizer
	wxSizer *sizer = repl_panel_top_left->GetSizer();
	int num_items = sizer->GetItemCount();
	if (num_items >= 6)
	{
		wxSizerItem *item = sizer->GetItem((size_t)0);
		if (item)
		{
			sizer->Remove(0);
		}

		repl_panel_top_left->Layout();
	}
	sizer->Add(idTextCtrl, 0, wxEXPAND | wxALL, 5);

	// Update the scrolled window
	repl_panel_top_left->FitInside(); // Adjust scrollbars to accommodate new content
	repl_panel_top_left->Layout();	  // Recalculate layout

	// Update the entire frame's layout if necessary
	this->Layout();

	// Reset the text boxes
	text_box->SetValue(wxT("Enter ID here!"));
	text_box_pw->SetValue(wxT("Enter Data here!"));

	// Write the encrypted data to file
	std::fstream encryption_data_opener(HashIDTwice(ID_text) + ".txt", std::ios::app);
	if (encryption_data_opener.is_open())
	{
		encryption_data_opener << idText << "|" << encrypt(data_text_str) << "\n";
		encryption_data_opener.close(); // Close the file stream explicitly
	}
}
