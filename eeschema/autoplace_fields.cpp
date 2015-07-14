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


class AUTOPLACER
{
    SCH_SCREEN* m_screen;
    SCH_COMPONENT* m_component;
    std::vector<SCH_FIELD*> m_fields;
    EDA_RECT m_comp_bbox;
    wxSize m_fbox_size;
    bool m_allow_rejustify, m_align_to_grid;

public:
    enum SIDE
    {
        SIDE_TOP, SIDE_BOTTOM, SIDE_LEFT, SIDE_RIGHT
    };


    struct SIDE_AND_NPINS
    {
        SIDE name;
        unsigned pins;
    };


    AUTOPLACER( SCH_COMPONENT* aComponent, SCH_SCREEN* aScreen )
        :m_screen( aScreen ), m_component( aComponent )
    {
        m_component->GetFields( m_fields, /* aVisibleOnly */ true );
        Kiface().KifaceSettings()->Read( AUTOPLACE_JUSTIFY_KEY, &m_allow_rejustify, true );
        Kiface().KifaceSettings()->Read( AUTOPLACE_ALIGN_KEY, &m_align_to_grid, false );

        m_comp_bbox = m_component->GetBodyBoundingBox();
        m_fbox_size = ComputeFBoxSize();
    }


    /**
     * Do the actual autoplacement.
     * @param aManual - if true, use extra heuristics for smarter placement when manually
     * called up.
     */
    void DoAutoplace( bool aManual )
    {
        // Do not autoplace on power symbols
        if( ! m_component->IsInNetlist() )
            return;

        SIDE field_side = choose_side_for_fields( aManual );
        wxPoint fbox_pos = field_box_placement( field_side );
        EDA_RECT field_box( fbox_pos, m_fbox_size );

        bool h_round_up, v_round_up;
        h_round_up = ( field_side != SIDE_LEFT );
        v_round_up = ( field_side == SIDE_BOTTOM );

        // Move the fields
        for( size_t field_idx = 0; field_idx < m_fields.size(); ++field_idx )
        {
            SCH_FIELD* field = m_fields[field_idx];

            if( m_allow_rejustify )
                justify_field( field, field_side );

            wxPoint pos;
            pos.x = field_horiz_placement( field, field_box );
            pos.y = field_box.GetY() + (FIELD_V_SPACING * field_idx);

            if( m_align_to_grid )
            {
                pos.x = round_n( pos.x, 50, h_round_up );
                pos.y = round_n( pos.y, 50, v_round_up );
            }

            field->SetPosition( pos );
        }
    }


protected:
    /**
     * Compute and return the size of the fields' bounding box.
     */
    wxSize ComputeFBoxSize()
    {
        int max_field_width = 0;
        for( size_t field_idx = 0; field_idx < m_fields.size(); ++field_idx )
        {
            if( m_component->GetTransform().y1 )
                m_fields[field_idx]->SetOrientation( TEXT_ORIENT_VERT );
            else
                m_fields[field_idx]->SetOrientation( TEXT_ORIENT_HORIZ );

            int field_width = m_fields[field_idx]->GetBoundingBox().GetWidth();
            max_field_width = std::max( max_field_width, field_width );
        }

        return wxSize( max_field_width, FIELD_V_SPACING * (m_fields.size() - 1) );
    }


    /**
     * Function get_pin_side
     * Return the side that a pin is on.
     */
    SIDE get_pin_side( LIB_PIN* aPin )
    {
        int pin_orient = aPin->PinDrawOrient( m_component->GetTransform() );
        switch( pin_orient )
        {
            case PIN_RIGHT: return SIDE_LEFT;
            case PIN_LEFT:  return SIDE_RIGHT;
            case PIN_UP:    return SIDE_BOTTOM;
            case PIN_DOWN:  return SIDE_TOP;
            default:
                wxFAIL_MSG( "Invalid pin orientation" );
                return SIDE_LEFT;
        }
    }


    /**
     * Function pins_on_side
     * Count the number of pins on a side of the component.
     */
    unsigned pins_on_side( SIDE aSide )
    {
        unsigned pin_count = 0;

        std::vector<LIB_PIN*> pins;
        m_component->GetPins( pins );

        BOOST_FOREACH( LIB_PIN* each_pin, pins )
        {
            if( !each_pin->IsVisible() )
                continue;
            if( get_pin_side( each_pin ) == aSide )
                ++pin_count;
        }

        return pin_count;
    }


    /**
     * Function get_preferred_sides
     * Return a list with the preferred field sides for the component, in
     * decreasing order of preference.
     */
    std::vector<SIDE_AND_NPINS> get_preferred_sides()
    {
        SIDE_AND_NPINS sides_init[] = {
            { SIDE_RIGHT,   pins_on_side( SIDE_RIGHT ) },
            { SIDE_TOP,     pins_on_side( SIDE_TOP ) },
            { SIDE_LEFT,    pins_on_side( SIDE_LEFT ) },
            { SIDE_BOTTOM,  pins_on_side( SIDE_BOTTOM ) },
        };
        std::vector<SIDE_AND_NPINS> sides( sides_init, sides_init + DIM( sides_init ) );

        int orient = m_component->GetOrientation();
        int orient_angle = orient & 0xff; // enum is a bitmask

        // If the component is horizontally mirrored, swap left and right
        if( ( orient & CMP_MIRROR_X ) && ( orient_angle == CMP_ORIENT_0 || orient_angle == CMP_ORIENT_180 ) )
        {
            std::swap( sides[0], sides[2] );
        }

        // If the component is very long, swap H and V
        if( double( m_comp_bbox.GetWidth() ) / double( m_comp_bbox.GetHeight() ) > 3.0 )
        {
            std::swap( sides[0], sides[1] );
            std::swap( sides[1], sides[3] );
        }

        return sides;
    }


    /**
     * Function get_colliding_sides
     * Return a list of the sides where a field set would collide with another item.
     */
    std::vector<SIDE> get_colliding_sides()
    {
        SIDE sides_init[] = { SIDE_RIGHT, SIDE_TOP, SIDE_LEFT, SIDE_BOTTOM };
        std::vector<SIDE> sides( sides_init, sides_init + DIM( sides_init ) );
        std::vector<SIDE> colliding;

        // Iterate over all items, and throw out the ones that are this component or
        // this component's fields
        std::vector<SCH_ITEM*> items;
        wxASSERT_MSG( m_screen, "get_colliding_sides() with null m_screen" );
        for( SCH_ITEM* item = m_screen->GetDrawItems(); item; item = item->Next() )
        {
            bool interested = true;
            if( SCH_COMPONENT* comp = dynamic_cast<SCH_COMPONENT*>( item ) )
            {
                if( comp == m_component )
                    interested = false;
                else
                {
                    std::vector<SCH_FIELD*> fields;
                    comp->GetFields( fields, /* aVisibleOnly */ true );
                    BOOST_FOREACH( SCH_FIELD* field, fields )
                        items.push_back( field );
                }
            }
            if( interested )
                items.push_back( item );
        }

        // Iterate over all sides and find the ones that collide
        BOOST_FOREACH( SIDE side, sides )
        {
            wxPoint box_pos = field_box_placement( side );
            EDA_RECT box( box_pos, m_fbox_size );

            BOOST_FOREACH( SCH_ITEM* item, items )
            {
                // SCH_COMPONENT bounding boxes include all fields, even hidden ones.
                // Since we're including fields as well anyway, we don't need fields.
                // Just use the body box
                EDA_RECT item_box;
                if( SCH_COMPONENT* item_comp = dynamic_cast<SCH_COMPONENT*>( item ) )
                    item_box = item_comp->GetBodyBoundingBox();
                else
                    item_box = item->GetBoundingBox();

                if( item_box.Intersects( box ) )
                {
                    colliding.push_back( side );
                    break;
                }
            }
        }

        return colliding;
    }


    /**
     * Function choose_side_for_component
     * Look where a component's pins are to pick a side to put the fields on
     * @param aAvoidCollisions - if true, pick last the sides where the label will collide
     *      with other items.
     */
    SIDE choose_side_for_fields( bool aAvoidCollisions )
    {
        std::vector<SIDE_AND_NPINS> sides = get_preferred_sides();

        unsigned min_pins = UINT_MAX;
        SIDE min_side = SIDE_RIGHT;

        if( aAvoidCollisions )
        {
            // Scan any colliding sides first so they end up overwritten later
            std::vector<SIDE_AND_NPINS> test_sides( sides );
            std::vector<SIDE> colliding_sides = get_colliding_sides();
            sides.clear();
            BOOST_REVERSE_FOREACH( SIDE_AND_NPINS test_side, test_sides )
            {
                bool collide = false;
                BOOST_FOREACH( SIDE coll_side, colliding_sides )
                {
                    if( coll_side == test_side.name )
                        collide = true;
                }
                if( !collide )
                    sides.insert( sides.begin(), test_side );
                else
                {
                    if( test_side.pins <= min_pins )
                    {
                        min_pins = test_side.pins;
                        min_side = test_side.name;
                    }
                }
            }
        }

        BOOST_FOREACH( SIDE_AND_NPINS& each_side, sides )
        {
            if( !each_side.pins ) return each_side.name;
        }

        BOOST_REVERSE_FOREACH( SIDE_AND_NPINS& each_side, sides )
        {
            if( each_side.pins <= min_pins )
            {
                min_pins = each_side.pins;
                min_side = each_side.name;
            }
        }

        return min_side;
    }


    /**
     * Function justify_field
     * Set the justification of a field based on the side it's supposed to be on, taking
     * into account whether the field will be displayed with flipped justification due to
     * mirroring.
     */
    void justify_field( SCH_FIELD* aField, SIDE aFieldSide )
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
        aField->SetVertJustify( GR_TEXT_VJUSTIFY_CENTER );
    }


    /**
     * Function field_box_placement
     * Returns the position of the field bounding box.
     */
    wxPoint field_box_placement( SIDE aFieldSide )
    {
        wxPoint fbox_pos;

        switch( aFieldSide )
        {
        case SIDE_RIGHT:
            fbox_pos.x = m_comp_bbox.GetRight() + HPADDING;
            fbox_pos.y = m_comp_bbox.Centre().y - m_fbox_size.GetHeight()/2;
            break;
        case SIDE_BOTTOM:
            fbox_pos.x = m_comp_bbox.GetLeft() +
                (m_comp_bbox.GetWidth() - m_fbox_size.GetWidth()) / 2;
            fbox_pos.y = m_comp_bbox.GetBottom() + VPADDING;
            break;
        case SIDE_LEFT:
            fbox_pos.x = m_comp_bbox.GetLeft() - m_fbox_size.GetWidth() - HPADDING;
            fbox_pos.y = m_comp_bbox.Centre().y - m_fbox_size.GetHeight()/2;
            break;
        case SIDE_TOP:
            fbox_pos.x = m_comp_bbox.GetLeft() +
                (m_comp_bbox.GetWidth() - m_fbox_size.GetWidth()) / 2;
            fbox_pos.y = m_comp_bbox.GetTop() - m_fbox_size.GetHeight() - VPADDING;
            break;
        default:
            wxFAIL_MSG( "Bad SIDE value" );
            fbox_pos.x = m_comp_bbox.GetRight();
            fbox_pos.y = m_comp_bbox.Centre().y - m_fbox_size.GetHeight()/2;
        }

        return fbox_pos;
    }


    /**
     * Function field_horiz_placement
     * Place a field horizontally, taking into account the field width and
     * justification.
     *
     * @param aField - the field to place.
     * @param aFieldBox - box in which fields will be placed
     *
     * @return Correct field horizontal position
     */
    int field_horiz_placement( SCH_FIELD *aField, const EDA_RECT &aFieldBox )
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
            field_xcoord = aFieldBox.Centre().x;
            break;
        case GR_TEXT_HJUSTIFY_RIGHT:
            field_xcoord = aFieldBox.GetRight();
            break;
        default:
            wxFAIL_MSG( "Unexpected value for SCH_FIELD::GetHorizJustify()" );
            field_xcoord = aFieldBox.Centre().x; // Most are centered
        }

        return field_xcoord;
    }

};


void SCH_EDIT_FRAME::OnAutoplaceFields( wxCommandEvent& aEvent )
{
    SCH_SCREEN* screen = GetScreen();
    SCH_ITEM* item = screen->GetCurItem();

    // Get the item under cursor if we're not currently moving something
    if( !item )
    {
        if( aEvent.GetInt() == 0 )
            return;

        EDA_HOTKEY_CLIENT_DATA& data = dynamic_cast<EDA_HOTKEY_CLIENT_DATA&>(
                *aEvent.GetClientObject() );
        item = LocateItem( data.GetPosition(), SCH_COLLECTOR::MovableItems, aEvent.GetInt() );
        screen->SetCurItem( NULL );
        if( !item || item->GetFlags() )
            return;
    }

    if( item->Type() != SCH_COMPONENT_T )
        return;

    if( !item->IsNew() )
        SaveCopyInUndoList( item, UR_CHANGED );

    SCH_COMPONENT& component = dynamic_cast<SCH_COMPONENT&>( *item );

    component.AutoplaceFields( screen, /* aManual */ true );

    GetCanvas()->Refresh();
    OnModify();
}


void SCH_COMPONENT::AutoplaceFields( SCH_SCREEN* aScreen, bool aManual )
{
    if( aManual )
        wxASSERT_MSG( aScreen, "A SCH_SCREEN pointer must be given for manual autoplacement" );
    AUTOPLACER autoplacer( this, aScreen );
    autoplacer.DoAutoplace( aManual );
    m_fieldsAutoplaced = true;
}
