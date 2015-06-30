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

// Libraries
#include <boost/foreach.hpp>

// KiCad-wide utilities
#include <class_drawpanel.h>
#include <class_eda_rect.h>
#include <kiface_i.h>

// Eeschema
#include <eeschema_config.h>
#include <sch_component.h>
#include <sch_field.h>
#include <sch_power.h>
#include <schframe.h>

// Base class
#include <dialog_edit_power_base.h>

class SCH_EDIT_FRAME;

static const wxString gsStyleName( _( "FLAT" ) );
static const wxString gsNameAutomatic( _( "::AUTOMATIC" ) );


/**
 * Subclass of wxClientData that holds any POD.
 */
template<typename T>
class wxPODClientData: public wxClientData
{
public:
    T data;

    wxPODClientData( const T& aData ) : data( aData ) {}

    /**
     * Return the stored data
     */
    const T& GetData() const { return data; }

    /**
     * Change the stored data.
     */
    void SetData( const T& aData ) { data = aData; }
};


/**
 * Get the data from a wxClientData.
 *
 * You must provide two types: the expected subclass of wxClientData, and the expected
 * data type that T::GetData will return.
 *
 * If aClientData is NULL or not an instance of T, returns the default value for type
 * BaseT. Otherwise, casts and returns the data.
 */
template<typename T, typename BaseT>
static BaseT get_client_data( wxClientData* aClientData )
{
    static BaseT default_value;
    if( !aClientData )
        return default_value;
    T* converted = dynamic_cast<T*>( aClientData );
    if( !converted )
        return default_value;
    return converted->GetData();
}


template<typename T>
static T get_pod( wxClientData* aClientData )
{
    return get_client_data<wxPODClientData<T>, T>( aClientData );
}


static wxString get_string_data( wxClientData* aClientData )
{
    return get_client_data<wxStringClientData, wxString>( aClientData );
}


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
 * Used to hold the data of a SCH_POWER without a full instance.
 */
struct libspec {
    wxString netname;
    wxString visiblename;
    wxString partname;
    bool namevisible;
};


/**
 * Automatically choose a symbol style based on the provided net.
 *
 * Populates the provided SCH_POWER with the settings.
 */
static void heur_choose_style( wxString aNet, SCH_POWER* aItem )
{
    wxString powerstyle;
    Kiface().KifaceSettings()->Read( DEFAULT_POWER_STYLE_KEY, &powerstyle, DEFAULTPOWERSTYLE );

    aItem->SetText( aNet );
    aItem->SetVisibleText( wxEmptyString );
    aItem->SetLabelHidden( false );

    if( aNet == _( "CGND" ) )
        aItem->SetPartName( _( "CHASSIS" ) );

    else if( aNet == _( "EGND" ) || aNet == _( "EARTH" ) )
        aItem->SetPartName( _( "EARTH" ) );

    else if( aNet == _( "GND" ) )
    {
        aItem->SetPartName( _( "GND" ) );
        aItem->SetLabelHidden( true );
    }
    else if( aNet.Find( _( "GND" ) ) != wxNOT_FOUND )
        aItem->SetPartName( _( "GND" ) );

    else if( aNet == _( "VSS" ) || aNet == _( "VEE" ) )
        aItem->SetPartName( powerstyle + _( "_DOWN" ) );

    else if( aNet.StartsWith( _( "-" ) ) || aNet.EndsWith( _( "-" ) ) )
        aItem->SetPartName( powerstyle + _( "_DOWN" ) );

    else
        aItem->SetPartName( powerstyle + _( "_UP" ) );
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


/**
 * Function get_type
 * Fills a vector with all of the project's items of a given type
 */
template<typename T>
static void get_type( std::vector<T*>& aComponents )
{
    SCH_SCREENS screens;
    for( SCH_SCREEN* screen = screens.GetFirst(); screen; screen = screens.GetNext() )
    {
        for( SCH_ITEM* item = screen->GetDrawItems(); item; item = item->Next() )
        {
            T* castitem = dynamic_cast<T*>( item );
            if( castitem )
                aComponents.push_back( castitem );
        }
    }
}


/**
 * History is kept in two ways. This keeps a per-application-instance history of power ports
 * placed, allowing them to remain in the history after being deleted. Then, history is also
 * compiled from the schematic itself, allowing the history to be immediately populated upon
 * opening a schematic.
 */
static boost::ptr_vector<SCH_POWER> History;


class DIALOG_EDIT_POWER: public DIALOG_EDIT_POWER_BASE
{
public:
    DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem );

private:
    SCH_EDIT_FRAME* m_parent;
    SCH_POWER* m_poweritem;
    SCH_POWER m_dummy;

    // Map of "All Styles" item names to actual items
    std::map<wxString, wxDataViewItem> m_dvitems;

    /**
     * List of library parts
     * All the items in the Library tree hold a wxPODClientData which indicates the index in
     * this vector at which the part will be found.
     */
    boost::ptr_vector<SCH_POWER> m_library;

    /**
     * Configure and fill the Styles tree.
     */
    void PopulateStyles();

    /**
     * Configure and fill the Library tree.
     */
    void PopulateLib();

    // Configure and fill sections of the Library tree.
    void PopulateCommonPorts();
    void PopulateHiddenPins();
    void PopulateHistory();

    bool TransferDataToWindow();
    bool TransferDataFromWindow();
    void OnCancelClick( wxCommandEvent& event );
    void OnEnterKey( wxCommandEvent& event );
    void OnToggleAdvanced( wxCommandEvent& event );
    void OnDataChanged( wxCommandEvent& event );
    void OnPreviewRepaint( wxPaintEvent& aRepaintEvent );
    void OnTreeChange( wxDataViewEvent& event );
    void OnLibKey( wxKeyEvent& event );

    void OnStyleChange( wxDataViewEvent& event );
    void OnLibChange( wxDataViewEvent& event );

    /**
     * Add a template port to the library using heuristics
     */
    void AddLibPort( wxDataViewItem* aParent, const wxString& aNet );

    /**
     * Add a template port from a SCH_POWER item. Takes ownership of the pointer.
     */
    void AddLibPort( wxDataViewItem* aParent, SCH_POWER* aItem );

    /**
     * Transfer data to the window from a given SCH_POWER
     * @param aUpdateNet - if false, the Net field will not be updated. This prevents
     *  infinite recursion when running this _from_ the Net field's change event.
     */
    bool TransferDataToWindow( SCH_POWER* aItem, bool aUpdateNet = true );

    /**
     * Transfer data from the window to a given SCH_POWER
     */
    bool TransferDataFromWindow( SCH_POWER* aItem );
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
wxIcon render_part_as_icon( const wxString& aName, int aWidth=48, int aHeight=48 )
{
    wxIcon icon;

    icon.SetWidth( aWidth );
    icon.SetHeight( aHeight );

    icon.CopyFromBitmap( SCH_POWER::RenderStylePreview( aName, aWidth, aHeight) );
    return icon;
}


/**
 * Render an SCH_POWER into a wxIcon.
 */
wxIcon render_power_as_icon( SCH_POWER* aItem, int aWidth=48, int aHeight=48 )
{
    wxIcon icon;

    icon.SetWidth( aWidth );
    icon.SetHeight( aHeight );

    wxBitmap bmp( aWidth, aHeight );

    wxMemoryDC dc( bmp );

    wxPoint offset = render_core( &dc, aItem->GetBoundingBox(), WHITE );

    aItem->Draw( NULL, &dc, offset, GR_COPY, UNSPECIFIED_COLOR );

    icon.CopyFromBitmap( bmp );
    return icon;
}


DIALOG_EDIT_POWER::DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem )
    : DIALOG_EDIT_POWER_BASE( aParent ),
      m_parent( aParent ),
      m_poweritem( aPowerItem ),
      m_dummy( wxPoint( 0, 0 ), wxEmptyString )
{
    m_stdButtonsOK->SetDefault();
    m_textNet->SetFocus();

    wxTextValidator* validator = static_cast<wxTextValidator*>( m_textNet->GetValidator() );
    wxArrayString excludes;
    excludes.Add( wxT( " " ) );
    validator->SetExcludes( excludes );

    PopulateHistory();
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

        wxIcon icon = render_part_as_icon( each_name );
        m_dvtStyles->SetItemIcon( item, icon );
        m_dvitems[each_name] = item;
    }

    m_dvtStyles->Select( dviAutomatic );
}


void DIALOG_EDIT_POWER::PopulateCommonPorts()
{
    wxDataViewItem dviCommon = m_dvtLib->AppendContainer(
            wxDataViewItem( 0 ), _( "Common Ports" ), 1 );

    static const struct libspec common_ports[] = {
        { _( "GND" ), wxEmptyString, _( "GND" ), false },
    };

    BOOST_FOREACH( const struct libspec& port, common_ports )
    {
        std::auto_ptr<SCH_POWER> item( new SCH_POWER( wxPoint( 0, 0 ), port.netname ) );
        item->SetVisibleText( port.visiblename );
        item->SetPartName( port.partname );
        item->SetLabelHidden( !port.namevisible );

        AddLibPort( &dviCommon, item.release() );
    }
    m_dvtLib->Expand( dviCommon );
}


void DIALOG_EDIT_POWER::PopulateHiddenPins()
{
    wxDataViewItem dviHP = m_dvtLib->AppendContainer(
            wxDataViewItem( 0 ), _( "Hidden Pins" ), 1 );

    std::vector<SCH_COMPONENT*> components;
    std::set<wxString> pin_name_set;
    get_type( components );

    // Gather all the hidden pins from components
    BOOST_FOREACH( SCH_COMPONENT* each_comp, components )
    {
        std::vector<wxString> pin_names;
        each_comp->GetPowerPortNames( pin_names );
        BOOST_FOREACH( const wxString& each_pin_name, pin_names )
        {
            pin_name_set.insert( each_pin_name );
        }
    }

    // Make template power ports for all of them
    BOOST_FOREACH( const wxString& each_pin_name, pin_name_set )
    {
        AddLibPort( &dviHP, each_pin_name );
    }

    // Remove the list if there's nothing in it
    if( !pin_name_set.size() )
    {
        m_dvtLib->DeleteItem( dviHP );
    }
}


void DIALOG_EDIT_POWER::PopulateHistory()
{
    wxDataViewItem dviHist = m_dvtLib->AppendContainer(
            wxDataViewItem( 0 ), _( "History" ), 1 );

    std::vector<SCH_POWER*> ports_from_history;
    boost::ptr_vector<SCH_POWER> ports_filtered;

    // Get history from the schematics
    get_type<SCH_POWER>( ports_from_history );

    // Get the local history
    BOOST_FOREACH( SCH_POWER& history_item, History )
        ports_from_history.push_back( &history_item );

    BOOST_FOREACH( SCH_POWER* source_port, ports_from_history )
    {
        bool found = false;
        BOOST_FOREACH( SCH_POWER& each_filtered, ports_filtered )
            if( each_filtered == *source_port )
                found = true;

        if( !found )
            ports_filtered.push_back( new SCH_POWER( *source_port ) );
    }

    // Remove the list if there's nothing in it
    if( !ports_filtered.size() )
    {
        m_dvtLib->DeleteItem( dviHist );
    }
    else
    {
        while( ports_filtered.size() )
        {
            AddLibPort( &dviHist, ports_filtered.release( ports_filtered.end() - 1 ).release() );
        }
        m_dvtLib->Expand( dviHist );
    }
}


void DIALOG_EDIT_POWER::PopulateLib()
{
    // Draw the styles
    fix_column( m_dvtLib );
    m_dvtLib->DeleteAllItems();

    PopulateHistory();
    PopulateCommonPorts();
    PopulateHiddenPins();
}


bool DIALOG_EDIT_POWER::TransferDataToWindow()
{
    if( !TransferDataToWindow( m_poweritem ) )
        return false;

    GetSizer()->Layout();
    Centre();
    return true;
}


bool DIALOG_EDIT_POWER::TransferDataToWindow( SCH_POWER* aItem, bool aUpdateNet )
{
    if( aUpdateNet && !wxDialog::TransferDataToWindow() )
        return false;

    if( aUpdateNet )
        m_textNet->SetValue( aItem->GetText() );

    m_textLabelText->SetValue( aItem->GetVisibleText() );
    m_cbHideLabel->SetValue( aItem->GetLabelHidden() );

    if( aUpdateNet && m_dvitems.count( aItem->GetPartName() ) )
    {
        m_dvtStyles->Select( m_dvitems[aItem->GetPartName()] );
        m_dvtStyles->EnsureVisible( m_dvitems[aItem->GetPartName()] );
    }

    return true;
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow()
{
    m_parent->OnModify();
    if( m_poweritem->GetFlags() == 0 )
        m_parent->SaveCopyInUndoList( m_poweritem, UR_CHANGED );

    if( !TransferDataFromWindow( m_poweritem ) )
        return false;

    if( m_poweritem->GetText() != wxEmptyString )
        History.push_back( new SCH_POWER( *m_poweritem ) );

    return true;
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow( SCH_POWER* aItem )
{
    if( !wxDialog::TransferDataFromWindow() )
        return false;

    aItem->SetText( m_textNet->GetValue() );
    aItem->SetVisibleText( m_textLabelText->GetValue() );
    aItem->SetLabelHidden( m_cbHideLabel->GetValue() );

    wxString name = get_string_data( get_selected_data( m_dvtStyles ) );
    if( name == gsNameAutomatic || name == wxEmptyString )
    {
        heur_choose_style( m_textNet->GetValue(), &m_dummy );
        aItem->SetPartName( m_dummy.GetPartName() );
    }
    else
        aItem->SetPartName( name );
    aItem->Resolve( Prj().SchLibs() );
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


void DIALOG_EDIT_POWER::OnDataChanged( wxCommandEvent& event )
{
    static bool recursion_guard = false;
    if( recursion_guard ) return;
    recursion_guard = true;

    if( event.GetEventObject() == m_textNet )
    {
        wxString name = get_string_data( get_selected_data( m_dvtStyles ) );
        if( name == gsNameAutomatic || name == wxEmptyString )
        {
            heur_choose_style( m_textNet->GetValue(), &m_dummy );
            TransferDataToWindow( &m_dummy, false );
        }
    }
    else if( event.GetEventObject() == m_textLabelText ||
             event.GetEventObject() == m_cbHideLabel )
    {
        wxString name = get_string_data( get_selected_data( m_dvtStyles ) );
        heur_choose_style( m_textNet->GetValue(), &m_dummy );
        if( name == gsNameAutomatic &&
            ( m_dummy.GetVisibleText() != m_textLabelText->GetValue() ||
              m_dummy.GetLabelHidden() != m_cbHideLabel->GetValue() ) )
        {
            m_dvtStyles->Select( m_dvitems[m_dummy.GetPartName()] );
            m_dvtStyles->EnsureVisible( m_dvitems[m_dummy.GetPartName()] );
        }
    }

    m_pPreview->Refresh();

    recursion_guard = false;
}


void DIALOG_EDIT_POWER::OnTreeChange( wxDataViewEvent& event )
{
    if( event.GetEventObject() == m_dvtStyles )
        OnStyleChange( event );
    else if( event.GetEventObject() == m_dvtLib )
        OnLibChange( event );
}


void DIALOG_EDIT_POWER::OnLibKey( wxKeyEvent& event )
{
    if( event.GetEventObject() != m_dvtLib )
        return;
    if( event.GetKeyCode() == WXK_DELETE )
    {
        // Delete the object from the history!
        wxClientData* data = get_selected_data( m_dvtLib );
        if( !data )
            return;
        unsigned lib_item_index = get_pod<unsigned>( data );
        SCH_POWER& prototype = m_library[lib_item_index];

        for( boost::ptr_vector<SCH_POWER>::iterator it = History.begin();
                it < History.end(); ++it )
        {
            if( prototype == *it )
            {
                it = History.erase( it );
            }
        }

        PopulateLib();
    }
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

    unsigned lib_item_index = get_pod<unsigned>( data );

    TransferDataToWindow( &m_library[lib_item_index] );

    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::AddLibPort( wxDataViewItem* aParent, const wxString& aNet )
{
    std::auto_ptr<SCH_POWER> power_item( new SCH_POWER( wxPoint( 0, 0 ), aNet ) );
    heur_choose_style( aNet, power_item.get() );
    AddLibPort( aParent, power_item.release() );
}


void DIALOG_EDIT_POWER::AddLibPort( wxDataViewItem* aParent, SCH_POWER* aPort )
{
    std::auto_ptr<SCH_POWER> power_item( aPort );
    power_item->Resolve( Prj().SchLibs() );

    wxDataViewItem dvitem = m_dvtLib->AppendItem( *aParent, aPort->GetText(), -1,
            new wxPODClientData<unsigned>( m_library.size() ) );
    wxIcon icon = render_power_as_icon( power_item.get() );
    m_dvtLib->SetItemIcon( dvitem, icon );
    m_library.push_back( power_item.release() );
}


void DIALOG_EDIT_POWER::OnPreviewRepaint( wxPaintEvent& aRepaintEvent )
{
    SCH_POWER temp_item( *m_poweritem );
    TransferDataFromWindow( &temp_item );
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
