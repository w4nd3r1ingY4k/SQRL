#include "Server.h"

Server::Server() : server(nullptr) {}

Server::~Server()
{
    Stop();
}

bool Server::Start(int port)
{
    // initialize an address using a port
    wxIPV4address addr;
    addr.Service(port);
    server = new wxSocketServer(addr, wxSOCKET_REUSEADDR);

    // if the server cannot be established, throw an error and return false
    if (!server->Ok())
    {
        wxLogError("Could not listen on the specified port!");
        return false;
    }

    server->SetEventHandler(*this, wxID_ANY);
    server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    server->Notify(true);
    Bind(wxEVT_SOCKET, &Server::OnSocketEvent, this);
    return true;
}

void Server::Stop()
{
    if (server)
    {
        server->Destroy();
        server = nullptr;
    }
}

void Server::OnSocketEvent(wxSocketEvent &event)
{
    wxSocketBase *socket = server->Accept(false);
    if (socket)
    {
        char buffer[100] = {0};                   // Ensuring the buffer is initialized
        socket->Read(buffer, sizeof(buffer) - 1); // Leave room for null terminator
        wxString msg(buffer, wxConvUTF8);
        wxLogMessage(msg + " Was sent to the server");

        if (msg.StartsWith("LOGIN"))
        {
            socket->Write("OK", 2);
        }
        else
        {
            socket->Write("INVALID", 7);
        }
        socket->Close();
        delete socket;
    }
}
