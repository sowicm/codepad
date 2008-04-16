///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 19 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __nameanddescbasedlg__
#define __nameanddescbasedlg__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NameAndDescBaseDlg
///////////////////////////////////////////////////////////////////////////////
class NameAndDescBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlName;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrlDescription;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonOK( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonCancel( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		NameAndDescBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Enter name and description:"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 594,220 ), long style = wxDEFAULT_DIALOG_STYLE );
		~NameAndDescBaseDlg();
	
};

#endif //__nameanddescbasedlg__
