/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Chris Pavlina <pavlina.chris@gmail.com>
 * Copyright (C) 1992-2015 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file sch_power.cpp
 * @brief Code for handling power symbols.
 */


// Libraries
#include <boost/foreach.hpp>

// KiCad-wide utilities
#include <base_units.h>
#include <kicad_string.h>
#include <macros.h>
#include <msgpanel.h>
#include <plot_common.h>
#include <richio.h>
#include <trigo.h>

// Eeschema
#include <class_drawpanel.h>
#include <eeschema_id.h>
#include <general.h>
#include <protos.h>
#include <schframe.h>
#include <sch_power.h>

// Resource
#include <power_symbols_lib_data.h>


static PART_LIB* get_symbol_library()
{
    static std::string SymbolLibData( PowerSymbolLibData );
    static std::auto_ptr<PART_LIB> SymbolLib;
    if( !SymbolLib.get() )
    {
        std::auto_ptr<PART_LIB> lib( new PART_LIB( LIBRARY_TYPE_EESCHEMA, wxEmptyString ) );

        STRING_LINE_READER reader(SymbolLibData, "hard-coded library");
        wxString error_msg;

        bool r = lib->Load( reader, error_msg );
        wxASSERT_MSG( r, _( "Internal error reading hard-coded library: " ) + error_msg );

        SymbolLib = lib;
    }

    return SymbolLib.get();
}


SCH_POWER::SCH_POWER( const wxPoint& pos, const wxString& text ) :
    SCH_TEXT( pos, text, SCH_POWER_T ),
    m_part_name( text ),
    m_label_hidden( false )
{
    m_Layer = LAYER_POWER;
    m_isDangling = false;
}


EDA_ITEM* SCH_POWER::Clone() const
{
    return new SCH_POWER( *this );
}


bool SCH_POWER::Save( FILE* aFile ) const
{
    if( m_part_name.Find( _( " " ) ) != wxNOT_FOUND )
    {
        wxFAIL_MSG( "Power port's part name must not contain a space" );
        return false;
    }

    wxString visible_nospace = m_visible_text;
    visible_nospace.Replace( _( " " ), _( "~" ) );

    std::string visible_text_escaped = EscapedUTF8( m_visible_text, /* aNoSpace */ true );
    std::string part_name_escaped = EscapedUTF8( m_part_name, /* aNoSpace */ true );

#define check(x) do { if( x == EOF ) return false; } while( 0 )

    // "Text GPower" fools older versions of Eeschema into reading this as a
    // SCH_GLOBALLABEL, which is functionally equivalent if a bit ugly.
    check( fprintf( aFile, "Text GPower %-4d %-4d ", m_Pos.x, m_Pos.y ) );

    // Placeholders for some SCH_TEXT data, to ensure backward compatibility
    check( fprintf( aFile, "%-4d ", /* m_schematicOrientation */ 0 ) );
    check( fprintf( aFile, "%-4d ", /* m_Size.x */ 60 ) );
    check( fprintf( aFile, "%s ~ %d ", SheetLabelType[NET_BIDI], /* thickness */ 0 ) );

    // These are added data for SCH_POWER. Because older versions of Eeschema read just
    // as many fields as they need with sscanf and then directly skip to the next line,
    // they will be ignored.
    check( fprintf( aFile, "%d ", m_label_hidden ? 1 : 0 ) );
    check( fprintf( aFile, "%s %s ", visible_text_escaped.c_str(), part_name_escaped.c_str() ) );
    check( fprintf( aFile, "%d %d %d %d\n",
                m_transform.x1, m_transform.y1, m_transform.x2, m_transform.y2 ) );
    check( fprintf( aFile, "%s\n", TO_UTF8( m_Text ) ) );

#undef check

    return true;
}


bool SCH_POWER::Load( LINE_READER& aLine, wxString& aErrorMsg )
{
    char      sdummy[256];
    int       idummy;

    char      VisibleText[256];
    char      PartName[256];
    int       label_hidden;

    char*     sline = (char*) aLine;

    while( (*sline != ' ' ) && *sline )
        sline++;

    // sline points the start of parameters
    // Note the dummy variables - these are placeholders for SCH_TEXT parameters for
    // backward compatibility. We don't care about them.
    int ii = sscanf( sline, "%255s %d %d %d %d %255s %255s %d %d %255s %255s %d %d %d %d",
            sdummy, &m_Pos.x, &m_Pos.y, &idummy, &idummy, sdummy, sdummy, &idummy,
            &label_hidden, VisibleText, PartName,
            &m_transform.x1, &m_transform.y1, &m_transform.x2, &m_transform.y2 );

    char* text;

    if( ( ii < 15 )
        || !aLine.ReadLine()
        || !( text = strtok( (char*) aLine, "\n\r" ) ) )
    {
        aErrorMsg.Printf( wxT( "Eeschema file power port load error at line %d" ),
                          aLine.LineNumber() );
        return false;
    }

    m_Text = FROM_UTF8( text );

    m_visible_text = UnescapedUTF8( VisibleText );
    m_part_name = UnescapedUTF8( PartName );
    m_label_hidden = label_hidden;

    return true;
}


void SCH_POWER::MirrorY( int aYaxis_position )
{
    /* The global label is NOT really mirrored.
     *  for an horizontal label, the schematic orientation is changed.
     *  for a vertical label, the schematic orientation is not changed.
     *  and the label is moved to a suitable position
     */
    switch( GetOrientation() )
    {
    case 0:             /* horizontal text */
        SetOrientation( 2 );
        break;

    case 2:        /* invert horizontal text*/
        SetOrientation( 0 );
        break;
    }

    MIRROR( m_Pos.x, aYaxis_position );
}


void SCH_POWER::MirrorX( int aXaxis_position )
{
    switch( GetOrientation() )
    {
    case 1:             /* vertical text */
        SetOrientation( 3 );
        break;

    case 3:        /* invert vertical text*/
        SetOrientation( 1 );
        break;
    }

    MIRROR( m_Pos.y, aXaxis_position );
}


void SCH_POWER::Rotate( wxPoint aPosition )
{
    RotatePoint( &m_Pos, aPosition, 900 );
    SetOrientation( (GetOrientation() + 3) % 4 );
}


void SCH_POWER::SetOrientation( int aOrientation )
{
    m_transform.ReOrient( (enum ORIENTATION_T) aOrientation );
}


int SCH_POWER::GetOrientation() const
{
    return (int) m_transform.GetOrientation();
}


void SCH_POWER::SwapData( SCH_ITEM* aItem )
{
    SCH_POWER& other = dynamic_cast<SCH_POWER&>( *aItem );
    SCH_TEXT::SwapData( aItem );

    std::swap( m_part, other.m_part );
    std::swap( m_transform, other.m_transform );
    std::swap( m_part_name, other.m_part_name );
    std::swap( m_visible_text, other.m_visible_text );
    std::swap( m_label_hidden, other.m_label_hidden );
}


void SCH_POWER::Draw( EDA_DRAW_PANEL* aPanel,
                            wxDC*           aDC,
                            const wxPoint&  aOffset,
                            GR_DRAWMODE     aDrawMode,
                            EDA_COLOR_T     aColor )
{
    LIB_FIELDS fields;

    PART_SPTR part = m_part.lock();
    if( !part )
        part = PART_SPTR( LIB_PART::GetDummy() );

    part->GetFields( fields );
    part->Draw( aPanel, aDC, m_Pos + aOffset, /* unit */ 1, /* convert */ 1, aDrawMode,
            aColor, m_transform, true, false, false, NULL );

    BOOST_FOREACH( LIB_FIELD& field, fields )
    {
        if( field.GetId() != VALUE )
            continue;

        if( field.IsVisible() && !m_label_hidden )
        {
            wxString text;
            if( m_visible_text == wxEmptyString )
                text = GetText();
            else
                text = m_visible_text;

            field.drawGraphic( aPanel, aDC, m_Pos + aOffset, aColor, aDrawMode,
                    (void*) &text, m_transform );
        }
    }
}


void SCH_POWER::CreateGraphicShape( std::vector <wxPoint>& aPoints, const wxPoint& Pos )
{
}


const EDA_RECT SCH_POWER::GetBoundingBox() const
{
    EDA_RECT bbox = GetBodyBoundingBox();
    PART_SPTR part = m_part.lock();
    if( !part )
        part = PART_SPTR( LIB_PART::GetDummy() );

    LIB_FIELDS fields;
    part->GetFields( fields );
    BOOST_FOREACH( LIB_FIELD& field, fields )
    {
        if( field.GetId() != VALUE )
            continue;
        if( !field.IsVisible() || field.IsVoid() ||
                m_label_hidden || GetText() == wxEmptyString )
            continue;

        LIB_FIELD temp( field );
        temp.SetText( GetText() );
        EDA_RECT fbbox = temp.GetBoundingBox();
        fbbox.Move( m_Pos );
        bbox.Merge( fbbox );
    }
    return bbox;
}


const EDA_RECT SCH_POWER::GetBodyBoundingBox() const
{
    EDA_RECT    bBox;

    if( PART_SPTR part = m_part.lock() )
    {
        bBox = part->GetBodyBoundingBox( /* unit */ 1, /* convert */ 1 );
    }
    else
    {
        bBox = LIB_PART::GetDummy()->GetBodyBoundingBox( /* unit */ 1, /* convert */ 1 );
    }

    int x0 = bBox.GetX();
    int xm = bBox.GetRight();

    // We must reverse Y values, because matrix orientation
    // suppose Y axis normal for the library items coordinates,
    // m_transform reverse Y values, but bBox is already reversed!
    int y0 = -bBox.GetY();
    int ym = -bBox.GetBottom();

    // Compute the real Boundary box (rotated, mirrored ...)
    int x1 = m_transform.x1 * x0 + m_transform.y1 * y0;
    int y1 = m_transform.x2 * x0 + m_transform.y2 * y0;
    int x2 = m_transform.x1 * xm + m_transform.y1 * ym;
    int y2 = m_transform.x2 * xm + m_transform.y2 * ym;

    // H and W must be > 0:
    if( x2 < x1 )
        std::swap( x2, x1 );

    if( y2 < y1 )
        std::swap( y2, y1 );

    bBox.SetX( x1 );
    bBox.SetY( y1 );
    bBox.SetWidth( x2 - x1 );
    bBox.SetHeight( y2 - y1 );

    bBox.Offset( m_Pos );
    return bBox;
}


wxString SCH_POWER::GetSelectMenuText() const
{
    wxString msg;
    msg.Printf( _( "Power Port [%s]" ), GetChars( ShortenedShownText() ) );
    return msg;
}


void SCH_POWER::SetPartName( const wxString& aName, PART_LIBS* aLibs )
{
    if( m_part_name != aName )
    {
        m_part_name = aName;
        SetModified();

        if( aLibs )
            Resolve( aLibs );
        else
            m_part.reset();
    }
}


bool SCH_POWER::Resolve( PART_LIBS* aLibs )
{
    if( LIB_PART* part = get_symbol_library()->FindPart( m_part_name ) )
    {
        m_part = part->SharedPtr();
    }
    else
    {
        m_part = LIB_PART::GetDummy()->SharedPtr();
    }

    return true;
}


void SCH_POWER::ResolveAll( const SCH_COLLECTOR& aPowers, PART_LIBS* aLibs )
{
    for( int i = 0; i < aPowers.GetCount(); ++i )
    {
        SCH_POWER* cmp = dynamic_cast<SCH_POWER*>( aPowers[i] );
        wxASSERT( cmp );
        if( cmp )
            cmp->Resolve( aLibs );
    }
}


void SCH_POWER::SetText( const wxString& aText )
{
    EDA_TEXT::SetText( aText );
}


PART_LIB* SCH_POWER::GetPartLib()
{
    return get_symbol_library();
}


bool SCH_POWER::operator==( const SCH_POWER& aOther ) const
{
    return
        ( m_transform == aOther.m_transform ) &&
        ( m_part_name == aOther.m_part_name ) &&
        ( m_visible_text == aOther.m_visible_text ) &&
        ( m_label_hidden == aOther.m_label_hidden ) &&
        ( GetText() == aOther.GetText() ) &&
        true;
}
