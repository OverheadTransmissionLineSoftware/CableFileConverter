// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "file_parser.h"

wxString FileParser::Extract(const wxString& str, const wxString& boundary) {
  // finds start position
  int pos_start = str.find(boundary) + 1;
  if (pos_start == -1) {
    return wxString();
  }

  // finds end position
  int pos_end = str.find(boundary, pos_start);
  if (pos_end == -1) {
    return wxString();
  }

  // extracts
  const int length = pos_end - pos_start;
  return str.substr(pos_start, length);
}

wxString FileParser::FileAndLineNumber(const wxString& filepath,
                                       const int& line_number) {
  wxString str_num;
  str_num << line_number;

  return filepath + ":" + str_num + "  --  ";
}

bool FileParser::Separate(const wxString& str, const wxString& separator,
                          wxString& str_left, wxString& str_right) {
  // clears output strings
  str_left.clear();
  str_right.clear();

  // finds separator position
  int pos = str.find(separator);
  if (pos == -1) {
    return false;
  }

  // creates left and right strings
  const int kSize = str.size();
  str_left = str.substr(0, pos);
  str_right = str.substr(pos + 1, kSize);

  return true;
}

std::list<wxString> FileParser::SubStrings(const wxString& str,
                                           const wxString& separator) {
  // declares list of strings
  std::list<wxString> sub_strs;

  // searches string for separators and creates substrings
  // adds to list
  int pos_before = 0;
  int pos = str.Find(separator);
  while (pos != -1) {
    int len = pos - pos_before;
    wxString sub_str = str.substr(pos_before, len);
    sub_strs.push_back(sub_str);

    pos_before = pos + 1;
    pos = str.find(separator, pos_before);
  }

  // adds last string to list
  wxString sub_str = str.substr(pos_before);
  sub_strs.push_back(sub_str);

  return sub_strs;
}
