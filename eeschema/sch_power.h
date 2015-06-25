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
 * @file sch_power.h
 * @brief Definitions of the SCH_POWER class for Eeschema.
 */

#ifndef CLASS_POWER_H
#define CLASS_POWER_H

#include <sch_text.h>
#include <class_libentry.h>
#include <class_library.h>


typedef std::vector<SCH_FIELD>      SCH_FIELDS;

class SCH_POWER : public SCH_TEXT
{
    PART_REF    m_part;
    TRANSFORM   m_transform;
    int         m_unit;
    int         m_convert;
    wxString    m_part_name;
    wxString    m_visible_text;
    bool        m_label_hidden;

public:
    SCH_POWER( const wxPoint& pos = wxPoint( 0, 0 ), const wxString& text = wxEmptyString );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~SCH_POWER() { }

    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC, const wxPoint& offset,
               GR_DRAWMODE draw_mode, EDA_COLOR_T Color = UNSPECIFIED_COLOR );

    wxString GetClass() const
    {
        return wxT( "SCH_POWER" );
    }

    void SetOrientation( int aSchematicOrientation );

    void SwapData( SCH_ITEM* aItem );

    wxPoint GetSchematicTextOffset() const;

    bool Save( FILE* aFile ) const;

    bool Load( LINE_READER& aLine, wxString& aErrorMsg );

    const EDA_RECT GetBoundingBox() const;  // Virtual

    const EDA_RECT GetBodyBoundingBox() const;

    void CreateGraphicShape( std::vector <wxPoint>& aPoints, const wxPoint& aPos );

    void MirrorY( int aYaxis_position );

    void MirrorX( int aXaxis_position );

    void Rotate( wxPoint aPosition );

    bool IsConnectable() const { return true; }

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  add_glabel_xpm; }

    EDA_ITEM* Clone() const;

    TRANSFORM& GetTransform() const { return const_cast<TRANSFORM&>( m_transform ); }

    void SetTransform( const TRANSFORM& aTransform )
    {
        if( m_transform != aTransform )
        {
            m_transform = aTransform;
            SetModified();
        }
    }

    void SetPartName( const wxString& aName, PART_LIBS* aLibs = NULL );
    wxString GetPartName() const { return m_part_name; }

    bool Resolve( PART_LIBS* aLibs );

    static void ResolveAll( const SCH_COLLECTOR& aPowers, PART_LIBS* aLibs );

    virtual void SetText( const wxString& aText );

    /**
     * Provide an alternate text label to be displayed, in place of the net name.
     * Set to wxEmptyString to use the default name instead.
     */
    void SetVisibleText( const wxString& aText ) { m_visible_text = aText; }

    /**
     * Get the visible text. Returns wxEmptyString if the default name will be
     * used instead; in this case, use GetText() to see what will be displayed.
     */
    wxString GetVisibleText() const { return m_visible_text; }

    void SetLabelHidden( bool aHidden ) { m_label_hidden = aHidden; }
    bool GetLabelHidden() const { return m_label_hidden; }

    /**
     * Get the internal PART_LIB containing the hard-coded power symbol
     * styles.
     */
    static PART_LIB* GetPartLib();

private:
    bool doIsConnected( const wxPoint& aPosition ) const { return m_Pos == aPosition; }
};

#endif /* CLASS_POWER_H */
