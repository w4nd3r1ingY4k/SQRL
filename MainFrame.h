#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

private:
	void OnKeyEvent(wxKeyEvent &event);
	void OnButtonEncrypt(wxCommandEvent &event);
	void OnButtonDecrypt(wxCommandEvent &event);
	void OnButtonAllKeys(wxCommandEvent &event);
	void OnButtonClear(wxCommandEvent &event);
	wxTextCtrl *text_box;	 // Member variable for ID text box
	wxTextCtrl *text_box_pw; // Member variable for data text box
	wxScrolledWindow *repl_panel_top_left;
	wxFont font;
};
