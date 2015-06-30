///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 13 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_EDIT_POWER_BASE_H__
#define __DIALOG_EDIT_POWER_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DIALOG_SHIM;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_EDIT_POWER_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_EDIT_POWER_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxStaticText* m_stInfo;
		wxStaticText* m_stLib;
		wxDataViewTreeCtrl* m_dvtLib;
		wxStaticText* m_stNet;
		wxTextCtrl* m_textNet;
		wxStaticText* m_stLabelText;
		wxTextCtrl* m_textLabelText;
		wxCheckBox* m_cbHideLabel;
		wxStaticText* m_stStyles;
		wxDataViewTreeCtrl* m_dvtStyles;
		wxStaticText* m_stPreview;
		wxPanel* m_pPreview;
		wxStdDialogButtonSizer* m_stdButtons;
		wxButton* m_stdButtonsOK;
		wxButton* m_stdButtonsCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnLibKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnTreeChange( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnDataChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPreviewRepaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString m_labelText; 
		
		DIALOG_EDIT_POWER_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Power Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 409,511 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_EDIT_POWER_BASE();
	
};

#endif //__DIALOG_EDIT_POWER_BASE_H__
