#include <wx/wx.h>
#include "App.h"
#include "MainFrame.h"
#include "LoginFrame.h"
#include "Server.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	server = new Server();
	if (!server->Start(3000))
	{ // Assuming Start returns bool indicating success/failure
		wxLogError("Server failed to start on port 3000.");
		return false; // Prevent the app from continuing if the server fails to start
	}

	wxLogMessage("Server started successfully on port 3000.");

	LoginFrame *lf = new LoginFrame("Log In");
	lf->SetClientSize(675, 350);
	lf->Center();
	lf->Show(true);
	SetTopWindow(lf); // Set the login frame as the top window

	return true;
}

int App::OnExit()
{
	if (server)
	{
		server->Stop();
		delete server;
	}
	return wxApp::OnExit();
}
