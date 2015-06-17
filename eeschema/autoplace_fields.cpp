/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Chris Pavlina <pavlina.chris@gmail.com>
 * Copyright (C) 2015 KiCad Developers, see change_log.txt for contributors.
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

#include <schframe.h>
#include <hotkeys_basic.h>
#include <sch_component.h>
#include <lib_pin.h>
#include <class_drawpanel.h>
#include <class_libentry.h>
#include <boost/foreach.hpp>
#include <vector>
#include <algorithm>

#define FIELD_V_SPACING 100

enum component_side {
    SIDE_TOP, SIDE_BOTTOM, SIDE_LEFT, SIDE_RIGHT
};


/**
 * Function round_up_50
 * Round up to the nearest 50
 */
template<typename T> T round_up_50( const T& value )
{
    if( value % 50 )
        return 50 * (value / 50 + 1);
    return value;
}


/**
 * Function round_down_50
 * Round down to the nearest 50
 */
template<typename T> T round_down_50( const T& value )
{
    if( value % 50 )
        return 50 * (value / 50);
    return value;
}


/**
 * Function get_pin_side
 * Return the side that a pin is on.
 */
static enum component_side get_pin_side( SCH_COMPONENT* aComponent, LIB_PIN* aPin )
{
    int pin_orient = aPin->PinDrawOrient( aComponent->GetTransform() );
    switch( pin_orient )
    {
    case PIN_RIGHT:
        return SIDE_LEFT;
    case PIN_LEFT:
        return SIDE_RIGHT;
    case PIN_UP:
        return SIDE_BOTTOM;
    case PIN_DOWN:
        return SIDE_TOP;
    default:
        wxFAIL_MSG( "Invalid pin orientation" );
        return SIDE_LEFT;
    }
}


/**
 * Function pins_on_side
 * Count the number of pins on a side of the component
 */
static unsigned pins_on_side( SCH_COMPONENT* aComponent, enum component_side aSide )
{
    unsigned pin_count = 0;

    std::vector<LIB_PIN*> pins;
    aComponent->GetPins( pins );

    BOOST_FOREACH( LIB_PIN* each_pin, pins )
    {
        if( get_pin_side( aComponent, each_pin ) == aSide )
            ++pin_count;
    }

    return pin_count;
}

// Used for iteration
struct side {
    enum component_side side_name;
    unsigned side_pins;
};


/**
 * Function populate_preferred_sides
 * Populate a list with the preferred field sides for the component, in
 * decreasing order of preference.
 */
static void populate_preferred_sides( std::vector<struct side>& aSides, SCH_COMPONENT* aComponent )
{
    struct side sides[] = {
        { SIDE_RIGHT,   pins_on_side( aComponent, SIDE_RIGHT ) },
        { SIDE_TOP,     pins_on_side( aComponent, SIDE_TOP ) },
        { SIDE_LEFT,    pins_on_side( aComponent, SIDE_LEFT ) },
        { SIDE_BOTTOM,  pins_on_side( aComponent, SIDE_BOTTOM ) },
    };

    int orient = aComponent->GetOrientation();
    int orient_angle = orient & 0xff; // enum is a bitmask

    // If the component is horizontally mirrored, swap left and right
    if( ( orient & CMP_MIRROR_X ) && ( orient_angle == CMP_ORIENT_0 || orient_angle == CMP_ORIENT_180 ) )
    {
        std::swap( sides[0], sides[2] );
    }

    // If the component is very long, swap H and V
    EDA_RECT body_box = aComponent->GetBodyBoundingBox();
    if( double( body_box.GetWidth() ) / double( body_box.GetHeight() ) > 3.0 )
    {
        std::swap( sides[0], sides[1] );
        std::swap( sides[1], sides[3] );
    }

    BOOST_FOREACH( struct side& each_side, sides )
    {
        aSides.push_back( each_side );
    }
}


/**
 * Function choose_side_for_component
 * Look where a component's pins are to pick a side to put the fields on
 */
static enum component_side choose_side_for_fields( SCH_COMPONENT* aComponent )
{
    std::vector<struct side> sides;
    populate_preferred_sides( sides, aComponent );


    BOOST_FOREACH( struct side& each_side, sides )
    {
        if( !each_side.side_pins ) return each_side.side_name;
    }

    unsigned min_pins = (unsigned)(-1);
    enum component_side min_side = SIDE_RIGHT;

    BOOST_REVERSE_FOREACH( struct side& each_side, sides )
    {
        if( each_side.side_pins < min_pins )
        {
            min_pins = each_side.side_pins;
            min_side = each_side.side_name;
        }
    }

    return min_side;
}

void SCH_EDIT_FRAME::OnAutoplaceFields( wxCommandEvent& aEvent )
{
    SCH_SCREEN* screen = GetScreen();
    SCH_ITEM* item = screen->GetCurItem();

    // Get the item under cursor if we're not currently moving something
    if( !item )
    {
        if( aEvent.GetInt() == 0 )
            return;

        EDA_HOTKEY_CLIENT_DATA* data = dynamic_cast<EDA_HOTKEY_CLIENT_DATA*>( aEvent.GetClientObject() );
        wxCHECK_RET( data, wxT( "Invalid hot key client object." ) );
        item = LocateAndShowItem( data->GetPosition(), SCH_COLLECTOR::MovableItems, aEvent.GetInt() );
        if( !item || item->GetFlags() )
            return;
    }

    if( item->Type() != SCH_COMPONENT_T )
        return;

    if( !item->IsNew() )
        SaveCopyInUndoList( item, UR_CHANGED );

    SCH_COMPONENT *component = dynamic_cast<SCH_COMPONENT*>( item );

    component->AutoplaceFields();

    GetCanvas()->Refresh();
}


/**
 * Function place_field_horiz
 * Place a field horizontally, taking into account the field width and
 * justification.
 *
 * @param aField - the field to place.
 * @param aFieldBox - box in which fields will be placed
 * @param aMirror - true if the component will be displayed mirrored or rotated,
 *  such that the justification inverts
 *
 * @return Correct field horizontal position
 */
static int place_field_horiz( SCH_FIELD *aField, const EDA_RECT &aFieldBox, bool aMirror )
{
    EDA_TEXT_HJUSTIFY_T field_hjust = aField->GetHorizJustify();
    if( aMirror && field_hjust == GR_TEXT_HJUSTIFY_LEFT )
        field_hjust = GR_TEXT_HJUSTIFY_RIGHT;
    else if( aMirror && field_hjust == GR_TEXT_HJUSTIFY_RIGHT )
        field_hjust = GR_TEXT_HJUSTIFY_LEFT;

    int field_xcoord;

    switch( field_hjust )
    {
    case GR_TEXT_HJUSTIFY_LEFT:
        field_xcoord = aFieldBox.GetLeft();
        break;
    case GR_TEXT_HJUSTIFY_CENTER:
        field_xcoord = aFieldBox.GetCenter().x;
        break;
    case GR_TEXT_HJUSTIFY_RIGHT:
        field_xcoord = aFieldBox.GetRight();
        break;
    default:
        wxFAIL_MSG( "Unexpected value for SCH_FIELD::GetHorizJustify()" );
        field_xcoord = aFieldBox.GetCenter().x; // Most are centered
    }

    return round_up_50( field_xcoord );
}


/**
 * Function field_is_mirrored
 * Determine whether a field will be displayed with mirrored horizontal justification.
 */
static bool field_is_mirrored( SCH_FIELD *aField )
{
    EDA_RECT bbox = aField->GetBoundingBox();
    wxPoint render_center = bbox.Centre();
    wxPoint pos = aField->GetPosition();

    switch( aField->GetHorizJustify() )
    {
    case GR_TEXT_HJUSTIFY_LEFT:
        return render_center.x < pos.x;
    case GR_TEXT_HJUSTIFY_RIGHT:
        return render_center.x > pos.x;
    default:
        return false;
    }
}


void SCH_COMPONENT::AutoplaceFields()
{
    // Do not autoplace on power symbols
    if( PART_SPTR part = m_part.lock() )
        if( part->IsPower() ) return;

    EDA_RECT body_box = GetBodyBoundingBox();

    enum component_side field_side = choose_side_for_fields( this );

    int max_field_width = 0;
    unsigned n_fields = 0;
    for( size_t field_idx = 0; field_idx < GetFieldCount(); ++field_idx )
    {
        SCH_FIELD* field = GetField( field_idx );
        if( ! field->IsVisible() ) continue;
        if( field->GetText() == wxEmptyString ) continue;
        ++n_fields;

        if( GetTransform().y1 )
            field->SetOrientation( TEXT_ORIENT_VERT );
        else
            field->SetOrientation( TEXT_ORIENT_HORIZ );

        int field_width = field->GetBoundingBox().GetWidth();
        if( field_width > max_field_width )
            max_field_width = field_width;
    }

    wxSize fbox_size( max_field_width, FIELD_V_SPACING * (n_fields - 1) );
    wxPoint fbox_pos;

    switch( field_side )
    {
    case SIDE_RIGHT:
        fbox_pos.x = body_box.GetRight();
        fbox_pos.y = round_down_50( body_box.GetCenter().y - fbox_size.GetHeight()/2 );
        break;
    case SIDE_BOTTOM:
        fbox_pos.x = body_box.GetLeft() + (body_box.GetWidth() - fbox_size.GetWidth()) / 2;
        fbox_pos.y = round_up_50( body_box.GetBottom() + 25 );
        break;
    case SIDE_LEFT:
        fbox_pos.x = body_box.GetLeft() - fbox_size.GetWidth();
        fbox_pos.y = round_down_50( body_box.GetCenter().y - fbox_size.GetHeight()/2 );
        break;
    case SIDE_TOP:
        fbox_pos.x = body_box.GetLeft() + (body_box.GetWidth() - fbox_size.GetWidth()) / 2;
        fbox_pos.y = round_down_50( body_box.GetTop() - fbox_size.GetHeight() - 25 );
        break;
    default:
        wxFAIL_MSG( "Bad enum component_side value" );
    }

    EDA_RECT field_box( fbox_pos, fbox_size );
    int new_field_y = field_box.GetY();

    // Move the field
    for( size_t field_idx = 0; field_idx < GetFieldCount(); ++field_idx )
    {
        SCH_FIELD* field = GetField( field_idx );
        if( ! field->IsVisible() ) continue;
        if( field->GetText() == wxEmptyString ) continue;
        wxPoint pos = field->GetPosition();
        pos.x = place_field_horiz( field, field_box, field_is_mirrored( field ) );
        pos.y = new_field_y;
        new_field_y += FIELD_V_SPACING;
        field->SetPosition( pos );
    }

    m_fieldsAutoplaced = true;
}
