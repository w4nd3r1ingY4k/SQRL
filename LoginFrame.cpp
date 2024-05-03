#include <wx/socket.h>
#include "LoginFrame.h"
#include "MainFrame.h" // Ensure this is included if used
#include "constants.h"

LoginFrame::LoginFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
    this->SetBackgroundColour(wxColour(47, 79, 79));

    ID_text = new wxTextCtrl(this, wxID_ANY, "9 DIGIT ID", wxPoint(200, 200), wxSize(80, 20));
    submit_button = new wxButton(this, wxID_ANY, "Submit", wxDefaultPosition, wxDefaultSize);

    new_user_checkbox = new wxCheckBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    existing_user_checkbox = new wxCheckBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    existing_user_text = new wxStaticText(this, wxID_ANY, "<-- Check if ID exists!", wxDefaultPosition, wxDefaultSize);
    existing_user_text->SetForegroundColour(*wxWHITE);

    new_user_text = new wxStaticText(this, wxID_ANY, " <-- Check if ID is new!", wxDefaultPosition, wxDefaultSize);
    new_user_text->SetForegroundColour(*wxWHITE);

    header_text = new wxStaticText(this, wxID_ANY, "SQIRL v0.1.0", wxDefaultPosition, wxDefaultSize);
    header_text->SetFont(wxFont(23, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    header_text->SetForegroundColour(*wxWHITE);

    status_text = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    status_text->SetForegroundColour(*wxWHITE);
    // ASCII Art - Squirrel in Side Profile
    wxString squirrelArt = wxT(
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠋⠀⢘⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠎⠀⠀⠀⠈⢧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⠱⡄⠀⠀⠀⠀⠀⠀⠀⠀⣼⠀⠀⠀⠀⠀⠀⠙⠦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⢀⡜⢀⣀⠹⠤⠤⢄⣀⠀⠀⠀⠀⢻⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⠢⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⣠⠜⠋⠁⠀⠀⢀⣤⡞⠻⡍⠢⣄⠀⠈⢆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠢⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⢀⠎⠀⠀⠀⠀⠀⢠⠎⣰⠃⠀⢹⠀⠈⠣⡀⠈⢣⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢆⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⢠⠇⠀⠀⠀⠀⠀⠀⠋⠀⢷⡄⠀⠀⡇⠀⠀⠙⢦⡀⠙⢆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡆⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⢀⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠋⠀⣰⠁⠀⠀⠀⠀⠉⠒⠂⠷⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡀⠀⠀⠀⠀⠀\n"
        "⠀⠀⡜⠀⠀⠀⠀⢠⣴⠦⡄⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⢤⡀⠀⠀⠀⠀⠀⠀⠀⣧⠀⠀⠀⠀⠀\n"
        "⠀⠀⣧⠀⠀⠀⠠⢾⣿⡷⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢆⠀⠀⠀⠀⠀⠀⢹⡄⠀⠀⠀⠀\n"
        "⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⠀⠀⠀⠀⠀⢸⠃⠀⠀⠀⠀\n"
        "⣠⣾⠛⢿⡟⠃⣀⢤⡄⠀⢀⡴⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠇⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀\n"
        "⣿⣘⣿⠿⠭⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⢰⠁⠀⠀⠀⠀⠀\n"
        "⠙⠯⠿⣗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⣀⡠⠤⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠆⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠉⠉⠉⠉⠑⠢⢄⡀⠀⠀⠀⣀⠔⢋⠔⠊⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⡜⠀⠀⡜⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡏⠉⠉⠀⢠⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃⠀⡼⠁⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⠸⡄⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃⠀⡜⠁⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⢸⠟⠛⣶⣆⠉⠙⢦⡀⠘⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣃⠤⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠈⠓⠚⠧⠤⠤⠤⠤⠭⢶⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⣤⡤⡴⠒⠒⠒⠉⠉⠓⠠⣄⣀⣀⣀⣀⠴⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⢻⡏⢉⡡⢴⠃⢠⠔⠒⠒⠈⠉⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠀⠘⠒⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    wxStaticText *artDisplay = new wxStaticText(this, wxID_ANY, squirrelArt, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    artDisplay->SetFont(wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Use monospaced font
    artDisplay->SetForegroundColour(*wxWHITE);

    // Use this squirrelArt string to set up a wxStaticText widget in your LoginFrame constructor
    submit_button->Bind(wxEVT_BUTTON, &LoginFrame::OnButtonPress, this);

    wxBoxSizer *existing_box = new wxBoxSizer(wxHORIZONTAL);
    existing_box->Add(existing_user_checkbox);
    existing_box->Add(existing_user_text);

    wxBoxSizer *new_box = new wxBoxSizer(wxHORIZONTAL);
    new_box->Add(new_user_checkbox);
    new_box->Add(new_user_text);

    wxBoxSizer *checkbox_sizer = new wxBoxSizer(wxVERTICAL);
    checkbox_sizer->Add(new_box, 0, 5);
    checkbox_sizer->AddStretchSpacer(6);
    checkbox_sizer->Add(existing_box, 0, 5);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->AddStretchSpacer(2);
    hbox->Add(artDisplay, 0, wxALL, 10);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(header_text, 0, 5);
    sizer->AddStretchSpacer(4);
    sizer->Add(status_text, 0, 5);
    sizer->AddStretchSpacer(4);
    sizer->Add(ID_text, 0, 5);
    sizer->AddStretchSpacer(4);
    sizer->Add(checkbox_sizer, 0, 5);
    sizer->Add(submit_button, 0, wxALL | wxCENTER, 5);
    sizer->AddStretchSpacer(3);
    sizer->Add(hbox, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(sizer);
    this->CreateStatusBar();
    this->Layout();
}

// Correctly define OnButtonPress as a member of LoginFrame
void LoginFrame::OnButtonPress(wxCommandEvent &event)
{

    // Create a socket client
    wxSocketClient socket;
    wxIPV4address addr;
    addr.Hostname("localhost"); // Server address
    addr.Service(3000);         // Server port

    // Try to connect to the server
    wxLogMessage("Client: Attempting to Connect to server...");
    socket.Connect(addr, true);

    if (socket.IsConnected())
    {

        status_text->SetForegroundColour(*wxGREEN);
        status_text->SetLabel("Connected To Server");
        wxLogMessage("Client: Connected to server");

        // initialize params to get public key
        std::tuple<CryptoPP::DH,
                   CryptoPP::SecByteBlock,
                   CryptoPP::SecByteBlock>
            InitializedParams = Client_DH_initialize();

        CryptoPP::SecByteBlock PublicKey = std::get<2>(InitializedParams);
        CryptoPP::SecByteBlock PrivateKey = std::get<1>(InitializedParams);

        // PK LOG START
        std::string PublicKey_hex;
        CryptoPP::HexEncoder encoder;
        encoder.Attach(new CryptoPP::StringSink(PublicKey_hex));
        encoder.Put(PublicKey.BytePtr(), PublicKey.size());
        encoder.MessageEnd();

        // Log the public key
        wxString logMessage = wxString::Format("Client: Public value -> %s", PublicKey_hex);
        wxLogMessage(logMessage);

        //-------------

        wxString ID = ID_text->GetValue();
        wxString message = "LOGIN " + ID; // Include newline if server expects lines

        // Send the message
        socket.Write(message, message.length());
        socket.Write(PublicKey_hex.c_str(), PublicKey_hex.length());

        // Wait for response
        char response[10];
        socket.Read(response, sizeof(response) - 1); // Leave space for null terminator
        response[socket.LastCount()] = '\0';         // Null-terminate the string

        wxString serverResponse(response);

        // Handle the server's response
        if (serverResponse.StartsWith("OK"))
        {
            this->Hide();
            MainFrame *mainFrame = new MainFrame("Main Application");
            mainFrame->SetClientSize(675, 350);
            mainFrame->Center();
            mainFrame->Show(true);
        }
        else if (serverResponse.StartsWith("INVALID"))
        {
            status_text->SetForegroundColour(*wxRED);
            status_text->SetLabel("Login failed, Server response: INVALID");
        }
    }
    else
    {
        status_text->SetForegroundColour(*wxRED);
        status_text->SetLabel("Failed to connect to server!");
        socket.Close();
    }
}
// Initialize parameters
std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> LoginFrame::Client_DH_initialize()
{
    CryptoPP::DH DH_obj(DL_P, DL_Q, DL_G);
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock DH_private_key(DH_obj.PrivateKeyLength());
    CryptoPP::SecByteBlock DH_public_key(DH_obj.PublicKeyLength());
    DH_obj.GenerateKeyPair(prng, DH_private_key, DH_public_key);
    return std::make_tuple(DH_obj, DH_private_key, DH_public_key);
}
