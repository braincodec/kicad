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

class SCH_EDIT_FRAME;

static const wxString gsStyleName( _( "FLAT" ) );
static const wxString gsNameAutomatic( _( "::AUTOMATIC" ) );


/**
 * Subclass of wxClientData that holds an unsigned int.
 */
class wxUintClientData: public wxClientData
{
public:
    unsigned data;

    /**
     * Define a wxUintClientData from a given data item.
     */
    wxUintClientData( unsigned aData ) : data( aData ) {}

    /**
     * Return the stored data
     */
    unsigned GetData() const { return data; }

    /**
     * Change the stored data.
     */
    void SetData( unsigned aData ) { data = aData; }
};


/**
 * A wxDataViewModel that allows items to be fit to a custom sort order.
 */
class CustomSortModel: public wxDataViewTreeStore
{
    std::map<void*, size_t> m_map;

public:
    CustomSortModel(): wxDataViewTreeStore() {}

    bool HasDefaultCompare() const { return true; }
    int Compare( const wxDataViewItem& item1, const wxDataViewItem& item2,
            unsigned column, bool ascending ) const
    {
        size_t sortidx1 = SIZE_MAX, sortidx2 = SIZE_MAX;

        std::map<void*, size_t>::const_iterator it1 = m_map.find( item1.GetID() );
        std::map<void*, size_t>::const_iterator it2 = m_map.find( item2.GetID() );

        if( it1 != m_map.end() )
            sortidx1 = it1->second;
        if( it2 != m_map.end() )
            sortidx2 = it2->second;

        if( sortidx1 == sortidx2 )
            return wxDataViewTreeStore::Compare( item1, item2, column, ascending );
        else if( ascending )
            return sortidx2 - sortidx1;
        else
            return sortidx1 - sortidx2;
    }

    /**
     * Register a wxDataViewItem in the sort order.
     *
     * Items will be sorted in the order registered, with unregistered items sorted last in
     * the order provided by the parent class.
     */
    void RegisterItem( const wxDataViewItem& aItem )
    {
        m_map[aItem.GetID()] = m_map.size();
    }
};


/**
 * Automatically choose a symbol style based on the provided net.
 */
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
 * Make a wxDataViewTreeCtrl non-editable.
 *
 * Delete the editable column that is created by default in the wxDataViewTreeCtrl and
 * replace it with a non-editable one.
 */
static void fix_column( wxDataViewTreeCtrl* aCtrl )
{
    std::auto_ptr<wxDataViewRenderer> renderer( new wxDataViewIconTextRenderer(
                _( "wxDataViewIconText" ), wxDATAVIEW_CELL_INERT,
                wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL ) );

    wxDataViewColumn* col = aCtrl->GetColumn( 0 );
    aCtrl->DeleteColumn( col );
    aCtrl->AppendColumn( new wxDataViewColumn( wxEmptyString, renderer.release(), 0,
                wxDVC_DEFAULT_WIDTH, wxALIGN_CENTER, 0 ) );
}


/**
 * Return data from a wxStringClientData.
 *
 * Returns wxEmptyString if the given item is not a wxStringClientData.
 */
static wxString get_string_data( wxClientData* aClientData )
{
    if( !aClientData )
        return wxEmptyString;
    wxStringClientData* scd = dynamic_cast<wxStringClientData*>( aClientData );
    if( !scd )
        return wxEmptyString;
    return scd->GetData();
}


/**
 * Return the selected item's wxClientData.
 *
 * Returns NULL if there is no selection.
 */
static wxClientData* get_selected_data( wxDataViewTreeCtrl* aDataView )
{
    if( !aDataView )
        return NULL;
    wxDataViewItem sel = aDataView->GetSelection();
    if( !sel.IsOk() )
        return NULL;
    return aDataView->GetItemData( sel );
}


class DIALOG_EDIT_POWER: public DIALOG_EDIT_POWER_BASE
{
public:
    DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem );

private:
    SCH_EDIT_FRAME* m_parent;
    SCH_POWER* m_poweritem;

    // Map of "All Styles" item names to actual items
    std::map<wxString, wxDataViewItem> m_dvitems;

    // List of library parts
    boost::ptr_vector<SCH_POWER> m_library;

    /**
     * Configure and fill the Styles tree.
     */
    void PopulateStyles();

    /**
     * Configure and fill the Library tree.
     */
    void PopulateLib();

    bool TransferDataToWindow();
    bool TransferDataFromWindow();
    void OnCancelClick( wxCommandEvent& event );
    void OnEnterKey( wxCommandEvent& event );
    void OnToggleAdvanced( wxCommandEvent& event );
    void OnDataChanged( wxCommandEvent& event );
    void OnPreviewRepaint( wxPaintEvent& aRepaintEvent );
    void OnTreeChange( wxDataViewEvent& event );

    void OnStyleChange( wxDataViewEvent& event );
    void OnLibChange( wxDataViewEvent& event );

    /**
     * Transfer data to the window from a given SCH_POWER
     */
    bool TransferDataToWindow( SCH_POWER& aPoweritem );

    /**
     * Transfer data from the window to a given SCH_POWER
     */
    bool TransferDataFromWindow( SCH_POWER& aPoweritem );
};


/**
 * Rendering core function.
 * This prepares the drawing canvas and returns an offset at which the object should be
 * drawn.
 */
wxPoint render_core( wxDC* aDC, EDA_RECT aBBox, EDA_COLOR_T aBGcolor )
{
    aDC->SetBackground( aBGcolor == BLACK ? *wxBLACK_BRUSH : *wxWHITE_BRUSH );
    aDC->Clear();

    const wxSize dc_size = aDC->GetSize();
    aDC->SetDeviceOrigin( dc_size.x / 2, dc_size.y / 2 );

    const double xscale = (double) dc_size.x / aBBox.GetWidth();
    const double yscale = (double) dc_size.y / aBBox.GetHeight();
    const double scale = std::min( xscale, yscale ) * 0.85;

    aDC->SetUserScale( scale, scale );
    return -aBBox.Centre();
}


/**
 * Render a LIB_PART into a wxIcon.
 */
wxIcon render_part_as_icon( LIB_PART* aPart, int aWidth=48, int aHeight=48 )
{
    wxIcon icon;

    icon.SetWidth( aWidth );
    icon.SetHeight( aHeight );

    wxBitmap bmp( aWidth, aHeight );

    wxMemoryDC dc( bmp );

    wxPoint offset = render_core( &dc, aPart->GetBodyBoundingBox(1, 1), WHITE );

    aPart->Draw( NULL, &dc, offset, 1, 1, GR_COPY, UNSPECIFIED_COLOR,
            DefaultTransform, false, false, false, NULL );

    icon.CopyFromBitmap( bmp );
    return icon;
}


/**
 * Render an SCH_POWER into a wxIcon.
 */
wxIcon render_power_as_icon( SCH_POWER* aPart, int aWidth=48, int aHeight=48 )
{
    wxIcon icon;

    icon.SetWidth( aWidth );
    icon.SetHeight( aHeight );

    wxBitmap bmp( aWidth, aHeight );

    wxMemoryDC dc( bmp );

    wxPoint offset = render_core( &dc, aPart->GetBoundingBox(), WHITE );

    aPart->Draw( NULL, &dc, offset, GR_COPY, UNSPECIFIED_COLOR );

    icon.CopyFromBitmap( bmp );
    return icon;
}


DIALOG_EDIT_POWER::DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem )
    : DIALOG_EDIT_POWER_BASE( aParent ),
      m_parent( aParent ),
      m_poweritem( aPowerItem )
{
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

    CustomSortModel* model = new CustomSortModel();
    m_dvtStyles->AssociateModel( model );

    wxDataViewItem dviAutomatic = m_dvtStyles->AppendItem(
            wxDataViewItem( 0 ), wxT( "Automatic" ), 1,
            new wxStringClientData( gsNameAutomatic ) );
    model->RegisterItem( dviAutomatic );

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
    if( !TransferDataToWindow( *m_poweritem ) )
        return false;

    GetSizer()->Layout();
    Centre();
    return true;
}

bool DIALOG_EDIT_POWER::TransferDataToWindow( SCH_POWER& aPoweritem )
{
    if( !wxDialog::TransferDataToWindow() )
        return false;

    m_textNet->SetValue( aPoweritem.GetText() );
    m_textLabelText->SetValue( aPoweritem.GetVisibleText() );
    m_cbHideLabel->SetValue( aPoweritem.GetLabelHidden() );
    if( m_dvitems.count( aPoweritem.GetPartName() ) )
    {
        m_dvtStyles->Select( m_dvitems[aPoweritem.GetPartName()] );
        m_dvtStyles->EnsureVisible( m_dvitems[aPoweritem.GetPartName()] );
    }

    return true;
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow()
{
    m_parent->OnModify();
    if( m_poweritem->GetFlags() == 0 )
        m_parent->SaveCopyInUndoList( m_poweritem, UR_CHANGED );

    return TransferDataFromWindow( *m_poweritem );
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow( SCH_POWER& aPoweritem )
{
    if( !wxDialog::TransferDataFromWindow() )
        return false;

    aPoweritem.SetText( m_textNet->GetValue() );
    aPoweritem.SetVisibleText( m_textLabelText->GetValue() );
    aPoweritem.SetLabelHidden( m_cbHideLabel->GetValue() );

    wxString name = get_string_data( get_selected_data( m_dvtStyles ) );
    if( name == gsNameAutomatic || name == wxEmptyString )
        aPoweritem.SetPartName( heur_choose_style( m_textNet->GetValue() ) );
    else
        aPoweritem.SetPartName( name );
    aPoweritem.Resolve( Prj().SchLibs() );
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
    if( m_dvtLib->GetSelection().IsOk() )
    {
        m_textLabelText->SetValue( wxEmptyString );
        m_cbHideLabel->SetValue( false );
    }
    m_dvtLib->UnselectAll();
    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnLibChange( wxDataViewEvent& event )
{
    wxClientData* data = get_selected_data( m_dvtLib );
    if( !data )
        return;

    unsigned lib_item_index = dynamic_cast<wxUintClientData&>( *data ).GetData();

    TransferDataToWindow( m_library[lib_item_index] );

    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnPreviewRepaint( wxPaintEvent& aRepaintEvent )
{
    SCH_POWER temp_item( *m_poweritem );
    TransferDataFromWindow( temp_item );
    wxPaintDC dc( m_pPreview );
    EDA_COLOR_T bgcolor = m_parent->GetDrawBgColor();

    wxPoint offset = render_core( &dc, temp_item.GetBoundingBox(), bgcolor );

    wxSize sz = dc.GetSize();
    if( !sz.GetWidth() || !sz.GetHeight() )
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
