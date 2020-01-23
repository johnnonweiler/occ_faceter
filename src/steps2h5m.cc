#include <iostream>
#include <fstream>

#include "STEPControl_Reader.hxx"
#include "dagmc_faceter.hh"
#include "read_metadata.hh"
#include "MBTool.hpp"
#include "UniqueId/json.hpp"

using json = nlohmann::json;

TopoDS_Shape step_to_one_brep(std::string step_file) {
  STEPControl_Reader step;
  step.ReadFile(step_file.c_str());
  step.PrintCheckLoad(false, IFSelect_ListByItem);
  step.ClearShapes();
  int count = step.NbRootsForTransfer();
  if (count > 1) {
    std::cerr << "Error: More than one root in STEP file." << std::endl;
  } else if (count < 1) {
    std::cerr << "Error: Less than one root in STEP file." << std::endl;
    return TopoDS_Shape();
  }

  bool ok = step.TransferRoot(1);
  step.PrintCheckTransfer(false, IFSelect_CountByItem);
  if (!ok) {
    return TopoDS_Shape();
  }
  return step.Shape(1);
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: steps2h5m JSON_FILE TOLERANCE H5M_FILE" << std::endl;
    return 1;
  }

  std::ifstream json_stream(argv[1]);
  float facet_tol = std::stof(argv[2]);
  std::string h5m_file(argv[3]);

  json j = json::parse(json_stream);

  MBTool mbtool;
  mbtool.set_tags();

  for (const auto &pair : j) {
    std::string step_file = pair[0];
    std::string material = pair[1];

    TopoDS_Shape shape = step_to_one_brep(step_file);
    MaterialsMap emptyMap;
    sew_and_facet(shape, facet_tol, mbtool, emptyMap, material);
  }

  mbtool.write_geometry(h5m_file.c_str());

  return 0;
}
