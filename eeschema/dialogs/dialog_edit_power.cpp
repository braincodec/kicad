/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Chris Pavlina <pavlina.chris@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file dialog_edit_power.cpp
 * @brief Implementation of the power properties dialog.
 */

#include <fctsys.h>
#include <wx/valgen.h>
#include <schframe.h>
#include <base_units.h>

#include <class_drawpanel.h>
#include <general.h>
#include <drawtxt.h>
#include <confirm.h>
#include <sch_text.h>
#include <sch_power.h>
#include <typeinfo>
#include <boost/foreach.hpp>

#include <dialog_edit_power_base.h>

#include <iostream>


static const wxString gsStyleName( _( "FLAT" ) );
static const wxString gsNameAutomatic( _( "::AUTOMATIC" ) );

static wxString heur_choose_style( wxString aNet )
{
    if( aNet == _( "CGND" ) )
        return _( "CHASSIS" );

    if( aNet == _( "EGND" ) || aNet == _( "EARTH" ) )
        return _( "EARTH" );

    if( aNet.Find( _( "GND" ) ) != wxNOT_FOUND )
        return _( "GND" );

    if( aNet == _( "VSS" ) || aNet == _( "VEE" ) )
        return gsStyleName + _( "_DOWN" );

    if( aNet.StartsWith( _( "-" ) ) || aNet.EndsWith( _( "-" ) ) )
        return gsStyleName + _( "_DOWN" );

    return gsStyleName + _( "_UP" );
}


/**
 * Function create_renderer
 * Creates a wxDataViewRenderer for use in the trees
 */
static std::auto_ptr<wxDataViewRenderer> create_renderer()
{
    std::auto_ptr<wxDataViewRenderer> treerenderer( new wxDataViewIconTextRenderer(
                _( "wxDataViewIconText" ), wxDATAVIEW_CELL_INERT,
                wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL ) );
    return treerenderer;
}


/**
 * Function fix_column
 * Delete the editable column that is created by default in the wxDataViewTreeCtrl and
 * replace it with a non-editable column.
 */
static void fix_column( wxDataViewTreeCtrl* aCtrl )
{
    std::auto_ptr<wxDataViewRenderer> renderer = create_renderer();

    wxDataViewColumn* col = aCtrl->GetColumn( 0 );
    aCtrl->DeleteColumn( col );
    aCtrl->AppendColumn( new wxDataViewColumn( wxEmptyString, renderer.release(), 0,
                wxDVC_DEFAULT_WIDTH, wxALIGN_CENTER, 0 ) );
}


class SCH_EDIT_FRAME;

class DIALOG_EDIT_POWER: public DIALOG_EDIT_POWER_BASE
{
public:
    DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem );

private:
    SCH_EDIT_FRAME* m_parent;
    SCH_POWER* m_poweritem;
    std::map<wxString, wxDataViewItem> m_dvitems;

    bool TransferDataToWindow();
    bool TransferDataFromWindow( SCH_POWER* aPoweritem );
    bool TransferDataFromWindow();
    void OnCancelClick( wxCommandEvent& event );
    void OnEnterKey( wxCommandEvent& event );
    void OnToggleAdvanced( wxCommandEvent& event );
    void OnDataChanged( wxCommandEvent& event );
    void OnPreviewRepaint( wxPaintEvent& aRepaintEvent );
    void OnChangeStyle( wxDataViewEvent& event );
};


wxIcon render_part_as_icon( LIB_PART* aPart, int aWidth=48, int aHeight=48 )
{
    wxIcon icon;

    icon.SetWidth( aWidth );
    icon.SetHeight( aHeight );

    wxBitmap bmp( aWidth, aHeight );

    wxMemoryDC dc( bmp );
    EDA_COLOR_T bgcolor = WHITE;

    dc.SetBackground( bgcolor == BLACK ? *wxBLACK_BRUSH : *wxWHITE_BRUSH );
    dc.Clear();

    const wxSize dc_size = dc.GetSize();
    dc.SetDeviceOrigin( dc_size.x / 2, dc_size.y / 2 );

    EDA_RECT bbox = aPart->GetBodyBoundingBox(1, 1);
    const double xscale = (double) dc_size.x / bbox.GetWidth();
    const double yscale = (double) dc_size.y / bbox.GetHeight();
    const double scale = std::min( xscale, yscale ) * 0.85;

    dc.SetUserScale( scale, scale );
    wxPoint offset = -bbox.Centre();

    int width, height;
    dc.GetSize( &width, &height );
    if( !width || !height )
        return icon;

    aPart->Draw( NULL, &dc, offset, 1, 1, GR_COPY, UNSPECIFIED_COLOR,
            DefaultTransform, false, false, false, NULL );

    icon.CopyFromBitmap( bmp );
    return icon;
}


DIALOG_EDIT_POWER::DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem )
    : DIALOG_EDIT_POWER_BASE( aParent ),
      m_parent( aParent ),
      m_poweritem( aPowerItem )
{
    // Get a renderer for each tree
    std::auto_ptr<wxDataViewRenderer> style_renderer = create_renderer();

    m_stdButtonsOK->SetDefault();
    m_panAdvanced->Hide();
    m_textNet->SetFocus();

    // Draw the styles
    PART_LIB* lib = SCH_POWER::GetPartLib();
    wxArrayString lib_names;
    lib->GetEntryNames( lib_names );

    fix_column( m_dvtStyles );

    wxDataViewItem automatic = m_dvtStyles->AppendItem(
            wxDataViewItem( 0 ), _( "Automatic" ), 1,
            new wxStringClientData( gsNameAutomatic ) );
    m_dvtStyles->Select( automatic );

    wxDataViewItem allStyles = m_dvtStyles->InsertContainer(
            wxDataViewItem( 0 ), automatic, _( "All Styles" ), 1 );
    BOOST_FOREACH( wxString& each_name, lib_names )
    {
        wxDataViewItem item = m_dvtStyles->AppendItem(
                allStyles, each_name, -1,
                new wxStringClientData( each_name ) );

        LIB_PART* part = lib->FindPart( each_name );
        wxIcon icon = render_part_as_icon( part );
        m_dvtStyles->SetItemIcon( item, icon );
        m_dvitems[each_name] = item;
    }
}


bool DIALOG_EDIT_POWER::TransferDataToWindow()
{
    if( !wxDialog::TransferDataToWindow() )
        return false;

    m_textNet->SetValue( m_poweritem->GetText() );
    m_textLabelText->SetValue( m_poweritem->GetVisibleText() );
    m_cbHideLabel->SetValue( m_poweritem->GetLabelHidden() );
    if( m_dvitems.count( m_poweritem->GetPartName() ) )
    {
        m_dvtStyles->Select( m_dvitems[m_poweritem->GetPartName()] );
    }

    GetSizer()->Layout();
    GetSizer()->Fit( this );
    GetSizer()->SetSizeHints( this );
    Centre();
    return true;
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow()
{
    m_parent->OnModify();
    return TransferDataFromWindow( m_poweritem );
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow( SCH_POWER* aPoweritem )
{
    if( !wxDialog::TransferDataFromWindow() )
        return false;

    aPoweritem->SetText( m_textNet->GetValue() );
    aPoweritem->SetVisibleText( m_textLabelText->GetValue() );
    aPoweritem->SetLabelHidden( m_cbHideLabel->GetValue() );

    wxDataViewItem sel = m_dvtStyles->GetSelection();
    wxClientData* data = NULL;
    if( sel.IsOk() )
        data = m_dvtStyles->GetItemData( sel );

    if( data )
    {
        wxString name = dynamic_cast<wxStringClientData&>( *data ).GetData();

        if( name == gsNameAutomatic )
            aPoweritem->SetPartName( heur_choose_style( m_textNet->GetValue() ) );
        else
            aPoweritem->SetPartName( name );
    }
    else
    {
        aPoweritem->SetPartName( heur_choose_style( m_textNet->GetValue() ) );
    }
    aPoweritem->Resolve( Prj().SchLibs() );
    return true;
}


void DIALOG_EDIT_POWER::OnCancelClick( wxCommandEvent& aEvent )
{
    DIALOG_EDIT_POWER_BASE::OnCancelClick( aEvent );
    m_textNet->SetValue( m_poweritem->GetText() );
    m_parent->GetCanvas()->MoveCursorToCrossHair();
}


void DIALOG_EDIT_POWER::OnEnterKey( wxCommandEvent& aEvent )
{
    DIALOG_EDIT_POWER_BASE::OnOkClick( aEvent );
    Close();
}


void DIALOG_EDIT_POWER::OnToggleAdvanced( wxCommandEvent& aEvent )
{
    bool show = m_btnAdvanced->GetValue();
    if( show )
        m_panAdvanced->Show();
    else
        m_panAdvanced->Hide();
    GetSizer()->Layout();
}


void DIALOG_EDIT_POWER::OnDataChanged( wxCommandEvent& event )
{
    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnChangeStyle( wxDataViewEvent& event )
{
    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnPreviewRepaint( wxPaintEvent& aRepaintEvent )
{
    SCH_POWER temp_item( *m_poweritem );
    TransferDataFromWindow( &temp_item );
    wxPaintDC dc( m_pPreview );
    EDA_COLOR_T bgcolor = m_parent->GetDrawBgColor();

    dc.SetBackground( bgcolor == BLACK ? *wxBLACK_BRUSH : *wxWHITE_BRUSH );
    dc.Clear();

    const wxSize dc_size = dc.GetSize();
    dc.SetDeviceOrigin( dc_size.x / 2, dc_size.y / 2 );

    EDA_RECT bbox = temp_item.GetBoundingBox();
    const double xscale = (double) dc_size.x / bbox.GetWidth();
    const double yscale = (double) dc_size.y / bbox.GetHeight();
    const double scale = std::min( xscale, yscale ) * 0.85;

    dc.SetUserScale( scale, scale );
    wxPoint offset = -bbox.Centre();

    int width, height;
    dc.GetSize( &width, &height );
    if( !width || !height )
        return;

    temp_item.Draw( NULL, &dc, offset, GR_COPY, UNSPECIFIED_COLOR );
}


void SCH_EDIT_FRAME::EditPower( SCH_POWER* aPowerItem )
{
    if( !aPowerItem )
        return;

    DIALOG_EDIT_POWER dialog( this, aPowerItem );
    dialog.ShowModal();
    m_canvas->Refresh();
}
