///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 13 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dialog_edit_power_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_EDIT_POWER_BASE::DIALOG_EDIT_POWER_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_stInfo = new wxStaticText( this, wxID_ANY, _("Select a premade symbol on the left, or choose a net and style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stInfo->Wrap( -1 );
	bMainSizer->Add( m_stInfo, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_stLib = new wxStaticText( this, wxID_ANY, _("Library of symbols:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLib->Wrap( -1 );
	bSizer8->Add( m_stLib, 0, wxALL, 5 );
	
	m_dvlLib = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_dvlLib, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer7->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_stNet = new wxStaticText( this, wxID_ANY, _("Net:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stNet->Wrap( -1 );
	bSizer6->Add( m_stNet, 0, wxALL, 5 );
	
	m_textNet = new wxTextCtrl( this, wxID_VALUESINGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textNet->SetValidator( wxTextValidator( wxFILTER_EXCLUDE_CHAR_LIST, &m_labelText ) );
	
	bSizer6->Add( m_textNet, 1, wxLEFT, 5 );
	
	
	bSizer9->Add( bSizer6, 0, wxEXPAND, 5 );
	
	m_stStyles = new wxStaticText( this, wxID_ANY, _("Styles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stStyles->Wrap( -1 );
	bSizer9->Add( m_stStyles, 0, wxALL, 5 );
	
	m_dvlStyles = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_dvlStyles, 2, wxALL|wxEXPAND, 5 );
	
	m_stPreview = new wxStaticText( this, wxID_ANY, _("Preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stPreview->Wrap( -1 );
	bSizer9->Add( m_stPreview, 0, wxALL, 5 );
	
	m_pPreview = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer9->Add( m_pPreview, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer7->Add( bSizer9, 1, wxEXPAND, 5 );
	
	
	bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_stdButtons = new wxStdDialogButtonSizer();
	m_stdButtonsOK = new wxButton( this, wxID_OK );
	m_stdButtons->AddButton( m_stdButtonsOK );
	m_stdButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_stdButtons->AddButton( m_stdButtonsCancel );
	m_stdButtons->Realize();
	
	bMainSizer->Add( m_stdButtons, 0, wxALL|wxEXPAND, 12 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	// Connect Events
	m_stdButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCancelClick ), NULL, this );
	m_stdButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnOkClick ), NULL, this );
}

DIALOG_EDIT_POWER_BASE::~DIALOG_EDIT_POWER_BASE()
{
	// Disconnect Events
	m_stdButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCancelClick ), NULL, this );
	m_stdButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnOkClick ), NULL, this );
	
}
