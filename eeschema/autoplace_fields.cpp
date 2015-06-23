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
#include <eeschema_config.h>
#include <kiface_i.h>
#include <boost/foreach.hpp>
#include <vector>
#include <algorithm>

#define FIELD_V_SPACING 100
#define HPADDING 25
#define VPADDING 50

enum component_side {
    SIDE_TOP, SIDE_BOTTOM, SIDE_LEFT, SIDE_RIGHT
};


/**
 * Function round_n
 * Round up/down to the nearest multiple of n
 */
template<typename T> T round_n( const T& value, const T& n, bool aRoundUp )
{
    if( value % n )
        return n * (value / n + (aRoundUp ? 1 : 0));
    else
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
        if( !each_pin->IsVisible() )
            continue;
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
static void populate_preferred_sides( std::vector<struct side>& aSides,
                                      SCH_COMPONENT* aComponent )
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


/**
 * Function justify_field
 * Set the justification of a field based on the side it's supposed to be on, taking into
 * account whether the field will be displayed with flipped justification due to mirroring.
 */
static void justify_field( SCH_FIELD* aField, enum component_side aFieldSide )
{
    // Justification is set twice to allow IsHorizJustifyFlipped() to work correctly.
    switch( aFieldSide )
    {
    case SIDE_LEFT:
        aField->SetHorizJustify( GR_TEXT_HJUSTIFY_RIGHT );
        aField->SetHorizJustify( aField->IsHorizJustifyFlipped()
                ? GR_TEXT_HJUSTIFY_LEFT : GR_TEXT_HJUSTIFY_RIGHT );
        break;

    case SIDE_RIGHT:
        aField->SetHorizJustify( GR_TEXT_HJUSTIFY_LEFT );
        aField->SetHorizJustify( aField->IsHorizJustifyFlipped()
                ? GR_TEXT_HJUSTIFY_RIGHT : GR_TEXT_HJUSTIFY_LEFT );
        break;
    case SIDE_TOP:
    case SIDE_BOTTOM:
        aField->SetHorizJustify( GR_TEXT_HJUSTIFY_CENTER );
        break;
    }
}


/**
 * Function place_field_box
 * Return the position of the field bounding box for a component.
 */
static wxPoint place_field_box( SCH_COMPONENT* aComponent, enum component_side aFieldSide,
            wxSize aBoxSize )
{
    EDA_RECT body_box = aComponent->GetBodyBoundingBox();
    wxPoint fbox_pos;

    switch( aFieldSide )
    {
    case SIDE_RIGHT:
        fbox_pos.x = body_box.GetRight() + HPADDING;
        fbox_pos.y = body_box.GetCenter().y - aBoxSize.GetHeight()/2;
        break;
    case SIDE_BOTTOM:
        fbox_pos.x = body_box.GetLeft() + (body_box.GetWidth() - aBoxSize.GetWidth()) / 2;
        fbox_pos.y = body_box.GetBottom() + VPADDING;
        break;
    case SIDE_LEFT:
        fbox_pos.x = body_box.GetLeft() - aBoxSize.GetWidth() - HPADDING;
        fbox_pos.y = body_box.GetCenter().y - aBoxSize.GetHeight()/2;
        break;
    case SIDE_TOP:
        fbox_pos.x = body_box.GetLeft() + (body_box.GetWidth() - aBoxSize.GetWidth()) / 2;
        fbox_pos.y = body_box.GetTop() - aBoxSize.GetHeight() - VPADDING;
        break;
    default:
        wxFAIL_MSG( "Bad enum component_side value" );
        fbox_pos.x = body_box.GetRight();
        fbox_pos.y = body_box.GetCenter().y - aBoxSize.GetHeight()/2;
    }

    return fbox_pos;
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

        EDA_HOTKEY_CLIENT_DATA* data;
        data = dynamic_cast<EDA_HOTKEY_CLIENT_DATA*>( aEvent.GetClientObject() );
        wxCHECK_RET( data, wxT( "Invalid hot key client object." ) );
        item = LocateItem( data->GetPosition(),
                SCH_COLLECTOR::MovableItems, aEvent.GetInt() );
        screen->SetCurItem( NULL );
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
    OnModify();
}


/**
 * Function place_field_horiz
 * Place a field horizontally, taking into account the field width and
 * justification.
 *
 * @param aField - the field to place.
 * @param aFieldBox - box in which fields will be placed
 *
 * @return Correct field horizontal position
 */
static int place_field_horiz( SCH_FIELD *aField, const EDA_RECT &aFieldBox )
{
    EDA_TEXT_HJUSTIFY_T field_hjust = aField->GetHorizJustify();
    bool flipped = aField->IsHorizJustifyFlipped();
    if( flipped && field_hjust == GR_TEXT_HJUSTIFY_LEFT )
        field_hjust = GR_TEXT_HJUSTIFY_RIGHT;
    else if( flipped && field_hjust == GR_TEXT_HJUSTIFY_RIGHT )
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

    return field_xcoord;
}


void SCH_COMPONENT::AutoplaceFields()
{
    // Do not autoplace on power symbols
    if( PART_SPTR part = m_part.lock() )
        if( part->IsPower() ) return;

    // Gather information
    std::vector<SCH_FIELD*> fields;
    GetFields( fields, /* aVisibleOnly */ true );

    bool allow_rejustify = true;
    Kiface().KifaceSettings()->Read( AUTOPLACE_JUSTIFY_KEY, &allow_rejustify, true );

    int max_field_width = 0;
    for( size_t field_idx = 0; field_idx < fields.size(); ++field_idx )
    {
        if( GetTransform().y1 )
            fields[field_idx]->SetOrientation( TEXT_ORIENT_VERT );
        else
            fields[field_idx]->SetOrientation( TEXT_ORIENT_HORIZ );

        int field_width = fields[field_idx]->GetBoundingBox().GetWidth();
        if( field_width > max_field_width )
            max_field_width = field_width;
    }

    // Determine which side the fields will be placed on
    enum component_side field_side = choose_side_for_fields( this );

    // Compute the box into which fields will go
    wxSize fbox_size( max_field_width, FIELD_V_SPACING * (fields.size() - 1) );
    wxPoint fbox_pos = place_field_box( this, field_side, fbox_size );
    EDA_RECT field_box( fbox_pos, fbox_size );

    bool h_round_up, v_round_up;
    h_round_up = ( field_side != SIDE_LEFT );
    v_round_up = ( field_side == SIDE_BOTTOM );

    // Move the fields
    bool align_to_grid = false;
    Kiface().KifaceSettings()->Read( AUTOPLACE_ALIGN_KEY, &align_to_grid, true );

    for( size_t field_idx = 0; field_idx < fields.size(); ++field_idx )
    {
        wxPoint pos;
        SCH_FIELD* field = fields[field_idx];

        if( allow_rejustify )
            justify_field( field, field_side );

        pos.x = place_field_horiz( field, field_box );
        pos.y = field_box.GetY() + (FIELD_V_SPACING * field_idx);

        if( align_to_grid )
        {
            pos.x = round_n( pos.x, 50, h_round_up );
            pos.y = round_n( pos.y, 50, false );
        }

        field->SetPosition( pos );
    }

    m_fieldsAutoplaced = true;
}
