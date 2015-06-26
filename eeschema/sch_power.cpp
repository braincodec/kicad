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

#include <fctsys.h>
#include <gr_basic.h>
#include <macros.h>
#include <trigo.h>
#include <eeschema_id.h>
#include <class_drawpanel.h>
#include <drawtxt.h>
#include <schframe.h>
#include <plot_common.h>
#include <base_units.h>
#include <msgpanel.h>

#include <general.h>
#include <protos.h>
#include <sch_power.h>
#include <class_netlist_object.h>

#include <lib_rectangle.h>
#include <lib_text.h>
#include <boost/foreach.hpp>

#include <richio.h>
#include <kicad_string.h>

#include <power_symbols_lib_data.h>

/**
 * Used when a LIB_PART is not found in library
 * to draw a dummy shape
 * This component is a 400 mils square with the text ??
 * DEF DUMMY U 0 40 Y Y 1 0 N
 * F0 "U" 0 -350 60 H V
 * F1 "DUMMY" 0 350 60 H V
 * DRAW
 * T 0 0 0 150 0 0 0 ??
 * S -200 200 200 -200 0 1 0
 * ENDDRAW
 * ENDDEF
 */
static std::auto_ptr<LIB_PART> sgDummy;
static LIB_PART* dummy()
{
    if( !sgDummy.get() )
    {
        std::auto_ptr<LIB_PART> partaptr( new LIB_PART( wxEmptyString ) );

        LIB_PART* part = partaptr.get();

        LIB_RECTANGLE* square = new LIB_RECTANGLE( part );

        square->Move( wxPoint( -200, 200 ) );
        square->SetEndPosition( wxPoint( 200, -200 ) );

        LIB_TEXT* text = new LIB_TEXT( part );

        text->SetSize( wxSize( 150, 150 ) );
        text->SetText( wxString( wxT( "??" ) ) );

        part->AddDrawItem( square );
        part->AddDrawItem( text );

        sgDummy = partaptr;
    }

    return sgDummy.get();
}

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
    SCH_TEXT( pos, text, SCH_POWER_T )
{
    m_unit = m_convert = 1;
    m_Layer = LAYER_POWER;
    m_shape = NET_BIDI;
    m_isDangling = true;
    m_MultilineAllowed = false;
    m_transform = TRANSFORM();
    SetPartName( text );
    m_label_hidden = false;
    m_part = dummy()->SharedPtr();
}


EDA_ITEM* SCH_POWER::Clone() const
{
    return new SCH_POWER( *this );
}


bool SCH_POWER::Save( FILE* aFile ) const
{
    bool        success = true;
    const char* shape   = "~";

    if( m_Italic )
        shape = "Italic";

    wxString visible_nospace = m_visible_text;
    visible_nospace.Replace( _( " " ), _( "~" ) );

    if( m_part_name.Find( _( " " ) ) != wxNOT_FOUND )
    {
        wxFAIL_MSG( "Power port's part name must not contain a space" );
        return false;
    }

    std::string visible_text_escaped = EscapedUTF8( m_visible_text );
    std::string part_name_escaped = EscapedUTF8( m_part_name );

    if( fprintf( aFile, "Text GPLabel %-4d %-4d %-4d %-4d %s %s %d %d %s %s\n%s\n",
                 m_Pos.x, m_Pos.y, m_schematicOrientation, m_Size.x,
                 SheetLabelType[m_shape], shape, m_Thickness,
                 m_label_hidden ? 1 : 0, visible_text_escaped.c_str(), part_name_escaped.c_str(),
                 TO_UTF8( m_Text ) ) == EOF )
    {
        success = false;
    }

    return success;
}


bool SCH_POWER::Load( LINE_READER& aLine, wxString& aErrorMsg )
{
    char      Name1[256];
    char      Name2[256];
    char      Name3[256];
    char      VisibleText[256];
    char      PartName[256];
    int       thickness = 0, size = 0, orient = 0, label_hidden = 0;

    Name1[0] = 0; Name2[0] = 0; Name3[0] = 0;

    char*     sline = (char*) aLine;

    while( (*sline != ' ' ) && *sline )
        sline++;

    // sline points the start of parameters
    int ii = sscanf( sline, "%255s %d %d %d %d %255s %255s %d %d %255s %255s", Name1, &m_Pos.x,
            &m_Pos.y, &orient, &size, Name2, Name3, &thickness, &label_hidden, VisibleText,
            PartName);

    if( ii < 11 )
    {
        aErrorMsg.Printf( wxT( "Eeschema file power port load error at line %d" ),
                          aLine.LineNumber() );
        return false;
    }

    if( !aLine.ReadLine() )
    {
        aErrorMsg.Printf( wxT( "Eeschema file power port load  error at line %d" ),
                          aLine.LineNumber() );
        return false;
    }

    if( size == 0 )
        size = GetDefaultTextSize();

    char* text = strtok( (char*) aLine, "\n\r" );

    if( text == NULL )
    {
        aErrorMsg.Printf( wxT( "Eeschema file power port load error at line %d" ),
                          aLine.LineNumber() );
        return false;
    }

    m_Text = FROM_UTF8( text );
    m_Size.x = m_Size.y = size;
    SetOrientation( orient );
    m_shape  = NET_INPUT;
    m_Bold = ( thickness != 0 );
    m_Thickness = m_Bold ? GetPenSizeForBold( size ) : 0;

    if( stricmp( Name2, SheetLabelType[NET_OUTPUT] ) == 0 )
        m_shape = NET_OUTPUT;

    if( stricmp( Name2, SheetLabelType[NET_BIDI] ) == 0 )
        m_shape = NET_BIDI;

    if( stricmp( Name2, SheetLabelType[NET_TRISTATE] ) == 0 )
        m_shape = NET_TRISTATE;

    if( stricmp( Name2, SheetLabelType[NET_UNSPECIFIED] ) == 0 )
        m_shape = NET_UNSPECIFIED;

    if( stricmp( Name3, "Italic" ) == 0 )
        m_Italic = 1;

    ReadDelimitedText( &m_visible_text, VisibleText );
    m_visible_text.Replace( _( "~" ), _( " " ) );
    ReadDelimitedText( &m_part_name, PartName );
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

    m_Pos.x -= aYaxis_position;
    NEGATE( m_Pos.x );
    m_Pos.x += aYaxis_position;
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

    m_Pos.y -= aXaxis_position;
    NEGATE( m_Pos.y );
    m_Pos.y += aXaxis_position;
}


void SCH_POWER::Rotate( wxPoint aPosition )
{
    RotatePoint( &m_Pos, aPosition, 900 );
    SetOrientation( (GetOrientation() + 3) % 4 );
}


wxPoint SCH_POWER::GetSchematicTextOffset() const
{
    wxPoint text_offset;
    int     width = (m_Thickness == 0) ? GetDefaultLineThickness() : m_Thickness;

    width = Clamp_Text_PenSize( width, m_Size, m_Bold );
    int     HalfSize = m_Size.x / 2;
    int     offset   = width;

    switch( m_shape )
    {
    case NET_INPUT:
    case NET_BIDI:
    case NET_TRISTATE:
        offset += HalfSize;
        break;

    case NET_OUTPUT:
    case NET_UNSPECIFIED:
        offset += TXTMARGE;
        break;

    default:
        break;
    }

    switch( m_schematicOrientation )
    {
    case 0:             /* Orientation horiz normal */
        text_offset.x -= offset;
        break;

    case 1:             /* Orientation vert UP */
        text_offset.y -= offset;
        break;

    case 2:             /* Orientation horiz inverse */
        text_offset.x += offset;
        break;

    case 3:             /* Orientation vert BOTTOM */
        text_offset.y += offset;
        break;
    }

    return text_offset;
}


void SCH_POWER::SetOrientation( int aOrientation )
{
    TRANSFORM temp = TRANSFORM();
    bool transform = false;

    switch( aOrientation )
    {
    case CMP_ORIENT_0:
    case CMP_NORMAL:                    // default transform matrix
        m_transform.x1 = 1;
        m_transform.y2 = -1;
        m_transform.x2 = m_transform.y1 = 0;
        break;

    case CMP_ROTATE_COUNTERCLOCKWISE:  // Rotate + (incremental rotation)
        temp.x1   = temp.y2 = 0;
        temp.y1   = 1;
        temp.x2   = -1;
        transform = true;
        break;

    case CMP_ROTATE_CLOCKWISE:          // Rotate - (incremental rotation)
        temp.x1   = temp.y2 = 0;
        temp.y1   = -1;
        temp.x2   = 1;
        transform = true;
        break;

    case CMP_MIRROR_Y:                  // Mirror Y (incremental rotation)
        temp.x1   = -1;
        temp.y2   = 1;
        temp.y1   = temp.x2 = 0;
        transform = true;
        break;

    case CMP_MIRROR_X:                  // Mirror X (incremental rotation)
        temp.x1   = 1;
        temp.y2   = -1;
        temp.y1   = temp.x2 = 0;
        transform = true;
        break;

    case CMP_ORIENT_90:
        SetOrientation( CMP_ORIENT_0 );
        SetOrientation( CMP_ROTATE_COUNTERCLOCKWISE );
        break;

    case CMP_ORIENT_180:
        SetOrientation( CMP_ORIENT_0 );
        SetOrientation( CMP_ROTATE_COUNTERCLOCKWISE );
        SetOrientation( CMP_ROTATE_COUNTERCLOCKWISE );
        break;

    case CMP_ORIENT_270:
        SetOrientation( CMP_ORIENT_0 );
        SetOrientation( CMP_ROTATE_CLOCKWISE );
        break;

    case ( CMP_ORIENT_0 + CMP_MIRROR_X ):
        SetOrientation( CMP_ORIENT_0 );
        SetOrientation( CMP_MIRROR_X );
        break;

    case ( CMP_ORIENT_0 + CMP_MIRROR_Y ):
        SetOrientation( CMP_ORIENT_0 );
        SetOrientation( CMP_MIRROR_Y );
        break;

    case ( CMP_ORIENT_90 + CMP_MIRROR_X ):
        SetOrientation( CMP_ORIENT_90 );
        SetOrientation( CMP_MIRROR_X );
        break;

    case ( CMP_ORIENT_90 + CMP_MIRROR_Y ):
        SetOrientation( CMP_ORIENT_90 );
        SetOrientation( CMP_MIRROR_Y );
        break;

    case ( CMP_ORIENT_180 + CMP_MIRROR_X ):
        SetOrientation( CMP_ORIENT_180 );
        SetOrientation( CMP_MIRROR_X );
        break;

    case ( CMP_ORIENT_180 + CMP_MIRROR_Y ):
        SetOrientation( CMP_ORIENT_180 );
        SetOrientation( CMP_MIRROR_Y );
        break;

    case ( CMP_ORIENT_270 + CMP_MIRROR_X ):
        SetOrientation( CMP_ORIENT_270 );
        SetOrientation( CMP_MIRROR_X );
        break;

    case ( CMP_ORIENT_270 + CMP_MIRROR_Y ):
        SetOrientation( CMP_ORIENT_270 );
        SetOrientation( CMP_MIRROR_Y );
        break;

    default:
        transform = false;
        wxMessageBox( wxT( "SetOrientation() error: invalid orientation" ) );
        break;
    }

    if( transform )
    {
        /* The new matrix transform is the old matrix transform modified by the
         *  requested transformation, which is the temp transform (rot,
         *  mirror ..) in order to have (in term of matrix transform):
         *     transform coord = new_m_transform * coord
         *  where transform coord is the coord modified by new_m_transform from
         *  the initial value coord.
         *  new_m_transform is computed (from old_m_transform and temp) to
         *  have:
         *     transform coord = old_m_transform * temp
         */
        TRANSFORM newTransform;

        newTransform.x1 = m_transform.x1 * temp.x1 + m_transform.x2 * temp.y1;
        newTransform.y1 = m_transform.y1 * temp.x1 + m_transform.y2 * temp.y1;
        newTransform.x2 = m_transform.x1 * temp.x2 + m_transform.x2 * temp.y2;
        newTransform.y2 = m_transform.y1 * temp.x2 + m_transform.y2 * temp.y2;
        m_transform = newTransform;
    }
}


void SCH_POWER::SwapData( SCH_ITEM* aItem )
{
    SCH_POWER& other = dynamic_cast<SCH_POWER&>( *aItem );
    SCH_TEXT::SwapData( aItem );

    std::swap( m_part, other.m_part );
    std::swap( m_transform, other.m_transform );
    std::swap( m_unit, other.m_unit );
    std::swap( m_convert, other.m_convert );
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

    if( PART_SPTR part = m_part.lock() )
    {
        part->GetFields( fields );
        part->Draw( aPanel, aDC, m_Pos + aOffset, m_unit, m_convert, aDrawMode, aColor,
                m_transform, true, false, false, NULL );
    }

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
    if( PART_SPTR part = m_part.lock() )
    {
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
    }
    return bbox;
}


const EDA_RECT SCH_POWER::GetBodyBoundingBox() const
{
    EDA_RECT    bBox;

    if( PART_SPTR part = m_part.lock() )
    {
        bBox = part->GetBodyBoundingBox( m_unit, m_convert );
    }
    else
    {
        bBox = dummy()->GetBodyBoundingBox( m_unit, m_convert );
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
        EXCHG( x2, x1 );

    if( y2 < y1 )
        EXCHG( y2, y1 );

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
        m_part = dummy()->SharedPtr();
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
        ( m_unit == aOther.m_unit ) &&
        ( m_convert == aOther.m_convert ) &&
        ( m_part_name == aOther.m_part_name ) &&
        ( m_visible_text == aOther.m_visible_text ) &&
        ( m_label_hidden == aOther.m_label_hidden ) &&
        true;
}
