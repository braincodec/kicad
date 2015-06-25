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

class wxUintClientData: public wxClientData
{
public:
    unsigned data;
    wxUintClientData( unsigned aData ) : data( aData ) {}
    unsigned GetData() const { return data; }
    void SetData( unsigned aData ) { data = aData; }
};


class wxEditPowerModel: public wxDataViewTreeStore
{
public:
    void* automaticID;
    std::map<void*, unsigned> id_to_sortidx;

    wxEditPowerModel(): wxDataViewTreeStore() {}
    bool HasDefaultCompare() const { return true; }
    int Compare( const wxDataViewItem& item1, const wxDataViewItem& item2,
            unsigned column, bool ascending ) const
    {
        unsigned sortidx1 = UINT_MAX, sortidx2 = UINT_MAX;

        std::map<void*, unsigned>::const_iterator it1 = id_to_sortidx.find( item1.GetID() );
        std::map<void*, unsigned>::const_iterator it2 = id_to_sortidx.find( item2.GetID() );

        if( it1 != id_to_sortidx.end() )
            sortidx1 = it1->second;
        if( it2 != id_to_sortidx.end() )
            sortidx2 = it2->second;

        if( sortidx1 == sortidx2 )
            return wxDataViewTreeStore::Compare( item1, item2, column, ascending );
        else if( ascending )
            return sortidx2 - sortidx1;
        else
            return sortidx1 - sortidx2;
    }
};


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
    boost::ptr_vector<SCH_POWER> m_library;

    void PopulateStyles();
    void PopulateLib();

    bool TransferDataToWindow( SCH_POWER* aPoweritem );
    bool TransferDataToWindow();
    bool TransferDataFromWindow( SCH_POWER* aPoweritem );
    bool TransferDataFromWindow();
    void OnCancelClick( wxCommandEvent& event );
    void OnEnterKey( wxCommandEvent& event );
    void OnToggleAdvanced( wxCommandEvent& event );
    void OnDataChanged( wxCommandEvent& event );
    void OnPreviewRepaint( wxPaintEvent& aRepaintEvent );
    void OnTreeChange( wxDataViewEvent& event );

    void OnStyleChange( wxDataViewEvent& event );
    void OnLibChange( wxDataViewEvent& event );
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


wxIcon render_power_as_icon( SCH_POWER* aPart, int aWidth=48, int aHeight=48 )
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

    EDA_RECT bbox = aPart->GetBoundingBox();
    const double xscale = (double) dc_size.x / bbox.GetWidth();
    const double yscale = (double) dc_size.y / bbox.GetHeight();
    const double scale = std::min( xscale, yscale ) * 0.85;

    dc.SetUserScale( scale, scale );
    wxPoint offset = -bbox.Centre();

    int width, height;
    dc.GetSize( &width, &height );
    if( !width || !height )
        return icon;

    aPart->Draw( NULL, &dc, offset, GR_COPY, UNSPECIFIED_COLOR );

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

    PopulateStyles();
    PopulateLib();
}


void DIALOG_EDIT_POWER::PopulateStyles()
{
    // Draw the styles
    PART_LIB* lib = SCH_POWER::GetPartLib();
    wxArrayString lib_names;
    lib->GetEntryNames( lib_names );
    fix_column( m_dvtStyles );
    wxEditPowerModel* model = new wxEditPowerModel();
    m_dvtStyles->AssociateModel( model );

    wxDataViewItem dviAutomatic = m_dvtStyles->AppendItem(
            wxDataViewItem( 0 ), wxT( "Automatic" ), 1,
            new wxStringClientData( gsNameAutomatic ) );
    model->id_to_sortidx[dviAutomatic.GetID()] = 1;

    wxDataViewItem dviAllStyles = m_dvtStyles->InsertContainer(
            wxDataViewItem( 0 ), dviAutomatic, wxT( "All Styles" ), 1 );
    BOOST_FOREACH( wxString& each_name, lib_names )
    {
        wxDataViewItem item = m_dvtStyles->AppendItem(
                dviAllStyles, each_name, -1,
                new wxStringClientData( each_name ) );

        LIB_PART* part = lib->FindPart( each_name );
        wxIcon icon = render_part_as_icon( part );
        m_dvtStyles->SetItemIcon( item, icon );
        m_dvitems[each_name] = item;
    }

    m_dvtStyles->Select( dviAutomatic );
}


struct libspec {
    wxString netname;
    wxString visiblename;
    wxString partname;
    bool namevisible;
};


void DIALOG_EDIT_POWER::PopulateLib()
{
    // Draw the styles
    fix_column( m_dvtLib );

    wxDataViewItem gnd;

    wxDataViewItem dviCommon = m_dvtLib->AppendContainer(
            wxDataViewItem( 0 ), _( "Common Ports" ), 1 );

    static const struct libspec common_ports[] = {
        { _( "GND" ), wxEmptyString, _( "GND" ), false },
    };

    BOOST_FOREACH( const struct libspec& port, common_ports )
    {
        std::auto_ptr<SCH_POWER> power_item( new SCH_POWER( wxPoint( 0, 0 ), port.netname ) );
        power_item->SetVisibleText( port.visiblename );
        power_item->SetPartName( port.partname );
        power_item->SetLabelHidden( !port.namevisible );
        power_item->Resolve( Prj().SchLibs() );

        wxDataViewItem dvitem = m_dvtLib->AppendItem(
                dviCommon, port.netname, -1, new wxUintClientData( m_library.size() ) );
        wxIcon icon = render_power_as_icon( power_item.get() );
        m_dvtLib->SetItemIcon( dvitem, icon );
        m_library.push_back( power_item.release() );
    }
    m_dvtLib->Expand( dviCommon );
}


bool DIALOG_EDIT_POWER::TransferDataToWindow()
{
    if( !TransferDataToWindow( m_poweritem ) )
        return false;

    GetSizer()->Layout();
    Centre();
    return true;
}

bool DIALOG_EDIT_POWER::TransferDataToWindow( SCH_POWER* aPoweritem )
{
    if( !wxDialog::TransferDataToWindow() )
        return false;

    m_textNet->SetValue( aPoweritem->GetText() );
    m_textLabelText->SetValue( aPoweritem->GetVisibleText() );
    m_cbHideLabel->SetValue( aPoweritem->GetLabelHidden() );
    if( m_dvitems.count( aPoweritem->GetPartName() ) )
    {
        m_dvtStyles->Select( m_dvitems[aPoweritem->GetPartName()] );
        m_dvtStyles->EnsureVisible( m_dvitems[aPoweritem->GetPartName()] );
    }

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


void DIALOG_EDIT_POWER::OnTreeChange( wxDataViewEvent& event )
{
    if( event.GetEventObject() == m_dvtStyles )
        OnStyleChange( event );
    else if( event.GetEventObject() == m_dvtLib )
        OnLibChange( event );
}


void DIALOG_EDIT_POWER::OnStyleChange( wxDataViewEvent& event )
{
    // If a library part was selected, clear out the advanced settings from it
    wxDataViewItem sel = m_dvtLib->GetSelection();
    if( sel.IsOk() )
    {
        m_textLabelText->SetValue( wxEmptyString );
        m_cbHideLabel->SetValue( false );
    }
    m_dvtLib->UnselectAll();
    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnLibChange( wxDataViewEvent& event )
{
    wxDataViewItem sel = m_dvtLib->GetSelection();
    wxClientData* data = NULL;
    if( sel.IsOk() )
        data = m_dvtLib->GetItemData( sel );
    if( !data )
        return;

    unsigned lib_item_index = dynamic_cast<wxUintClientData&>( *data ).GetData();

    TransferDataToWindow( &m_library[lib_item_index] );

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
