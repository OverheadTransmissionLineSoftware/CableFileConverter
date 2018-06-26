// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "cable_file_converter_app.h"

#include "appcommon/units/cable_unit_converter.h"
#include "appcommon/xml/cable_xml_handler.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "wx/xml/xml.h"

#include "cable_polynomial_searcher.h"

/// \brief Parses a cable file.
/// \param[in] filepath
///   The relative or absolute filepath to open and parse.
/// \param[out] units
///   The unit system that is populated.
/// \param[out] cable
///   The cable that is populated.
/// \return The status of the file parsing. A true return flag means that no
///   errors were encountered.
/// This function is declared here (globally) so a custom parser can be added
/// onto the project in a modular way. The source code in this class does not
/// need modified. Instead, create an external file with a matching function
/// definition. Add the external file to the build process and the linker will
/// make the connection.
extern bool ParseCableFile(const wxString& filepath, units::UnitSystem& units,
                           Cable& cable);


IMPLEMENT_APP(CableFileConverterApp)

CableFileConverterApp::CableFileConverterApp() {
}

CableFileConverterApp::~CableFileConverterApp() {
}

bool CableFileConverterApp::OnCmdLineParsed(wxCmdLineParser& parser) {
  // captures the command line switches
  if (parser.Found("verbose")) {
    wxLog::SetLogLevel(wxLOG_Info);
    wxLog::SetVerbose(true);
  }

  // captures the command line options
  wxString option_str;
  double option_num;
  if (parser.Found("strain", &option_num) == true) {
    strain_percent_polynomial_limits_ = option_num;
  } else if (parser.Found("units", &option_str) == true) {
    if (option_str == "imperial") {
      units_ = units::UnitSystem::kImperial;
    } else if (option_str == "metric") {
      units_ = units::UnitSystem::kMetric;
    } else {
      wxLogError("Invalid units option. Exiting.");
      return false;
    }
  }

  // captures the command line parameters
  if (parser.GetParamCount() == 2) {
    filepath_input_ = parser.GetParam(0);
    filepath_output_ = parser.GetParam(1);
  } else {
    wxLogError("Invalid number of parameters. Exiting.");
    return false;
  }

  return true;
}

bool CableFileConverterApp::OnInit() {
  // initializes variables
  filepath_input_ = "";
  filepath_output_ = "";
  strain_percent_polynomial_limits_ = -1;
  units_ = units::UnitSystem::kImperial;

  // redirects log to a file in the executable directory
  wxFileName filepath(wxStandardPaths::Get().GetExecutablePath());
  filepath.SetName("CableFileConverter");
  filepath.SetExt("log");
  wxString str_filepath = filepath.GetFullPath();

  FILE* file_log;
  file_log = fopen(str_filepath.c_str(), "a");
  wxLogStderr* log = new wxLogStderr(file_log);
  wxLog::SetActiveTarget(log);

  // initializes command line
  if (wxAppConsole::OnInit() == false) {
    return false;
  }

  // sets application name
  SetAppDisplayName("CableFileConverter");
  SetAppName("CableFileConverter");

  return true;
}

void CableFileConverterApp::OnInitCmdLine(wxCmdLineParser& parser) {
  // sets command line style and parameters
  parser.EnableLongOptions();
  parser.SetDesc(cmd_line_desc);
  parser.SetSwitchChars("--");
}

int CableFileConverterApp::OnRun() {
  wxString message;

  // validates input file
  if (wxFileName::Exists(filepath_input_) == false) {
    wxLogError("Invalid input filepath. Exiting.");
    return 0;
  }

  // validates output file directory
  wxFileName filename(filepath_output_);
  if (wxFileName::DirExists(filename.GetPath()) == false) {
    wxLogError("Invalid output directory. Exiting.");
    return 0;
  }

  if (wxFileName::IsDirWritable(filename.GetPath()) == false) {
    wxLogError("Insufficient permissions for output directory: "
               + filename.GetPath() + ". Exiting.");
    return 0;
  }

  // initializes data to be filled when parsing occurs
  Cable cable;
  units::UnitSystem units;

  // parses input file
  wxLogVerbose("Parsing input file: " + filepath_input_);
  if (ParseCableFile(filepath_input_, units, cable) == false) {
    wxLogError("Parsing errors were encountered. Exiting.");
    return 0;
  }

  // converts units if necessary
  if (units_ != units) {
    // converts from different to consistent unit style
    CableUnitConverter::ConvertUnitStyle(units, units::UnitStyle::kDifferent,
                                         units::UnitStyle::kConsistent, cable);

    // converts from file to app unit system
    CableUnitConverter::ConvertUnitSystem(units, units_, cable);

    // converts from consistent to different unit style
    CableUnitConverter::ConvertUnitStyle(units, units::UnitStyle::kConsistent,
                                         units::UnitStyle::kDifferent, cable);
  }

  // searches for the polynomial limits
  wxLogVerbose("Solving for polynomial limits.");
  if (CablePolynomialSearcher::SolveLimits(strain_percent_polynomial_limits_,
                                           cable) == false) {
    wxLogError("Limit searching errors were encountered. Exiting.");
    return 0;
  }

  // generates output file
  wxLogVerbose("Saving output file: " + filepath_output_);
  wxXmlNode* root = CableXmlHandler::CreateNode(cable, "",
                                                units::UnitSystem::kImperial);
  root->AddAttribute("units", "Imperial");

  wxXmlDocument doc;
  doc.SetRoot(root);
  doc.Save(filepath_output_, 2);

  // exits application
  return 0;
}
