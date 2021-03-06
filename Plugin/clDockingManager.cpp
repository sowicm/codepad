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

#define WORKSPACE_VIEW "Workspace View"
#define OUTPUT_VIEW "Output View"


clDockingManager::clDockingManager()
{
    Bind(wxEVT_AUI_PANE_BUTTON, &clDockingManager::OnButtonClicked, this);
    Bind(wxEVT_AUI_RENDER, &clDockingManager::OnRender, this);
}

clDockingManager::~clDockingManager()
{
    Unbind(wxEVT_AUI_PANE_BUTTON, &clDockingManager::OnButtonClicked, this);
    Unbind(wxEVT_AUI_RENDER, &clDockingManager::OnRender, this);
}

void clDockingManager::OnRender(wxAuiManagerEvent& event) { event.Skip(); }

void clDockingManager::OnButtonClicked(wxAuiManagerEvent& event)
{
    if (event.GetButton() == wxAUI_BUTTON_CUSTOM1)
    {
        //if (event.GetPane()->name == OUTPUT_VIEW)
        //{
            event.Veto();
            event.Skip(false);
            CallAfter(&clDockingManager::BuildAndExecute);
            return;
        //}
    }

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

void clDockingManager::BuildAndExecute()
{
    clCommandEvent event(wxEVT_BUILD_AND_EXECUTE);
    EventNotifier::Get()->AddPendingEvent(event);
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
