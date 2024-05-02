#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wx.h>
#include "Server.h"

class App : public wxApp
{

public:
	bool OnInit();
	int OnExit();

private:
	Server *server;
};

#endif
