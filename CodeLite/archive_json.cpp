//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2016 by Sowicm
// file name            : archive.cpp
//
// -------------------------------------------------------------------------
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "archive_json.h"
#include <wx/colour.h>
#include <wx/xml/xml.h>
#include "serialized_object.h"

#include <memory>
#include "wx_xml_compatibility.h"
#include "clFontHelper.h"

namespace
{
const wxChar breakpointName[] = wxT("Breakpoint");
const wxChar breakpointArrayName[] = wxT("BreakpointArray");
}

// helper functions
static wxXmlNode* FindNodeByName(const wxXmlNode* parent, const wxString& tagName, const wxJSONValue& name)
{
    if(!parent) {
        return NULL;
    }

    wxXmlNode* child = parent->GetChildren();
    while(child) {
        if(child->GetName() == tagName) {
            if(child->GetPropVal(wxT("Name"), wxEmptyString) == name) {
                return child;
            }
        }
        child = child->GetNext();
    }
    return NULL;
}

static void SetNodeContent(wxXmlNode* node, const wxString& text)
{
    wxXmlNode* n = node->GetChildren();
    wxXmlNode* contentNode = NULL;
    while(n) {
        if(n->GetType() == wxXML_TEXT_NODE || n->GetType() == wxXML_CDATA_SECTION_NODE) {
            contentNode = n;
            break;
        }
        n = n->GetNext();
    }

    if(contentNode) {
        // remove old node
        node->RemoveChild(contentNode);
        delete contentNode;
    }

    if(!text.IsEmpty()) {
        contentNode = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, text);
        node->AddChild(contentNode);
    }
}

static void SetCDATANodeContent(wxXmlNode* node, const wxString& text)
{
    wxXmlNode* n = node->GetChildren();
    wxXmlNode* contentNode = NULL;
    while(n) {
        if(n->GetType() == wxXML_TEXT_NODE || n->GetType() == wxXML_CDATA_SECTION_NODE) {
            contentNode = n;
            break;
        }
        n = n->GetNext();
    }

    if(contentNode) {
        // remove old node
        node->RemoveChild(contentNode);
        delete contentNode;
    }

    if(!text.IsEmpty()) {
        contentNode = new wxXmlNode(wxXML_CDATA_SECTION_NODE, wxEmptyString, text);
        node->AddChild(contentNode);
    }
}

// class Tab info
TabInfo::TabInfo() {}

TabInfo::~TabInfo() {}

void TabInfo::DeSerialize(Archive& arch)
{
    arch.Read(wxT("FileName"), m_fileName);
    arch.Read(wxT("FirstVisibleLine"), m_firstVisibleLine);
    arch.Read(wxT("CurrentLine"), m_currentLine);
    arch.Read(wxT("Bookmarks"), m_bookmarks);
    arch.Read(wxT("CollapsedFolds"), m_folds);
}

void TabInfo::Serialize(Archive& arch)
{
    arch.Write(wxT("FileName"), m_fileName);
    arch.Write(wxT("FirstVisibleLine"), m_firstVisibleLine);
    arch.Write(wxT("CurrentLine"), m_currentLine);
    arch.Write(wxT("Bookmarks"), m_bookmarks);
    arch.Write(wxT("CollapsedFolds"), m_folds);
}

// class Archive
ArchiveJson::ArchiveJson()
    : m_root(NULL)
{
}

ArchiveJson::~ArchiveJson() {}

bool ArchiveJson::Write(const wxJSONValue& name, const wxArrayString& arr)
{
    for (size_t i = 0; i < arr.GetCount(); ++i)
        name.Append(arr.Item(i));
    return true;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxArrayString& arr)
{
    arr.Clear();
    for (int i = 0; i < name.Size(); ++i)
    {
        arr.Add(name[i].AsString());
    }
    return true;
}

bool ArchiveJson::Write(const wxJSONValue& name, std::vector<TabInfo>& _vTabInfoArr)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("TabInfoArray"));
    m_root->AddChild(node);
    node->AddProperty(wxT("Name"), name);

    // add an entry for each wxString in the array
    for (size_t i = 0; i < _vTabInfoArr.size(); i++)
    {
        wxJSONValue child;
        ArchiveJson arch;
        archi.SetJsonValue(child);
        _vTabInfoArr[i].Serialize(arch);
        name.Append(child);
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("TabInfo"));
        Archive arch;
        arch.SetXmlNode(child);
        _vTabInfoArr[i].Serialize(arch);
        node->AddChild(child);
    }
    return true;
}

bool ArchiveJson::Read(const wxJSONValue& name, std::vector<TabInfo>& _vTabInfoArr)
{
    if(!m_root) {
        return false;
    }

    Archive arch;
    wxXmlNode* node = FindNodeByName(m_root, wxT("TabInfoArray"), name);
    if(node) {
        // fill the output array with the values
        _vTabInfoArr.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("TabInfo")) {
                arch.SetXmlNode(child);
                TabInfo oTabInfo;
                oTabInfo.DeSerialize(arch);
                _vTabInfoArr.push_back(oTabInfo);
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool ArchiveJson::Read(const wxJSONValue& name, std::vector<int>& _vInt)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("IntVector"), name);
    if(node) {
        // fill the output array with the values
        _vInt.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("IntVectorItem")) {
                long value;
                wxString stringvalue = child->GetPropVal(wxT("Value"), wxEmptyString);
                if(stringvalue.ToLong(&value)) {
                    _vInt.push_back(value);
                }
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool ArchiveJson::Read(const wxJSONValue& name, StringMap& str_map)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("StringMap"), name);
    if(node) {
        // fill the output array with the values
        str_map.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("MapEntry")) {
                wxString value;
                wxString key;
                key = child->GetPropVal(wxT("Key"), wxEmptyString);
                value = child->GetPropVal(wxT("Value"), wxEmptyString);
                str_map[key] = value;
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxSize& size)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("wxSize"), name);
    if(node) {
        long v;
        wxString value;
        value = node->GetPropVal(wxT("x"), wxEmptyString);
        value.ToLong(&v);
        size.x = v;

        value = node->GetPropVal(wxT("y"), wxEmptyString);
        value.ToLong(&v);
        size.y = v;
        return true;
    }
    return false;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxPoint& pt)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("wxPoint"), name);
    if(node) {
        long v;
        wxString value;
        value = node->GetPropVal(wxT("x"), wxEmptyString);
        value.ToLong(&v);
        pt.x = v;

        value = node->GetPropVal(wxT("y"), wxEmptyString);
        value.ToLong(&v);
        pt.y = v;
        return true;
    }
    return false;
}

bool ArchiveJson::Write(const wxJSONValue& name, int value)
{
    name = value;
}

bool ArchiveJson::Read(const wxJSONValue& name, int& value)
{
    value = name.AsInt();
    return true;
}

bool ArchiveJson::Write(const wxJSONValue& name, bool value) { return WriteSimple(value ? 1 : 0, wxT("bool"), name); }

bool ArchiveJson::Read(const wxJSONValue& name, bool& value)
{
    long v;
    bool res = ReadSimple(v, wxT("bool"), name);
    if(res) {
        v == 0 ? value = false : value = true;
    }
    return res;
}

bool ArchiveJson::Write(const wxJSONValue& name, const wxString& str)
{
    name = str;
    return true;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxString& value)
{
    value = name.AsString();
    return true;
}

bool ArchiveJson::Read(const wxJSONValue& name, size_t& value)
{
    long v = 0;
    bool res = Read(name, v);
    if(res) {
        value = v;
    }
    return res;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxFileName& fileName)
{
    wxString value;
    bool res = Read(name, value);
    if(res) {
        fileName = wxFileName(value);
    }
    return res;
}

bool ArchiveJson::Write(const wxJSONValue& name, size_t value) { return Write(name, (long)value); }

bool ArchiveJson::Write(const wxJSONValue& name, const wxFileName& fileName) { return Write(name, fileName.GetFullPath()); }

void ArchiveJson::SetXmlNode(wxXmlNode* node) { m_root = node; }

bool ArchiveJson::WriteSimple(long value, const wxString& typeName, const wxJSONValue& name)
{
    if(!m_root) return false;

    wxString propValue;
    propValue << value;

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, typeName);
    m_root->AddChild(node);
    node->AddProperty(wxT("Value"), propValue);
    node->AddProperty(wxT("Name"), name);
    return true;
}

bool ArchiveJson::ReadSimple(long& value, const wxString& typeName, const wxJSONValue& name)
{
    if(!m_root) return false;

    value = 0;
    wxXmlNode* node = FindNodeByName(m_root, typeName, name);
    if(node) {
        wxString val = node->GetPropVal(wxT("Value"), wxEmptyString);
        val.ToLong(&value);
        return true;
    }
    return false;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxColour& colour)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("wxColour"), name);
    wxString value;
    if(node) {
        value = node->GetPropVal(wxT("Value"), wxEmptyString);
    }

    if(value.IsEmpty()) {
        return false;
    }

    colour = wxColour(value);
    return true;
}

bool ArchiveJson::Write(const wxJSONValue& name, const wxColour& colour)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxColour"));
    m_root->AddChild(node);
    node->AddProperty(wxT("Value"), colour.GetAsString());
    node->AddProperty(wxT("Name"), name);
    return true;
}

bool ArchiveJson::Write(const wxJSONValue& name, const std::map<wxString, wxString>& strinMap)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("std_string_map"));
    m_root->AddChild(node);
    node->AddProperty(wxT("Name"), name);

    // add an entry for each wxString in the array
    std::map<wxString, wxString>::const_iterator iter = strinMap.begin();
    for(; iter != strinMap.end(); iter++) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("MapEntry"));
        node->AddChild(child);
        child->AddProperty(wxT("Key"), iter->first);
        SetNodeContent(child, iter->second);
    }
    return true;
}

bool ArchiveJson::Read(const wxJSONValue& name, std::map<wxString, wxString>& strinMap)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("std_string_map"), name);
    if(node) {
        // fill the output array with the values
        strinMap.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("MapEntry")) {
                wxString value;
                wxString key;
                key = child->GetPropVal(wxT("Key"), wxEmptyString);
                value = child->GetNodeContent();
                strinMap[key] = value;
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool ArchiveJson::Read(const wxJSONValue& name, std::set<wxString>& s)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = FindNodeByName(m_root, wxT("std_string_set"), name);
    if(node) {
        // fill the output array with the values
        s.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("SetEntry")) {
                wxString value;
                value = child->GetNodeContent();
                s.insert(value);
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool ArchiveJson::Write(const wxJSONValue& name, const std::set<wxString>& s)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("std_string_set"));
    m_root->AddChild(node);
    node->AddProperty(wxT("Name"), name);

    // add an entry for each wxString in the array
    std::set<wxString>::const_iterator iter = s.begin();
    for(; iter != s.end(); iter++) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("SetEntry"));
        node->AddChild(child);
        SetNodeContent(child, *iter);
    }
    return true;
}

bool ArchiveJson::Read(const wxJSONValue& name, wxFont& font, const wxFont& defaultFont) 
{
    wxString strFont;
    bool res = Read(name, strFont);
    if(!res || strFont.IsEmpty()) {
        font = defaultFont;
        return false;
    }
    font = clFontHelper::FromString(strFont);
    return true;
}

bool ArchiveJson::Write(const wxJSONValue& name, const wxFont& font) 
{
    return Write(name, clFontHelper::ToString(font));
}
