// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_CABLEFILECONVERTER_FILEPARSER_H_
#define OTLS_CABLEFILECONVERTER_FILEPARSER_H_

#include <list>

#include "wx/wx.h"

/// \todo What is best to use - wxString vs std::string?

/// \par OVERVIEW
///
/// This a general file parser class that contains some helper functions.
class FileParser {
 public:
  /// \brief Extracts a string.
  /// \param[in] str
  ///   The monolithic string.
  /// \param[in] boundary
  ///   The boundary strings. This defines the start and end extraction
  ///   positions.
  /// \return The extracted string. If two boundary characters are not found,
  ///   an empty string is returned.
  static wxString Extract(const wxString& str, const wxString& boundary);

  /// \brief Gets a formatted string that contains the filepath and line number.
  /// \param[in] filepath
  ///   The filepath.
  /// \param[in] line_number
  ///   The line number.
  /// \return A formatted string containing the filepath and line number.
  static wxString FileAndLineNumber(const wxString& filepath,
                                    const int& line_number);

  /// \brief Separates the string into two separate strings.
  /// \param[in] str
  ///   The monolithic string.
  /// \param[in] separator
  ///   The separator string. This is typically just one character.
  /// \param[out] str_left
  ///   The sub-string to the left of the separator.
  /// \param[out] str_right
  ///   The sub-string to the left of the separator.
  /// \return If the string was separated.
  /// If the separator character occurs more than once in the string, only the
  /// first once is used.
  static bool Separate(const wxString& str,
                       const wxString& separator,
                       wxString& str_left,
                       wxString& str_right);

  /// \brief Generates a list of substrings from the provided string.
  /// \param[in] str
  ///   The monolithic string.
  /// \param[in] separator
  ///   The separator character(s).
  /// \return A list of substrings.
  static std::list<wxString> SubStrings(const wxString& str,
                                        const wxString& separator);
};

#endif  // OTLS_CABLEFILECONVERTER_FILEPARSER_H_
