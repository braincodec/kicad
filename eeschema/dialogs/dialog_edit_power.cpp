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

#include <fctsys.h>
#include <wx/valgen.h>
#include <schframe.h>
#include <base_units.h>

#include <class_drawpanel.h>
#include <general.h>
#include <drawtxt.h>
#include <confirm.h>
#include <sch_text.h>
#include <sch_power.h>
#include <typeinfo>

#include <dialog_edit_power_base.h>

#include <iostream>

class SCH_EDIT_FRAME;

class DIALOG_EDIT_POWER: public DIALOG_EDIT_POWER_BASE
{
public:
    DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem );

private:
    SCH_EDIT_FRAME* m_parent;
    SCH_POWER* m_poweritem;

    bool TransferDataToWindow();
    bool TransferDataFromWindow();
    void OnCancelClick( wxCommandEvent& event );
    void OnEnterKey( wxCommandEvent& event );
};

DIALOG_EDIT_POWER::DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem )
    : DIALOG_EDIT_POWER_BASE( aParent ),
      m_parent( aParent ),
      m_poweritem( aPowerItem )
{
    m_stdButtonsOK->SetDefault();
}

bool DIALOG_EDIT_POWER::TransferDataToWindow()
{
    if( !wxDialog::TransferDataToWindow() )
        return false;

    m_textNet->SetValue( m_poweritem->GetText() );
    GetSizer()->Layout();
    GetSizer()->Fit( this );
    GetSizer()->SetSizeHints( this );
    Centre();
    return true;
}

bool DIALOG_EDIT_POWER::TransferDataFromWindow()
{
    if( !wxDialog::TransferDataFromWindow() )
        return false;

    m_poweritem->SetText( m_textNet->GetValue() );
    m_poweritem->Resolve( Prj().SchLibs() );
    return true;
}

void DIALOG_EDIT_POWER::OnCancelClick( wxCommandEvent& aEvent )
{
    m_textNet->SetValue( m_poweritem->GetText() );
    DIALOG_EDIT_POWER_BASE::OnCancelClick( aEvent );
}

void DIALOG_EDIT_POWER::OnEnterKey( wxCommandEvent& aEvent )
{
    DIALOG_EDIT_POWER_BASE::OnOkClick( aEvent );
    Close();
}

void SCH_EDIT_FRAME::EditPower( SCH_POWER* aPowerItem )
{
    if( !aPowerItem )
        return;

    DIALOG_EDIT_POWER dialog( this, aPowerItem );
    dialog.ShowModal();
}
