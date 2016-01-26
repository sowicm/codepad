#include "clDockingManager.h"
#include "imanager.h"
#include "globals.h"
#include "Notebook.h"
#include <wx/menu.h>
#include <map>
#include "codelite_events.h"
#include "event_notifier.h"
#include "detachedpanesinfo.h"
#include "editor_config.h"
#include <wx/aui/dockart.h>
#include "plugin_general_wxcp.h"

#define WORKSPACE_VIEW "Workspace View"
#define OUTPUT_VIEW "Output View"

class WXDLLIMPEXP_AUI clDockingArt : public wxAuiDefaultDockArt
{
public:
//    clDockingArt();
    void DrawPaneButton(wxDC& dc,
                  wxWindow *window,
                  int button,
                  int buttonState,
                  const wxRect& rect,
                  wxAuiPaneInfo& pane);

protected:
    void InitBitmaps();

    wxBitmap m_runButtonBitmap;

};

void clDockingArt::InitBitmaps()
{
    wxAuiDefaultDockArt::InitBitmaps();

    GeneralImages img;
    m_runButtonBitmap = img.Bitmap("runButton");
}

void clDockingArt::DrawPaneButton(wxDC& dc, wxWindow *window,
                                      int button,
                                      int button_state,
                                      const wxRect& _rect,
                                      wxAuiPaneInfo& pane)
{
    throw "error";
    if (button != wxAUI_BUTTON_CUSTOM1)
    {
        wxAuiDefaultDockArt::DrawPaneButton(dc, window, button, button_state, _rect, pane);
        return;
    }


        wxBitmap bmp = m_runButtonBitmap;//wxXmlResource::Get()->LoadBitmap(wxT("tabClose"));

        /*
        switch (button)
        {
            default:
            case wxAUI_BUTTON_CLOSE:
                if (pane.state & wxAuiPaneInfo::optionActive)
                    bmp = m_activeCloseBitmap;
                else
                    bmp = m_inactiveCloseBitmap;
                break;
            case wxAUI_BUTTON_PIN:
                if (pane.state & wxAuiPaneInfo::optionActive)
                    bmp = m_activePinBitmap;
                else
                    bmp = m_inactivePinBitmap;
                break;
            case wxAUI_BUTTON_MAXIMIZE_RESTORE:
                if (pane.IsMaximized())
                {
                    if (pane.state & wxAuiPaneInfo::optionActive)
                        bmp = m_activeRestoreBitmap;
                    else
                        bmp = m_inactiveRestoreBitmap;
                }
                else
                {
                    if (pane.state & wxAuiPaneInfo::optionActive)
                        bmp = m_activeMaximizeBitmap;
                    else
                        bmp = m_inactiveMaximizeBitmap;
                }
                break;
        }
        */

static int stopChange = 0;

        wxRect rect = _rect;

        if (!stopChange)
        {
            int old_y = rect.y;
            rect.y = rect.y + (rect.height/2) - (bmp.GetHeight()/2);
            //rect.height = old_y + rect.height - rect.y - 1;
            rect.x = rect.x - bmp.GetWidth();
            rect.width = bmp.GetWidth();
            rect.height = bmp.GetHeight();
            //first_time = 0;
        }

/*
        if (button_state == wxAUI_BUTTON_STATE_PRESSED)
        {
            rect.x++;
            rect.y++;
        }

        if (button_state == wxAUI_BUTTON_STATE_HOVER ||
            button_state == wxAUI_BUTTON_STATE_PRESSED)
        {
            if (pane.state & wxAuiPaneInfo::optionActive)
            {
                dc->SetBrush(wxBrush(m_activeCaptionColour.ChangeLightness(120)));
                dc->SetPen(wxPen(m_activeCaptionColour.ChangeLightness(70)));
            }
            else
            {
                dc->SetBrush(wxBrush(m_inactiveCaptionColour.ChangeLightness(120)));
                dc->SetPen(wxPen(m_inactiveCaptionColour.ChangeLightness(70)));
            }

            // draw the background behind the button
            dc->DrawRectangle(rect.x, rect.y, 15, 15);
        }
*/

        dc.DrawLabel(wxT("Run"), rect);

        // draw the button itself
        dc.DrawBitmap(bmp, rect.x, rect.y, true);


}


clDockingManager::clDockingManager()
{
    Bind(wxEVT_AUI_PANE_BUTTON, &clDockingManager::OnButtonClicked, this);
    Bind(wxEVT_AUI_RENDER, &clDockingManager::OnRender, this);

    delete m_art;
    m_art = new clDockingArt;
}

clDockingManager::~clDockingManager()
{
    Unbind(wxEVT_AUI_PANE_BUTTON, &clDockingManager::OnButtonClicked, this);
    Unbind(wxEVT_AUI_RENDER, &clDockingManager::OnRender, this);
}

void clDockingManager::OnRender(wxAuiManagerEvent& event) { event.Skip(); }

void clDockingManager::OnButtonClicked(wxAuiManagerEvent& event)
{
    if(event.GetButton() != wxAUI_BUTTON_PIN) {
        event.Skip();
        return;
    }
    event.Veto();
    event.Skip(false);

    if(event.GetPane()->name == OUTPUT_VIEW) {
        CallAfter(&clDockingManager::ShowOutputViewOpenTabMenu);
    } else {
        CallAfter(&clDockingManager::ShowWorkspaceOpenTabMenu);
    }
}

void clDockingManager::ShowWorkspaceOpenTabMenu()
{
    // Override the "pin" button to show a window list
    bool show;
    wxString selection = ShowMenu(GetPane(WORKSPACE_VIEW).window,
                                  clGetManager()->GetWorkspaceTabs(),
                                  clGetManager()->GetWorkspacePaneNotebook(),
                                  show);
    CHECK_COND_RET(!selection.IsEmpty());
    clCommandEvent event(wxEVT_SHOW_WORKSPACE_TAB);
    event.SetSelected(show).SetString(selection);
    EventNotifier::Get()->AddPendingEvent(event);
}

void clDockingManager::ShowOutputViewOpenTabMenu()
{
    bool show;
    wxString selection = ShowMenu(
        GetPane(OUTPUT_VIEW).window, clGetManager()->GetOutputTabs(), clGetManager()->GetOutputPaneNotebook(), show);
    CHECK_COND_RET(!selection.IsEmpty());
    clCommandEvent event(wxEVT_SHOW_OUTPUT_TAB);
    event.SetSelected(show).SetString(selection);
    EventNotifier::Get()->AddPendingEvent(event);
}

wxString clDockingManager::ShowMenu(wxWindow* win, const wxArrayString& tabs, Notebook* book, bool& checked)
{
    DetachedPanesInfo dpi;
    EditorConfigST::Get()->ReadObject("DetachedPanesList", &dpi);
    
    std::map<int, wxString> tabsIds;
    wxMenu menu(_("Toggle Tabs"));
    for(size_t i = 0; i < tabs.size(); ++i) {
        int tabId = ::wxNewId();
        const wxString& label = tabs.Item(i);
        tabsIds.insert(std::make_pair(tabId, label));
        wxMenuItem* item = new wxMenuItem(&menu, tabId, label, "", wxITEM_CHECK);
        menu.Append(item);
        item->Check((book->GetPageIndex(label) != wxNOT_FOUND));
        
        // Output pane does not support "detach"
        if((book != clGetManager()->GetOutputPaneNotebook()) && dpi.GetPanes().Index(label) != wxNOT_FOUND) {
            item->Enable(false);
        }
    }

    int sel = win->GetPopupMenuSelectionFromUser(menu);
    if(sel == wxID_NONE) return "";
    const wxString& label = tabsIds.find(sel)->second;
    checked = (book->GetPageIndex(label) == wxNOT_FOUND);
    return label;
}
