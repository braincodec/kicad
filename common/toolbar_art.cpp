/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2016 KiCad Developers, see CHANGELOG.TXT for contributors.
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
 
#include <iostream>
//#include <wx/menu.h>

#include <wx/aui/framemanager.h>
#include <wx/dcclient.h>
 
#include <toolbar_art.h>

class ToolbarCommandCapture : public wxEvtHandler
{
public:

    ToolbarCommandCapture() { m_lastId = 0; }
    int GetCommandId() const { return m_lastId; }

    bool ProcessEvent(wxEvent& evt)
    {
        std::cout << "HERE" << std::endl;
        if (evt.GetEventType() == wxEVT_MENU)
        {
            m_lastId = evt.GetId();
            return true;
        }

        if (GetNextHandler())
            return GetNextHandler()->ProcessEvent(evt);

        return false;
    }

private:
    int m_lastId;
};

int TOOLBAR_ART::ShowDropDown( wxWindow* aWindow, const wxAuiToolBarItemArray& aItems )
{
    if ( m_toolbar ) {
        delete m_toolbar;
        m_toolbar = NULL;
        return -1;
    }
    wxRect rect = aWindow->GetRect();
    
    m_toolbar = new wxAuiToolBar(aWindow->GetParent(), wxID_ANY, rect.GetBottomRight(),
                                             wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
    size_t i, count = aItems.GetCount();
    for (i = 0; i < count; ++i )
    {
        wxAuiToolBarItem& item = aItems.Item( i );
        m_toolbar->AddTool( item.GetId(), item.GetLabel(), item.GetBitmap(), item.GetShortHelp(), (wxItemKind)item.GetKind() );
        if ( item.GetKind() == wxITEM_CHECK )
            m_toolbar->ToggleTool( item.GetId(), item.GetState() );
    }
    m_toolbar->Realize();
    wxAuiManager* mgr = wxAuiManager::GetManager(aWindow->GetParent());
    
    ToolbarCommandCapture* cc = new ToolbarCommandCapture;
    aWindow->GetParent()->PushEventHandler(cc);
    int command = cc->GetCommandId();
    aWindow->GetParent()->PopEventHandler(true);
    
    mgr->AddPane(m_toolbar, wxAuiPaneInfo::wxAuiPaneInfo().Float().Show().FloatingSize(m_toolbar->GetSize()));
    
    // Return -1 because otherwise the wxCommandEvent handler will break wxITEM_CHECK
    return -1;
}
