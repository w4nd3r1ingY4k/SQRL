#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wx.h>
#include "Server.h"

class LoginFrame;
class MainFrame;

class App : public wxApp
{

public:
	bool OnInit();
	int OnExit();

	LoginFrame *GetLoginFrame() const { return loginFrame; }
	MainFrame *mainFrame;
	LoginFrame *loginFrame;

private:
		Server *server;
};

#endif
