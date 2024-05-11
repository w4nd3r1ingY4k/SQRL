#include <wx/socket.h>
#include "LoginFrame.h"
#include "MainFrame.h" // Ensure this is included if used
#include "constants.h"
#include "Messages.hpp"
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

CryptoPP::SecByteBlock PublicKey;

LoginFrame::LoginFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
    this->SetBackgroundColour(wxColour(47, 79, 79));
    updateChallengeString();
    mail_sent = false;

    // This is exclusively for the 5 digit verification code
    check_button = new wxButton(this, wxID_ANY, "Check", wxPoint(250, 170), wxDefaultSize);
    check_button->Hide();
    five_digit_code = new wxTextCtrl(this, wxID_ANY, "ENTER HERE", wxPoint(250, 120), wxDefaultSize);
    five_digit_code->Hide();
    instructions = new wxStaticText(this, wxID_ANY,
                                    "Click the [Check] button once first to send the code to your email. \n"
                                    "Then, check your email for an authentication code!\n"
                                    "NOTE: After 3 attempts, the session will terminate.",
                                    wxPoint(250, 70), wxDefaultSize);
    instructions->SetForegroundColour(*wxWHITE);
    instructions->Hide();

    check_button->Bind(wxEVT_BUTTON, &LoginFrame::OnButtonDecrement, this);
    // end of verification code frame elements.

    // this is for the normal login frame
    ID_text = new wxTextCtrl(this, wxID_ANY, "ID", wxPoint(200, 200), wxSize(80, 20));
    submit_button = new wxButton(this, wxID_ANY, "Submit", wxDefaultPosition, wxDefaultSize);
    first_instructions = new wxStaticText(this, wxID_ANY,
                                          "What's new in v0.0.1:\n"
                                          "This is the first version of SQRL, everything is new!\n"
                                          "NOTE: to enter text into a box, remove the existing text in the box first.",
                                          wxDefaultPosition, wxDefaultSize);
    first_instructions->SetForegroundColour(*wxWHITE);

    EMAIL_text = new wxTextCtrl(this, wxID_ANY, "EMAIL", wxPoint(200, 200), wxSize(80, 20));

    new_user_checkbox = new wxCheckBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    existing_user_checkbox = new wxCheckBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    new_user_checkbox->SetValue(true);

    existing_user_text = new wxStaticText(this, wxID_ANY, "<-- Check if ID exists!", wxDefaultPosition, wxDefaultSize);
    existing_user_text->SetForegroundColour(*wxWHITE);

    new_user_text = new wxStaticText(this, wxID_ANY, " <-- Check if ID is new!", wxDefaultPosition, wxDefaultSize);
    new_user_text->SetForegroundColour(*wxWHITE);

    header_text = new wxStaticText(this, wxID_ANY, "SQRL v0.0.1", wxDefaultPosition, wxDefaultSize);
    header_text->SetFont(wxFont(23, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    header_text->SetForegroundColour(*wxWHITE);

    status_text = new wxStaticText(this, wxID_ANY, "In Signup mode", wxDefaultPosition, wxDefaultSize);
    status_text->SetForegroundColour(*wxYELLOW);
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
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠀⠘⠒⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    wxStaticText *artDisplay = new wxStaticText(this, wxID_ANY, squirrelArt, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    artDisplay->SetFont(wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Use monospaced font
    artDisplay->SetForegroundColour(*wxWHITE);

    // Use this squirrelArt string to set up a wxStaticText widget in your LoginFrame constructor
    submit_button->Bind(wxEVT_BUTTON, &LoginFrame::OnButtonPress, this);
    new_user_checkbox->Bind(wxEVT_CHECKBOX, &LoginFrame::OnNewUserCheck, this);
    existing_user_checkbox->Bind(wxEVT_CHECKBOX, &LoginFrame::OnExistingUserCheck, this);

    wxBoxSizer *code_sizer = new wxBoxSizer(wxVERTICAL);
    code_sizer->Add(instructions, 0, 5);
    code_sizer->Add(five_digit_code, 0, 5);
    code_sizer->Add(check_button, 0, 5);

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
    hbox->Add(first_instructions, 0, wxALL, 10);
    hbox->AddStretchSpacer(2);
    hbox->Add(artDisplay, 0, wxALL, 10);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(header_text, 0, 5);
    sizer->AddStretchSpacer(4);
    sizer->Add(status_text, 0, 5);
    sizer->AddStretchSpacer(4);
    sizer->Add(ID_text, 0, 5);
    sizer->Add(EMAIL_text, 0, 5);
    sizer->AddStretchSpacer(4);
    sizer->Add(checkbox_sizer, 0, 5);
    sizer->Add(submit_button, 0, wxALL | wxCENTER, 5);
    sizer->AddStretchSpacer(3);
    sizer->Add(hbox, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(sizer);
    this->CreateStatusBar();
    this->Layout();
}
void LoginFrame::sendEmail()
{
    if (existing_user_checkbox->IsChecked())
    {
        if (mail_sent == false)
        {
            std::string id(ID_text->GetValue());
            std::string to_email = retrieve_email(id);
            std::string subject = "SQRL Verification Code";
            std::string body = "Here is your SQRL verification code: \n" + challenge_string;

            std::stringstream ss;
            ss << "python python_mail_script.py \"" << subject << "\" \"" << body << "\" \"" << to_email << "\"";
            system(ss.str().c_str());
            mail_sent = true;
        }
        else if (mail_sent == true)
        {
            std::cout << "check your email for auth key" << std::endl;
        }
    }
    else if (new_user_checkbox->IsChecked())
    {
        if (mail_sent == false)
        {
            std::string subject = "SQRL Verification Code";
            std::string body = challenge_string;
            wxString to_email_wx = EMAIL_text->GetValue();
            std::string to_email(to_email_wx);

            std::stringstream ss;
            ss << "python python_mail_script.py \"" << subject << "\" \"" << body << "\" \"" << to_email << "\"";
            system(ss.str().c_str());
            mail_sent = true;
        }
        else if (mail_sent == true)
        {
            std::cout << "Check email for auth key" << std::endl;
        }
    }
}
/**
 *
 */
void LoginFrame::OnButtonDecrement(wxCommandEvent &event)
{
    sendEmail();
    // std::cout
    //     << this->challenge_string << std::endl;
    wxString attempt = five_digit_code->GetValue();
    std::string attempt_str(attempt);

    if (attempt_str != challenge_string)
    {

        counter -= 1;
        if (counter <= 0)
        {
            std::cout << "XXX" << std::endl;
            this->Close();
        }
        else
        {
            std::cout << "Attempts remaining: " << counter << std::endl;
        }
    }
    else
    {
        std::cout << "Logging in..." << std::endl;
        this->Hide();
        // Create and show the MainFrame
        std::string id_text_str(ID_text->GetValue());
        MainFrame *mainFrame = new MainFrame(id_text_str, "Main Frame");
        // Set the size of the frame
        mainFrame->SetSize(800, 600); // Width = 800, Height = 600
        mainFrame->Show();
    }
}

/**
 * @brief when the existing user button is checked:
 *  - the new user button is unchecked
 *  - the email text box is hidden
 *  - the user status flag is set to true
 */
void LoginFrame::OnExistingUserCheck(wxCommandEvent &event)
{
    if (event.IsChecked())
    {
        // If the existing user checkbox is checked, ensure the new user checkbox is unchecked
        new_user_checkbox->SetValue(false);
        status_text->SetForegroundColour(*wxYELLOW);
        this->Layout();
        status_text->SetLabel("In login mode");
        EMAIL_text->Hide();
        this->Layout();
    }
    else
    {
        // If existing user checkbox is being unchecked, automatically check the new user checkbox
        new_user_checkbox->SetValue(true);
        status_text->SetLabel("In signup mode");
        this->Layout();
        EMAIL_text->Show();
        this->Layout();
    }
}
/**
 * @brief when the new user button is checked:
 *  - the existing user button is unchecked
 *  - the email text box is visible
 *  - the user status flag is set to false
 */
void LoginFrame::OnNewUserCheck(wxCommandEvent &event)
{
    if (event.IsChecked())
    {
        // If the new user checkbox is checked, ensure the existing user checkbox is unchecked
        existing_user_checkbox->SetValue(false);
        status_text->SetForegroundColour(*wxYELLOW);
        this->Layout();
        status_text->SetLabel("In signup mode");
        EMAIL_text->Show();
        this->Layout();
    }
    else
    {
        // If new user checkbox is being unchecked, automatically check the existing user checkbox
        existing_user_checkbox->SetValue(true);
        this->Layout();
        status_text->SetLabel("In login mode");
        EMAIL_text->Hide();
        this->Layout();
    }
}

/*
 * On Button Press function. This function:
 *   - Handles button click functionality
 *       - creates a client and attempts to connect client to server
 *       - establishes a public key
 */
void LoginFrame::OnButtonPress(wxCommandEvent &event)
{
    // Create a socket client
    wxIPV4address addr;
    addr.Hostname("localhost"); // Server address
    addr.Service(3000);
    socket = new wxSocketClient;
    socket->SetFlags(wxSOCKET_WAITALL);

    // Using socket to connect to server and establishing event and bind link
    wxLogMessage("CLIENT: Attempting to Connect to server...");
    socket->Connect(addr, false);
    socket->SetEventHandler(*this, wxID_ANY);
    socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    socket->Notify(true);
    Bind(wxEVT_SOCKET, &LoginFrame::OnSocketEvent, this);
}
/*
 *   OnSocketEvent()
 *   @brief this function directs the program to different functions depending on the recieved wxFlag.
 */
void LoginFrame::OnSocketEvent(wxSocketEvent &event)
{
    switch (event.GetSocketEvent())
    {
    // data has arrived at the socket
    case wxSOCKET_INPUT:
        // function that handles data arriving at the socket.
        LoginFrame::HandleSocketInput();
        break;

    // a connection has been established
    case wxSOCKET_CONNECTION:
        // function that handles connection establishment
        LoginFrame::HandleSocketConnection();
        break;

    // if the socket is lost
    case wxSOCKET_LOST:
        // function that handles lost connection
        LoginFrame::HandleSocketLost();
        break;

    // default being an unknown code
    default:
        wxLogMessage("unknown socket code");
        break;
    }
}

/*
 *   Client_DH_Initialize()
 *   @brief this function establishes DH parameters using the constants in constants.h
 *   @return a tuple of dh object, private key and public key in that order.
 */
std::tuple<CryptoPP::DH, CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> LoginFrame::Client_DH_initialize()
{
    CryptoPP::DH DH_obj(DL_P, DL_Q, DL_G);
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock DH_private_key(DH_obj.PrivateKeyLength());
    CryptoPP::SecByteBlock DH_public_key(DH_obj.PublicKeyLength());
    DH_obj.GenerateKeyPair(prng, DH_private_key, DH_public_key);
    return std::make_tuple(DH_obj, DH_private_key, DH_public_key);
}

/*
 *   HandleSocketConnection()
 *   @brief this function handles a connection between the socket and server
 */
void LoginFrame::HandleSocketConnection()
{
    if (socket->IsConnected())
    {
        // send success message to user
        this->Layout();
        status_text->SetForegroundColour(*wxGREEN);
        status_text->SetLabel("Server connection established, Loading...");
        this->Layout();

        // send success message to terminal
        wxLogMessage("CLIENT: Connected to server");

        // establish DH public and private key
        std::tuple<CryptoPP::DH,
                   CryptoPP::SecByteBlock,
                   CryptoPP::SecByteBlock>
            InitializedParams = Client_DH_initialize();

        // establish public and private keys
        PublicKey = std::get<2>(InitializedParams);
        CryptoPP::SecByteBlock PrivateKey = std::get<1>(InitializedParams);

        // serialize the public key with helper function
        std::string PublicKey_hex = SerializeHex(PublicKey);

        // log size of the message in the client
        // std::string size = std::to_string(PublicKey_hex.size());
        // wxString sizew = "CLIENT: public message size is:\n " + size;
        // wxLogMessage(sizew);
        wxLogMessage("CLIENT: public key hex data:\n");
        wxLogMessage(PublicKey_hex.data());

        // send the public key hex encoded

        socket->Write(PublicKey_hex.data(), PublicKey_hex.size());
        wxLogMessage("CLIENT: public key sent");
    }
    else
    {
        status_text->SetForegroundColour(*wxRED);
        status_text->SetLabel("Failed to connect to server!");
        wxLogMessage("CLIENT:: Failed to connect to server");
        socket->Close();
    }
}

/*
 *   HandleSocketInput()
 *   @brief this function handles data (RSA signature) arriving at the socket, sent from the server \n
 *   @brief to do this, the function checks what type of message is incoming\n and calls the function pertaining to the next step
 *   @brief of the process
 *   @brief Recieves RSA, deserializes, runs verify algorithm.
 */
void LoginFrame::HandleSocketInput()
{
    wxLogMessage("CLIENT: recieved server signature");
    wxLogMessage("CLIENT: Reading server sig...");
    size_t publicKeySize = DL_P.ByteCount();
    std::vector<CryptoPP::byte> read_buffer(921);
    void *ptr = static_cast<void *>(read_buffer.data());
    socket->Read(ptr, 921);
    wxLogMessage("CLIENT: read server sig");
    size_t bytes_read = socket->LastCount();
    if (bytes_read > 0 && !socket->Error())
    {

        std::string size_f = std::to_string(read_buffer.size());
        wxString str = "Buffer size is: " + size_f;
        wxLogMessage(str);

        CryptoPP::SecByteBlock decoded_str = Base64Decode(B64_RSA_PUBLIC_KEY);
        CryptoPP::RSA::PublicKey pk = SecByteBlock_To_RSA_Public_Key(decoded_str);

        // std::vector<std::string> split_str = string_split(read_buffer, '|');
        // std::cout << "Here is the server public key: " << std::endl;
        // std::cout << split_str[0] << std::endl;
        // std::cout << "Here is the signature: " << std::endl;
        // std::cout << split_str[1] << std::endl;
        // std::cout << ToHexString((char *)split_str[1].c_str(), sizeof((char *)split_str[1].c_str())) << std::endl;
        ServerToUser_DHPublicValue_Message Server_vals;

        Server_vals.deserialize(read_buffer);
        std::cout << "########################" << std::endl;
        std::cout << byteblock_to_string(Server_vals.server_public_value) << std::endl;
        std::cout << byteblock_to_string(Server_vals.user_public_value) << std::endl;
        std::cout << (Server_vals.server_signature) << std::endl;
        std::cout << "########################" << std::endl;

        std::string pkfr_hex = SerializeHex(PublicKey);
        std::string RSA_sign_string = Base64Encode(Server_vals.server_public_value) + Base64Encode(Server_vals.user_public_value);
        std::cout << "CLIENT: This is the sign string in the client: \n"
                  << RSA_sign_string << std::endl;
        std::string test_str = "hello";
        if (RSA_verify(pk, str2chvec(test_str), Server_vals.server_signature) == true)
        {
            wxLogMessage("Returned false");
        }
        else
        {

            wxLogMessage("True!");
            /**
             * 2FA logic:
             *  - if the new checkbox is checked:
             *      - send id and email
             *      - receive salt, email has been stored
             *      - hash salt and id and send to server
             *      - store hash and Id and then send email
             *      - set timer for client to enter 4 digit code and
             *      - give the user 3 shots to enter code
             *
             *  - if the existing checkbox is checked:
             *      - check that the user ID exists
             *      - send email to the email
             *      - check for the verification code within time limits
             */
            if (new_user_checkbox->IsChecked())
            {

                // hide normal elements
                ID_text->Hide();
                EMAIL_text->Hide();
                status_text->Hide();
                // header_text->Hide();
                existing_user_text->Hide();
                first_instructions->Hide();
                new_user_text->Hide();
                submit_button->Hide();
                new_user_checkbox->Hide();
                existing_user_checkbox->Hide();
                five_digit_code->Show();
                instructions->Show();
                check_button->Show();
                // load verification elements.
                wxString id = ID_text->GetValue();
                std::string id_str(id);
                wxString email = EMAIL_text->GetValue();
                std::string email_str(email);
                user_email = email_str;
                sendEmail();

                // wxCharBuffer buffer = id_and_mail.mb_str(wxConvUTF8);
                // size_t length = strlen(buffer.data());

                // socket->Write(buffer.data(), length);
                // wxLogMessage("Written email and ID");

                std::string salt = GenerateRandomSalt(8);

                std::fstream encryption_data_opener("login_details.txt", std::ios::app);
                if (encryption_data_opener.is_open())
                {
                    encryption_data_opener
                        << salt
                        << "|"
                        << HashSaltAndID(salt, id_str)
                        << "|"
                        << email_str
                        << "\n";
                    encryption_data_opener.close(); // Close the file stream explicitly
                }
            }
            else if (existing_user_checkbox->IsChecked())
            {

                // load verification elements.
                wxString id = ID_text->GetValue();
                std::string id_str(id);
                if (checkForID(id_str) == true)
                {
                    std::cout << "Found the key" << std::endl;
                    ID_text->Hide();
                    EMAIL_text->Hide();
                    status_text->Hide();
                    first_instructions->Hide();
                    // header_text->Hide();
                    existing_user_text->Hide();
                    new_user_text->Hide();
                    submit_button->Hide();
                    new_user_checkbox->Hide();
                    existing_user_checkbox->Hide();
                    five_digit_code->Show();
                    instructions->Show();
                    check_button->Show();
                }
                else
                {
                    status_text->SetForegroundColour(*wxRED);
                    status_text->SetLabel("Could not find login, please restart!");
                    std::cout << "Key not found." << std::endl;
                }
            }

            // this->Destroy();
            // // Create and show the MainFrame
            // MainFrame *mainFrame = new MainFrame("Main Frame");
            // // Set the size of the frame
            // mainFrame->SetSize(800, 600); // Width = 800, Height = 600

            // // Alternatively, set the size to fit the display or other requirements
            // mainFrame->SetSize(wxSize(600, 400));
            // mainFrame->Show();
        }
    }
}
/**
 *
 */
wxString LoginFrame::getUserID()
{
    return ID_text->GetValue();
}

/**
 *
 */
std::vector<std::string> LoginFrame::split(const std::string &str, char delimiter)
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
std::string LoginFrame::retrieve_elt_2(std::string id)
{
    std::string filename = "login_details.txt";
    std::ifstream file(filename);
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::vector<std::string> tokens = split(line, '|');
            if (tokens.size() == 3)
            {
                std::string salt = tokens[0];
                std::string hash_str_ID = tokens[1];
                std::string email = tokens[2];
                if (HashSaltAndID(salt, id) == hash_str_ID)
                {
                    return hash_str_ID;
                }
            }
        }
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
    }
}

/**
 *
 */
std::string LoginFrame::retrieve_email(std::string id)
{
    std::string filename = "login_details.txt";
    std::ifstream file(filename);
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::vector<std::string> tokens = split(line, '|');
            if (tokens.size() == 3)
            {
                std::string salt = tokens[0];
                std::string hash_str_ID = tokens[1];
                std::string email = tokens[2];
                if (HashSaltAndID(salt, id) == hash_str_ID)
                {
                    return email;
                }
            }
        }
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
    }
}
/**
 *
 */
bool LoginFrame::checkForID(std::string id)
{
    std::string filename = "login_details.txt";
    std::ifstream file(filename);
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::vector<std::string> tokens = split(line, '|');
            if (tokens.size() == 3)
            {
                std::string salt = tokens[0];
                std::string hash_str_ID = tokens[1];
                std::string email = tokens[2];
                if (HashSaltAndID(salt, id) == hash_str_ID)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
    }
    return false;
}
/**
 *
 */
std::string LoginFrame::HashSaltAndID(const std::string &salt, const std::string &id)
{
    // Concatenate salt and ID
    std::string saltedInput = salt + id;

    // Prepare the hash object
    CryptoPP::SHA256 hash;

    std::string digest;
    CryptoPP::StringSource ss(saltedInput, true,
                              new CryptoPP::HashFilter(hash,
                                                       new CryptoPP::HexEncoder(
                                                           new CryptoPP::StringSink(digest))));

    return digest;
}

std::string LoginFrame::ToHexString(char *data, size_t length)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i)
    {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return std::string(ss.str());
}

/*
 *   LoginFrame::HandleSocketLost()
 *   @brief This function handles the socket being lost or a failed connection.
 */
void LoginFrame::HandleSocketLost()
{
}
/**
 *
 */
std::string LoginFrame::GenerateRandomSalt(size_t length)
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::SecByteBlock bytes(length);
    rng.GenerateBlock(bytes, bytes.size());

    std::string hex;
    CryptoPP::HexEncoder encoder;
    encoder.Attach(new CryptoPP::StringSink(hex));
    encoder.Put(bytes.data(), bytes.size());
    encoder.MessageEnd();

    return hex;
}

/*
 * SerializeHex function
 * @brief takes a secbyteblock and serializes to a hex econded string
 * @return a serialized hex string.
 */
std::string LoginFrame::SerializeHex(const CryptoPP::SecByteBlock &block)
{
    std::string PublicKey_hex;
    CryptoPP::HexEncoder encoder;
    encoder.Attach(new CryptoPP::StringSink(PublicKey_hex));
    encoder.Put(block.BytePtr(), block.size());
    encoder.MessageEnd();
    return PublicKey_hex;
}
/**
 *
 */
CryptoPP::SecByteBlock LoginFrame::decoder(const std::string &hexString)
{
    // Check if the input hex string has an even number of characters
    // std::string length = std::to_string(hexString.length());
    // wxString x = "CLIENT: recieved data in buffer has length " + length;
    // wxLogMessage(x);

    if (hexString.length() % 2 != 0)
    {
        wxLogMessage("CLIENT: Client key hex is not divisible by 2.");
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
        wxLogMessage("CLIENT: decoded size is 0");
        throw std::runtime_error("Failed to decode hex string.");
    }
    // wxLogMessage("Looking for issue...4");

    return decodedBytes;
}
/**
 *
 */

bool LoginFrame::RSA_verify(const CryptoPP::RSA::PublicKey &verification_key,
                            const std::vector<unsigned char> &message,
                            const std::string &signature)
{
    // Setup verifier
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(verification_key);

    bool result = false; // This will hold the result of verification

    // Create a StringSource that processes the message and the signature
    CryptoPP::StringSource ss(
        signature + std::string(message.begin(), message.end()), // Append message to signature for processing
        true,                                                    // Pump all data
        new CryptoPP::SignatureVerificationFilter(
            verifier,
            new CryptoPP::ArraySink(reinterpret_cast<CryptoPP::byte *>(&result), sizeof(result)),                        // Output the result to a boolean
            CryptoPP::SignatureVerificationFilter::PUT_RESULT | CryptoPP::SignatureVerificationFilter::SIGNATURE_AT_END) // SignatureVerificationFilter
    );

    // Return the result of the signature verification

    return result;
}

/**
 *
 */
void LoginFrame::updateChallengeString()
{
    this->challenge_string = GenerateRandomSalt(4);
}