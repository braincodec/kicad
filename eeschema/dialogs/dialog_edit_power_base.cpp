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
	
	m_btnAdvanced = new wxToggleButton( this, wxID_ANY, _("Advanced options"), wxDefaultPosition, wxDefaultSize, 0 );
	bMainSizer->Add( m_btnAdvanced, 0, wxALL, 5 );
	
	m_panAdvanced = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	mbsz_Advanced = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_stLabelText = new wxStaticText( m_panAdvanced, wxID_ANY, _("Label text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLabelText->Wrap( -1 );
	bSizer14->Add( m_stLabelText, 0, wxALL, 5 );
	
	m_textLabelText = new wxTextCtrl( m_panAdvanced, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_textLabelText, 1, wxALL, 2 );
	
	
	bSizer14->Add( 5, 0, 0, wxEXPAND, 5 );
	
	
	mbsz_Advanced->Add( bSizer14, 0, wxEXPAND, 5 );
	
	m_cbHideLabel = new wxCheckBox( m_panAdvanced, wxID_ANY, _("Hide label"), wxDefaultPosition, wxDefaultSize, 0 );
	mbsz_Advanced->Add( m_cbHideLabel, 0, wxALL, 5 );
	
	
	m_panAdvanced->SetSizer( mbsz_Advanced );
	m_panAdvanced->Layout();
	mbsz_Advanced->Fit( m_panAdvanced );
	bMainSizer->Add( m_panAdvanced, 0, wxEXPAND | wxALL, 5 );
	
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
	
	bSizer6->Add( m_textNet, 1, wxLEFT, 0 );
	
	
	bSizer6->Add( 5, 0, 0, wxEXPAND, 0 );
	
	
	bSizer9->Add( bSizer6, 0, wxEXPAND, 5 );
	
	m_stStyles = new wxStaticText( this, wxID_ANY, _("Styles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stStyles->Wrap( -1 );
	bSizer9->Add( m_stStyles, 0, wxALL, 5 );
	
	m_dvlStyles = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_dvlStyles, 1, wxALL|wxEXPAND, 5 );
	
	m_stPreview = new wxStaticText( this, wxID_ANY, _("Preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stPreview->Wrap( -1 );
	bSizer9->Add( m_stPreview, 0, wxALL, 5 );
	
	m_pPreview = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_pPreview->SetMinSize( wxSize( 150,150 ) );
	
	bSizer9->Add( m_pPreview, 0, wxEXPAND | wxALL, 5 );
	
	
	bSizer7->Add( bSizer9, 1, wxEXPAND, 5 );
	
	
	bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_stdButtons = new wxStdDialogButtonSizer();
	m_stdButtonsOK = new wxButton( this, wxID_OK );
	m_stdButtons->AddButton( m_stdButtonsOK );
	m_stdButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_stdButtons->AddButton( m_stdButtonsCancel );
	m_stdButtons->Realize();
	
	bMainSizer->Add( m_stdButtons, 0, wxALL|wxEXPAND, 2 );
	
	
	bMainSizer->Add( 0, 5, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	// Connect Events
	m_btnAdvanced->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnToggleAdvanced ), NULL, this );
	m_textLabelText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_cbHideLabel->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_textNet->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_EDIT_POWER_BASE::OnChangeStyle ) );
	m_pPreview->Connect( wxEVT_PAINT, wxPaintEventHandler( DIALOG_EDIT_POWER_BASE::OnPreviewRepaint ), NULL, this );
	m_stdButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCancelClick ), NULL, this );
	m_stdButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnOkClick ), NULL, this );
}

DIALOG_EDIT_POWER_BASE::~DIALOG_EDIT_POWER_BASE()
{
	// Disconnect Events
	m_btnAdvanced->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnToggleAdvanced ), NULL, this );
	m_textLabelText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_cbHideLabel->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	m_textNet->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnDataChanged ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_EDIT_POWER_BASE::OnChangeStyle ) );
	m_pPreview->Disconnect( wxEVT_PAINT, wxPaintEventHandler( DIALOG_EDIT_POWER_BASE::OnPreviewRepaint ), NULL, this );
	m_stdButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnCancelClick ), NULL, this );
	m_stdButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_EDIT_POWER_BASE::OnOkClick ), NULL, this );
	
}
