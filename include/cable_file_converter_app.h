// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_CABLEFILECONVERTER_CABLEFILECONVERTERAPP_H_
#define OTLS_CABLEFILECONVERTER_CABLEFILECONVERTERAPP_H_

#include "models/base/units.h"
#include "wx/cmdline.h"
#include "wx/wx.h"

/// \par OVERVIEW
///
/// This is the CableFileConverter application class.
///
/// This application converts an input file to an OTLS cable file.
class CableFileConverterApp : public wxAppConsole {
 public:
  /// \brief Constructor.
  CableFileConverterApp();

  /// \brief Destructor.
  ~CableFileConverterApp();

  /// \brief Transfers the parsed command line switches, options, and parameters
  ///   to application settings.
  /// \param[in] parser
  ///   The command line parser, which is provided by wxWidgets.
  /// \return The success of the command line switch, option, and parameter
  ///   transfer.
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

  /// \brief Initializes the application.
  /// \return The success of the application initialization.
  virtual bool OnInit();

  /// \brief Initializes the command line parser settings.
  /// \param[in] parser
  ///   The command line parser, which is provided by wxWidgets.
  virtual void OnInitCmdLine(wxCmdLineParser& parser);

  /// \brief Runs all of the application logic and methods.
  /// \return 0
  virtual int OnRun();

 private:
  /// \var filepath_input_
  ///   The input filepath. This is specified as a command line parameter.
  wxString filepath_input_;

  /// \var filepath_output_
  ///   The output filepath. This is specified as a command line parameter.
  wxString filepath_output_;

  /// \var strain_percent_polynomial_limits_
  ///   The percent strain value for the polynomial limits.
  double strain_percent_polynomial_limits_;

  /// \var units_
  ///   The unit system.
  units::UnitSystem units_;
};

/// This is an array of command line options.
static const wxCmdLineEntryDesc cmd_line_desc [] = {
  {wxCMD_LINE_SWITCH, nullptr, "help", "show this help message",
      wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
  {wxCMD_LINE_SWITCH, "v", "verbose", "enable verbose logging"},

  {wxCMD_LINE_OPTION, "s", "strain", "percent strain for polynomial limits",
      wxCMD_LINE_VAL_DOUBLE, wxCMD_LINE_PARAM_OPTIONAL},
  {wxCMD_LINE_OPTION, "u", "units", "unit system for generated file - "
                                    "'imperial' (default) or 'metric'",
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},

  {wxCMD_LINE_PARAM, nullptr, nullptr, "input file",
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
  {wxCMD_LINE_PARAM, nullptr, nullptr, "output file",
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},

  {wxCMD_LINE_NONE}
};

DECLARE_APP(CableFileConverterApp)

#endif  // OTLS_CABLEFILECONVERTER_CABLEFILECONVERTERAPP_H_
