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
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPreview = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
	m_pPreview->SetMinSize( wxSize( 150,150 ) );
	
	bSizer71->Add( m_pPreview, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer2->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboStyle = new wxBitmapComboBox( this, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	fgSizer2->Add( m_cboStyle, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer2->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_textText, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_cbShowText = new wxCheckBox( this, wxID_ANY, _("Show Text"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_cbShowText, 1, wxALL, 5 );
	
	
	bSizer71->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer6->Add( bSizer71, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_stNet = new wxStaticText( this, wxID_ANY, _("Net:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stNet->Wrap( -1 );
	bSizer10->Add( m_stNet, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboNet = new wxBitmapComboBox( this, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_cboNet->SetValidator( wxTextValidator( wxFILTER_EXCLUDE_CHAR_LIST, &m_valcboNet ) );
	
	bSizer10->Add( m_cboNet, 1, wxALL, 5 );
	
	
	bSizer6->Add( bSizer10, 0, wxEXPAND, 5 );
	
	m_stdButtons = new wxStdDialogButtonSizer();
	m_stdButtonsOK = new wxButton( this, wxID_OK );
	m_stdButtons->AddButton( m_stdButtonsOK );
	m_stdButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_stdButtons->AddButton( m_stdButtonsCancel );
	m_stdButtons->Realize();
	
	bSizer6->Add( m_stdButtons, 0, wxALL|wxEXPAND, 2 );
	
	
	bSizer6->Add( 0, 5, 0, wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizer6, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	// Connect Events
	m_pPreview->Connect( wxEVT_PAINT, wxPaintEventHandler( DIALOG_EDIT_POWER_BASE::OnPreviewRepaint ), NULL, this );
	m_cboStyle->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCombo ), NULL, this );
	m_textText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_cbShowText->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_cboNet->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCombo ), NULL, this );
	m_cboNet->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_stdButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCancelClick ), NULL, this );
	m_stdButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnOkClick ), NULL, this );
}

DIALOG_EDIT_POWER_BASE::~DIALOG_EDIT_POWER_BASE()
{
	// Disconnect Events
	m_pPreview->Disconnect( wxEVT_PAINT, wxPaintEventHandler( DIALOG_EDIT_POWER_BASE::OnPreviewRepaint ), NULL, this );
	m_cboStyle->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCombo ), NULL, this );
	m_textText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_cbShowText->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_cboNet->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCombo ), NULL, this );
	m_cboNet->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_stdButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCancelClick ), NULL, this );
	m_stdButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnOkClick ), NULL, this );
	
}
