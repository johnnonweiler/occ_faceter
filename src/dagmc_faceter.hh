#ifndef DAGMC_FACETER_HH
#define DAGMC_FACETER_HH

#include <string>

#include "TopoDS_Shape.hxx"
#include "MBTool.hpp"
#include "read_metadata.hh"

void sew_and_facet(TopoDS_Shape &shape, float facet_tol, MBTool &mbtool, MaterialsMap &mat_map);
void dagmc_faceter(std::string brep_file, float facet_tol, std::string h5m_file);

#endif // DAGMC_FACETER_HH
