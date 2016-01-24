//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2016 by Sowicm
// file name            : archive_json.h
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

#ifndef ARCHIVE_JSON_H
#define ARCHIVE_JSON_H

#include "wx/string.h"
#include "wx/hashmap.h"
#include <wx/arrstr.h>
#include "wx/filename.h"
#include <wx/gdicmn.h>
#include <vector>
#include <map>
#include <set>
#include "codelite_exports.h"
#include <wx/font.h>
#include "wx/jsonval.h"


class wxXmlNode;
class SerializedObject;
class TabInfo;
class BreakpointInfo;

WX_DECLARE_STRING_HASH_MAP(wxString, StringMap);

/**
 * \class Archive
 * \brief an auxulariy class which serializes variables into XML format
 * \author Eran
 * \date 07/20/07
 */
class WXDLLIMPEXP_CL ArchiveJson
{
    wxJSONValue* m_root;

public:
    ArchiveJson();
    virtual ~ArchiveJson();

    /**
     * \brief set a root node for this Archive object, all Write operations will append their nodes
     * to this node
     * \param node wxXmlNode to act as the root
     */
    void SetJsonValue(wxJSONValue* root);

    //--------------------
    // Write API
    //--------------------
//    bool Write(const wxJSONValue& name, SerializedObject* obj);
    bool Write(const wxJSONValue& name, int value);
//    bool Write(const wxJSONValue& name, bool value);
    bool Write(const wxJSONValue& name, const wxString& str);
    bool Write(const wxJSONValue& name, const wxArrayString& arr);
//    bool Write(const wxJSONValue& name, const wxFileName& fileName);
//    bool Write(const wxJSONValue& name, size_t value);
//    bool Write(const wxJSONValue& name, wxSize size);
//    bool Write(const wxJSONValue& name, wxPoint pt);
//    bool Write(const wxJSONValue& name, const StringMap& str_map);
//    bool Write(const wxJSONValue& name, const wxColour& colour);
    bool Write(const wxJSONValue& name, std::vector<TabInfo>& _vTabInfoArr);
//    bool Write(const wxJSONValue& name, std::vector<int>& _vInt);
//    bool Write(const wxJSONValue& name, const std::map<wxString, wxString>& strinMap);
//    bool Write(const wxJSONValue& name, const std::set<wxString>& s);
//    bool Write(const wxJSONValue& name, const wxFont& font);
//    bool WriteCData(const wxJSONValue& name, const wxString& value);

    //--------------------
    // Read API
    //--------------------
    bool Read(const wxJSONValue& name, int& value);
//    bool Read(const wxJSONValue& name, bool& value);
    bool Read(const wxJSONValue& name, wxString& value);
    bool Read(const wxJSONValue& name, wxArrayString& arr);
//    bool Read(const wxJSONValue& name, wxFileName& fileName);
//    bool Read(const wxJSONValue& name, size_t& value);
//    bool Read(const wxJSONValue& name, wxSize& size);
//    bool Read(const wxJSONValue& name, wxPoint& pt);
//    bool Read(const wxJSONValue& name, StringMap& str_map);
//    bool Read(const wxJSONValue& name, SerializedObject* obj);
//    bool Read(const wxJSONValue& name, wxColour& colour);
    bool Read(const wxJSONValue& name, std::vector<TabInfo>& _vTabInfoArr);
//    bool Read(const wxJSONValue& name, std::vector<int>& _vInt);
//    bool Read(const wxJSONValue& name, std::map<wxString, wxString>& strinMap);
//    bool Read(const wxJSONValue& name, std::set<wxString>& s);
//    bool ReadCData(const wxJSONValue& name, wxString& value);
//    bool Read(const wxJSONValue& name, wxFont& font, const wxFont& defaultFont = wxNullFont);

//private:
//    bool WriteSimple(long value, const wxString& typeName, const wxJSONValue& name);
//    bool ReadSimple(long& value, const wxString& typeName, const wxJSONValue& name);
};

#endif // ARCHIVE_JSON_H
