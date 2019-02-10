// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "cable_file_xml_handler.h"

#include "appcommon/xml/cable_xml_handler.h"

wxXmlNode* CableFileXmlHandler::CreateNode(
    const Cable& cable,
    const std::string& name,
    const units::UnitSystem& system_units,
    const units::UnitStyle& style_units) {
  // variables used to create XML node
  wxXmlNode* node_root = nullptr;
  wxXmlNode* node_element = nullptr;

  // creates a node for the cable file root
  node_root = new wxXmlNode(wxXML_ELEMENT_NODE, "cable_file");
  node_root->AddAttribute("version", "0");

  if (system_units == units::UnitSystem::kImperial) {
    node_root->AddAttribute("units", "Imperial");
  } else if (system_units == units::UnitSystem::kMetric) {
    node_root->AddAttribute("units", "Metric");
  }

  if (name != "") {
    node_root->AddAttribute("name", name);
  }

  // creates cable node and adds to root node
  node_element = CableXmlHandler::CreateNode(
      cable, name, system_units, style_units);
  node_root->AddChild(node_element);

  // returns node
  return node_root;
}

bool CableFileXmlHandler::ParseNode(
    const wxXmlNode* root,
    const std::string& filepath,
    const units::UnitSystem& units,
    const bool& convert,
    Cable& cable) {
  // parsing isn't needed for this app
}
