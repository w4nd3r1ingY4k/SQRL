#include <wx/wx.h>
#include "MainFrame.h"
#include <fstream>
#include <iostream>
#include "CryptoDriver.h"
#include <wx/socket.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
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
											   "Welcome to SQUIRL v.0.1.0!\nWith SQUIRL, you can encrypt sensitive data locally\nwith customizable parameters.");
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
	clear_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClear, this);

	// Finalize the layout and show the status bar
	this->SetSizerAndFit(horizontal_sizer);
	this->CreateStatusBar();
}

// implementation of OnKeyEvent as defined in MainFrame.h
void MainFrame::OnKeyEvent(wxKeyEvent &event)
{
	wxChar keyChar = event.GetUnicodeKey();
	wxLogStatus("key event: %c", keyChar);
}
void MainFrame::OnButtonDecrypt(wxCommandEvent &event)
{
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
 * encryption scheme
 */
wxString MainFrame::encrypt(wxString id)
{
}
void MainFrame::OnButtonEncrypt(wxCommandEvent &event)
{
	wxString idText = text_box->GetValue();
	wxString dataText = text_box_pw->GetValue();

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
	std::fstream encryption_data_opener("encryption_data.txt", std::ios::app);
	if (encryption_data_opener.is_open())
	{
		encryption_data_opener << idText << "|" << dataText << "\n";
		encryption_data_opener.close(); // Close the file stream explicitly
	}
}
