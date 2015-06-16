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
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>

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
 * Function choose_side_for_component
 * Look where a component's pins are to pick a side to put the fields on
 */
static enum component_side choose_side_for_fields( SCH_COMPONENT* aComponent )
{
    // Place fields on the first side with no pins, in this order:
    // RIGHT, TOP, LEFT, BOTTOM
    // If all four sides have pins, go with the side with fewer.
    // If all four sides are equal, go with TOP - it's probably a large IC.

    struct side sides[] = {
        { SIDE_RIGHT,   pins_on_side( aComponent, SIDE_RIGHT ) },
        { SIDE_TOP,     pins_on_side( aComponent, SIDE_TOP ) },
        { SIDE_LEFT,    pins_on_side( aComponent, SIDE_LEFT ) },
        { SIDE_BOTTOM,  pins_on_side( aComponent, SIDE_BOTTOM ) } };

    // If the component is mirrored, swap the preferred locations in that direction.
    int orient = aComponent->GetOrientation();
    int orient_angle = orient & 0xff; // enum is a bitmask

    if( ( orient & CMP_MIRROR_X ) && ( orient_angle == CMP_ORIENT_0 || orient_angle == CMP_ORIENT_180 ) )
    {
        struct side temp = sides[0];
        sides[0] = sides[2];
        sides[2] = temp;
    }

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

void SCH_COMPONENT::AutoplaceFields()
{
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

    int new_field_x, new_field_y;
    unsigned fields_height = 100 * (n_fields - 1);

    switch( field_side )
    {
    case SIDE_RIGHT:
        new_field_x = round_up_50( body_box.GetRight() + (max_field_width / 2) );
        new_field_y = round_down_50( body_box.GetY() + (body_box.GetHeight() / 2) - fields_height / 2 );
        break;
    case SIDE_BOTTOM:
        new_field_x = round_up_50( body_box.GetLeft() + (body_box.GetWidth() / 2) );
        new_field_y = round_up_50( body_box.GetBottom() + 25 );
        break;
    case SIDE_LEFT:
        new_field_x = round_down_50( body_box.GetLeft() - (max_field_width / 2) );
        new_field_y = round_down_50( body_box.GetY() + (body_box.GetHeight() / 2) - fields_height / 2 );
        break;
    case SIDE_TOP:
        new_field_x = round_up_50( body_box.GetLeft() + (body_box.GetWidth() / 2) );
        new_field_y = round_down_50( body_box.GetY() - fields_height - 25 );
        break;
    default:
        wxFAIL_MSG( "Bad enum component_side value" );
    }

    // Move the field
    for( size_t field_idx = 0; field_idx < GetFieldCount(); ++field_idx )
    {
        SCH_FIELD* field = GetField( field_idx );
        if( ! field->IsVisible() ) continue;
        if( field->GetText() == wxEmptyString ) continue;
        wxPoint pos = field->GetPosition();
        pos.x = new_field_x;
        pos.y = new_field_y;
        new_field_y += 100;
        field->SetPosition( pos );
    }

    m_fieldsAutoplaced = true;
}
