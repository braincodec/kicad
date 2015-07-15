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

class wxDataViewItem;

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
 * Return the selected item's wxClientData.
 *
 * Returns NULL if there is no selection.
 */
static wxClientData* get_selected_data( wxBitmapComboBox* aBox )
{
    if( !aBox )
        return NULL;
    int sel = aBox->GetSelection();
    if( sel == wxNOT_FOUND )
        return NULL;
    return aBox->GetClientObject( sel );
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
    std::map<wxString, int> m_dvitems;

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
    void OnLibKey( wxKeyEvent& event );
    void OnCombo( wxCommandEvent& event );

    void OnStyleChange( wxCommandEvent& event );
    void OnLibChange( wxCommandEvent& event );

    /**
     * Add a template port to the library using heuristics
     */
    void AddLibPort( const wxString& aNet );

    /**
     * Clean up duplicate ports
     */
    void CleanUpPorts();

    /**
     * Add a template port from a SCH_POWER item. Takes ownership of the pointer.
     */
    void AddLibPort( SCH_POWER* aItem );

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
wxBitmap render_power_as_bmp( SCH_POWER* aItem, int aWidth=48, int aHeight=48 )
{
    wxBitmap bmp( aWidth, aHeight );

    wxMemoryDC dc( bmp );

    wxPoint offset = render_core( &dc, aItem->GetBoundingBox(), WHITE );

    aItem->Draw( NULL, &dc, offset, GR_COPY, UNSPECIFIED_COLOR );
    return bmp;
}


DIALOG_EDIT_POWER::DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem )
    : DIALOG_EDIT_POWER_BASE( aParent ),
      m_parent( aParent ),
      m_poweritem( aPowerItem ),
      m_dummy( wxPoint( 0, 0 ), wxEmptyString )
{
    m_stdButtonsOK->SetDefault();
    m_cboNet->SetFocus();

    wxTextValidator* validator = static_cast<wxTextValidator*>( m_cboNet->GetValidator() );
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

    m_cboStyle->Append( "Automatic", wxNullBitmap, new wxStringClientData( gsNameAutomatic ) );

    BOOST_FOREACH( wxString& each_name, lib_names )
    {
        int n = m_cboStyle->GetCount();
        wxBitmap bmp = SCH_POWER::RenderStylePreview( each_name, 32, 32 );
        m_cboStyle->Append( each_name, bmp, new wxStringClientData( each_name ) );
        m_dvitems[each_name] = n;
    }

    m_cboStyle->Select( 0 );
}


void DIALOG_EDIT_POWER::PopulateCommonPorts()
{
    static const struct libspec common_ports[] = {
        { _( "GND" ), wxEmptyString, _( "GND" ), false },
    };

    BOOST_FOREACH( const struct libspec& port, common_ports )
    {
        std::auto_ptr<SCH_POWER> item( new SCH_POWER( wxPoint( 0, 0 ), port.netname ) );
        item->SetVisibleText( port.visiblename );
        item->SetPartName( port.partname );
        item->SetLabelHidden( !port.namevisible );

        AddLibPort( item.release() );
    }
}


void DIALOG_EDIT_POWER::PopulateHiddenPins()
{
    std::vector<SCH_COMPONENT*> components;
    std::set<wxString> pin_name_set;
    get_type( components );

    BOOST_FOREACH( SCH_COMPONENT* each_comp, components )
    {
        std::vector<wxString> pin_names;
        each_comp->GetPowerPortNames( pin_names );
        BOOST_FOREACH( const wxString& each_pin_name, pin_names )
        {
            pin_name_set.insert( each_pin_name );
        }
    }

    BOOST_FOREACH( const wxString& each_pin_name, pin_name_set )
    {
        AddLibPort( each_pin_name );
    }
}


void DIALOG_EDIT_POWER::PopulateHistory()
{
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
    while( ports_filtered.size() )
    {
        AddLibPort( ports_filtered.release( ports_filtered.end() - 1 ).release() );
    }
}


void DIALOG_EDIT_POWER::PopulateLib()
{
    // Draw the styles
    PopulateHistory();
    PopulateCommonPorts();
    PopulateHiddenPins();
    CleanUpPorts();
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
        m_cboNet->SetValue( aItem->GetText() );

    m_textText->SetValue( aItem->GetVisibleText() );
    m_cbShowText->SetValue( !aItem->GetLabelHidden() );

    if( aUpdateNet && m_dvitems.count( aItem->GetPartName() ) )
    {
        m_cboStyle->Select( m_dvitems[aItem->GetPartName()] );
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

    aItem->SetText( m_cboNet->GetValue() );
    aItem->SetVisibleText( m_textText->GetValue() );
    aItem->SetLabelHidden( !m_cbShowText->GetValue() );

    wxString name = get_string_data( get_selected_data( m_cboStyle ) );
    if( name == gsNameAutomatic || name == wxEmptyString )
    {
        heur_choose_style( m_cboNet->GetValue(), &m_dummy );
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
    m_cboNet->SetValue( m_poweritem->GetText() );
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

    if( event.GetEventObject() == m_cboNet )
    {
        wxString name = get_string_data( get_selected_data( m_cboStyle ) );
        if( name == gsNameAutomatic || name == wxEmptyString )
        {
            heur_choose_style( m_cboNet->GetValue(), &m_dummy );
            TransferDataToWindow( &m_dummy, false );
        }
    }
    else if( event.GetEventObject() == m_textText ||
             event.GetEventObject() == m_cbShowText )
    {
        wxString name = get_string_data( get_selected_data( m_cboStyle ) );
        heur_choose_style( m_cboNet->GetValue(), &m_dummy );
        if( name == gsNameAutomatic &&
            ( m_dummy.GetVisibleText() != m_textText->GetValue() ||
              m_dummy.GetLabelHidden() != !m_cbShowText->GetValue() ) )
        {
            m_cboStyle->Select( m_dvitems[m_dummy.GetPartName()] );
        }
    }

    m_pPreview->Refresh();

    recursion_guard = false;
}


void DIALOG_EDIT_POWER::OnCombo( wxCommandEvent& event )
{
    if( event.GetEventObject() == m_cboStyle )
        OnStyleChange( event );
    else if( event.GetEventObject() == m_cboNet )
        OnLibChange( event );
}


void DIALOG_EDIT_POWER::OnLibKey( wxKeyEvent& event )
{
    if( event.GetEventObject() != m_cboNet )
        return;
    if( event.GetKeyCode() == WXK_DELETE )
    {
        // Delete the object from the history!
        wxClientData* data = get_selected_data( m_cboNet );
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


void DIALOG_EDIT_POWER::OnStyleChange( wxCommandEvent& event )
{
    // If a library part was selected, clear out the advanced settings from it
    if( m_cboNet->GetSelection() != wxNOT_FOUND )
    {
        m_textText->SetValue( wxEmptyString );
        m_cbShowText->SetValue( true );
    }
    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnLibChange( wxCommandEvent& event )
{
    wxClientData* data = get_selected_data( m_cboNet );
    if( !data )
        return;

    unsigned lib_item_index = get_pod<unsigned>( data );

    TransferDataToWindow( &m_library[lib_item_index] );

    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::AddLibPort( const wxString& aNet )
{
    std::auto_ptr<SCH_POWER> power_item( new SCH_POWER( wxPoint( 0, 0 ), aNet ) );
    heur_choose_style( aNet, power_item.get() );
    AddLibPort( power_item.release() );
}


void DIALOG_EDIT_POWER::AddLibPort( SCH_POWER* aPort )
{
    std::auto_ptr<SCH_POWER> power_item( aPort );
    power_item->Resolve( Prj().SchLibs() );

    wxBitmap bmp = render_power_as_bmp( power_item.get() );
    m_cboNet->Append( aPort->GetText(), bmp,
            new wxPODClientData<unsigned>( m_library.size() ) );
    m_library.push_back( power_item.release() );
}


void DIALOG_EDIT_POWER::CleanUpPorts()
{
    std::map<wxString, SCH_POWER*> items_seen;
    for( int i = 0; i < m_cboNet->GetCount(); ++i )
    {
        wxClientData* data = m_cboNet->GetClientObject( i );
        if( !data ) continue;
        wxString str = m_cboNet->GetString( i );
        unsigned lib_item_index = get_pod<unsigned>( data );

        std::map<wxString, SCH_POWER*>::iterator it = items_seen.find( str );
        if( it == items_seen.end() )
            items_seen[str] = &m_library[lib_item_index];
        else
        {
            SCH_POWER* other = it->second;
            if( *other == m_library[lib_item_index] )
            {
                m_cboNet->Delete( i );
                --i;
            }
        }
    }
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
