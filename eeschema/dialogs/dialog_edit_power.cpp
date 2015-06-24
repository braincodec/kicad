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
    bool TransferDataFromWindow( SCH_POWER* aPoweritem );
    bool TransferDataFromWindow();
    void OnCancelClick( wxCommandEvent& event );
    void OnEnterKey( wxCommandEvent& event );
    void OnToggleAdvanced( wxCommandEvent& event );
    void OnDataChanged( wxCommandEvent& event );
    void OnPreviewRepaint( wxPaintEvent& aRepaintEvent );
};


DIALOG_EDIT_POWER::DIALOG_EDIT_POWER( SCH_EDIT_FRAME* aParent, SCH_POWER* aPowerItem )
    : DIALOG_EDIT_POWER_BASE( aParent ),
      m_parent( aParent ),
      m_poweritem( aPowerItem )
{
    m_stdButtonsOK->SetDefault();
    m_panAdvanced->Hide();
    m_textNet->SetFocus();
}


bool DIALOG_EDIT_POWER::TransferDataToWindow()
{
    if( !wxDialog::TransferDataToWindow() )
        return false;

    m_textNet->SetValue( m_poweritem->GetText() );
    m_textLabelText->SetValue( m_poweritem->GetVisibleText() );
    m_cbHideLabel->SetValue( m_poweritem->GetLabelHidden() );
    GetSizer()->Layout();
    GetSizer()->Fit( this );
    GetSizer()->SetSizeHints( this );
    Centre();
    return true;
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow()
{
    return TransferDataFromWindow( m_poweritem );
}


bool DIALOG_EDIT_POWER::TransferDataFromWindow( SCH_POWER* aPoweritem )
{
    if( !wxDialog::TransferDataFromWindow() )
        return false;

    aPoweritem->SetText( m_textNet->GetValue() );
    aPoweritem->SetVisibleText( m_textLabelText->GetValue() );
    aPoweritem->SetLabelHidden( m_cbHideLabel->GetValue() );
    aPoweritem->Resolve( Prj().SchLibs() );
    return true;
}


void DIALOG_EDIT_POWER::OnCancelClick( wxCommandEvent& aEvent )
{
    DIALOG_EDIT_POWER_BASE::OnCancelClick( aEvent );
    m_textNet->SetValue( m_poweritem->GetText() );
    m_parent->GetCanvas()->MoveCursorToCrossHair();
}


void DIALOG_EDIT_POWER::OnEnterKey( wxCommandEvent& aEvent )
{
    DIALOG_EDIT_POWER_BASE::OnOkClick( aEvent );
    Close();
}


void DIALOG_EDIT_POWER::OnToggleAdvanced( wxCommandEvent& aEvent )
{
    bool show = m_btnAdvanced->GetValue();
    if( show )
        m_panAdvanced->Show();
    else
        m_panAdvanced->Hide();
    GetSizer()->Layout();
}


void DIALOG_EDIT_POWER::OnDataChanged( wxCommandEvent& event )
{
    m_pPreview->Refresh();
}


void DIALOG_EDIT_POWER::OnPreviewRepaint( wxPaintEvent& aRepaintEvent )
{
    SCH_POWER temp_item( *m_poweritem );
    TransferDataFromWindow( &temp_item );
    wxPaintDC dc( m_pPreview );
    EDA_COLOR_T bgcolor = m_parent->GetDrawBgColor();

    dc.SetBackground( bgcolor == BLACK ? *wxBLACK_BRUSH : *wxWHITE_BRUSH );
    dc.Clear();

    const wxSize dc_size = dc.GetSize();
    dc.SetDeviceOrigin( dc_size.x / 2, dc_size.y / 2 );

    EDA_RECT bbox = temp_item.GetBoundingBox();
    const double xscale = (double) dc_size.x / bbox.GetWidth();
    const double yscale = (double) dc_size.y / bbox.GetHeight();
    const double scale = std::min( xscale, yscale ) * 0.85;

    dc.SetUserScale( scale, scale );
    wxPoint offset = -bbox.Centre();

    int width, height;
    dc.GetSize( &width, &height );
    if( !width || !height )
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
