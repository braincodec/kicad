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
#include <class_drawpanel.h>
#include <iostream>

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

    SCH_COMPONENT *component = dynamic_cast<SCH_COMPONENT*>( item );

    EDA_RECT body_box = component->GetBodyBoundingBox();

    int max_field_width = 0;
    for( size_t field_idx = 0; field_idx < component->GetFieldCount(); ++field_idx )
    {
        SCH_FIELD* field = component->GetField( field_idx );
        int field_width = field->GetBoundingBox().GetWidth();
        if( field_width > max_field_width )
            max_field_width = field_width;
    }

    // New field position will be the right of the body + half the field width, rounded up to 50
    int new_field_pos = body_box.GetRight() + (max_field_width / 2);
    if( new_field_pos % 50 )
        new_field_pos = 50 * (new_field_pos / 50 + 1);

    // Move the field
    for( size_t field_idx = 0; field_idx < component->GetFieldCount(); ++field_idx )
    {
        SCH_FIELD* field = component->GetField( field_idx );
        wxPoint pos = field->GetPosition();
        pos.x = new_field_pos;
        field->SetPosition( pos );
    }

    GetCanvas()->Refresh();
}
