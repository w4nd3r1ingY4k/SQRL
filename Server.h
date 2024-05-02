#ifndef SERVER_H
#define SERVER_H

#include <wx/socket.h>
#include <wx/wx.h>
#include <wx/event.h>

class Server : public wxEvtHandler
{
public:
    Server();
    ~Server();
    bool Start(int port);
    void Stop();

private:
    wxSocketServer *server;
    void OnSocketEvent(wxSocketEvent &event);
};

#endif // SERVER_H
