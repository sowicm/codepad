//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : sessionmanager.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "sessionmanager.h"
#include "xmlutils.h"
#include "wx/ffile.h"
#include "wx/jsonreader.h"
#include "wx/jsonwriter.h"
#include <wx/log.h>

#include <memory>
#include "wx_xml_compatibility.h"
#include "cl_standard_paths.h"

// Session entry
SessionEntry::SessionEntry() {}

SessionEntry::~SessionEntry() {}

void SessionEntry::DeSerialize(wxJSONValue& root)
{
    m_selectedTab = root["selected_tab"].AsInt();
/*    wxJSONValue tabs = root["tabs"];
    m_tabs.Clear();
    for (int i = 0; i < tabs.Size(); ++i)
    {
        m_tabs.Add(tabs[i].AsString());
    }
*/
    m_workspaceName = root["workspace"]["name"].AsString();

    wxJSONValue buffers = root["buffers"];
    m_vTabInfoArr.clear();
    for (int i = 0; i < buffers.Size(); ++i)
    {
        TabInfo oTabInfo;
        if (buffers[i].HasMember("file"))
        {
            oTabInfo.SetFileName(buffers[i]["file"].AsString());
        }
        oTabInfo.SetFirstVisibleLine(buffers[i]["first_visible_line"].AsInt());
        oTabInfo.SetCurrentLine(buffers[i]["current_line"].AsInt());
        //oTabInfo.m_bookmarks
        //oTabInfo.m_folds (collapsedfolds)
        //

        if (buffers[i].HasMember("contents"))
        {
            oTabInfo.unsaved(true);
            oTabInfo.contents(buffers[i]["contents"].AsString());
        }

        m_vTabInfoArr.push_back(oTabInfo);
    }

//    arch.Read(wxT("m_breakpoints"), (SerializedObject*)&m_breakpoints);
}

void SessionEntry::Serialize(wxJSONValue& root)
{
    root["selected_tab"] = m_selectedTab;
    root["workspace"]["name"] = m_workspaceName;
    for (size_t i = 0; i < m_vTabInfoArr.size(); i++)
    {
        TabInfo& oTabInfo = m_vTabInfoArr[i];
        if (oTabInfo.GetFileName().Length() > 0)
        {
            root["buffers"][i]["file"] = oTabInfo.GetFileName();
        }
        root["buffers"][i]["first_visible_line"] = oTabInfo.GetFirstVisibleLine();
        root["buffers"][i]["current_line"] = oTabInfo.GetCurrentLine();
        if (oTabInfo.unsaved())
        {
            root["buffers"][i]["contents"] = oTabInfo.contents();
        }
    }
   // arch.Write(wxT("m_breakpoints"), (SerializedObject*)&m_breakpoints);
}

//---------------------------------------------

void TabGroupEntry::DeSerialize(Archive& arch)
{
    arch.Read(wxT("m_TabgroupName"), m_tabgroupName);
    arch.Read(wxT("TabInfoArray"), m_vTabInfoArr);
}

void TabGroupEntry::Serialize(Archive& arch)
{
    arch.Write(wxT("m_TabgroupName"), m_tabgroupName);
    arch.Write(wxT("TabInfoArray"), m_vTabInfoArr);
}

//---------------------------------------------
SessionManager& SessionManager::Get()
{
    static SessionManager theManager;
    return theManager;
}

SessionManager::SessionManager() {}

SessionManager::~SessionManager() {}

bool SessionManager::Load(const wxString& fileName)
{
    m_fileName = wxFileName(fileName);

    if(!m_fileName.FileExists()) {
        return true;
    }

    wxJSONReader reader;
    wxString content;
    wxFFile fp(m_fileName.GetFullPath(), wxT("rb"));
    if (!fp.IsOpened())
        return false;
    if (!fp.ReadAll(&content, wxConvUTF8))
        return false;
    if (reader.Parse(content, &m_json) > 0)
        return false;
    return true;
}

wxFileName SessionManager::GetSessionFileName(const wxString& name, const wxString& suffix /*=wxT("")*/) const
{
    if(defaultSessionName == name) {
        wxFileName sessionFileName = wxFileName(clStandardPaths::Get().GetUserDataDir(), "Auto Save Session.session");
        sessionFileName.AppendDir("config");
        return sessionFileName;

    } else {
        wxFileName sessionFileName(name);
        if(suffix != "tabgroup") {
            sessionFileName.AppendDir(".codelite");
        }
        sessionFileName.SetExt(suffix.IsEmpty() ? wxString("session") : suffix);
        return sessionFileName;
    }
}

bool SessionManager::GetSession(const wxString& workspaceFile,
                                SessionEntry& session,
                                const wxString& suffix,
                                const wxChar* Tag)
{
    wxFileName sessionFileName = GetSessionFileName(workspaceFile, suffix);
    wxXmlDocument doc;
    wxJSONValue root;
    wxJSONReader reader;

    if (sessionFileName.FileExists()) {
        wxString content;
        wxFFile fp(sessionFileName.GetFullPath(), wxT("rb"));
        if (!fp.IsOpened())
            return false;
        if (!fp.ReadAll(&content, wxConvUTF8))
            return false;
        //if (reader.Parse(content.mb_str(wxConvUTF8).data(), root) > 0)
        if (reader.Parse(content, &root) > 0)
            return false;
    } else {
        root["name"] = Tag;
        //doc.SetRoot(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, Tag));
    }

    //wxXmlNode* const node = doc.GetRoot();
    //if(!node || node->GetName() != Tag) return false;
    //if (root["name"].AsString() != Tag)
    //    return false;

//    Archive arch;
//    arch.SetXmlNode(node);
    session.DeSerialize(root);

    return true;
}

bool SessionManager::Save(const wxString& name,
                          SessionEntry& session,
                          const wxString& suffix /*=wxT("")*/,
                          const wxChar* Tag /*=sessionTag*/)
{
    if(name.empty()) return false;

    wxJSONValue root;
    wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
    session.Serialize(root);
    wxString jsonText;
    writer.Write(root, jsonText);

    // If we're saving a tabgroup, suffix will be ".tabgroup", not the default ".session"
    const wxFileName& sessionFileName = GetSessionFileName(name, suffix);

    wxFFile fp(sessionFileName.GetFullPath(), wxT("wb"));
    if (fp.IsOpened())
    {
        fp.Write(jsonText, wxConvUTF8);
        fp.Close();
        return true;
    }

    return false;
}

void SessionManager::SetLastSession(const wxString& name)
{
    m_json["last_session"] = name;
    wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
    wxString jsonText;
    writer.Write(m_json, jsonText);
    wxFFile fp(m_fileName.GetFullPath(), wxT("wb"));
    if (fp.IsOpened())
    {
        fp.Write(jsonText, wxConvUTF8);
        fp.Close();
    }
}

wxString SessionManager::GetLastSession()
{
    if (m_json.HasMember("last_session") && m_json["last_session"].AsString().Length() > 0)
        return m_json["last_session"].AsString();
    else
        return defaultSessionName;
}
