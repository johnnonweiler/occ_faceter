#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "dagmc_faceter.hh"
#include "BRep_Builder.hxx"
#include "BRepTools.hxx"

TEST_CASE("Faceting BREP and writing to MOAB", "[faceter]") {

  MBTool mbtool;
  mbtool.set_tags();

  float facet_tol = 1.e-3;

  const char *input_path = "gluedCompSolid.brep";
  const char *output_path = "test_output.h5m";

  TopoDS_Shape shape;
  BRep_Builder builder;

  BRepTools::Read(shape, input_path, builder);

  REQUIRE(!shape.IsNull());

  sew_and_facet(shape, facet_tol, mbtool);

  mbtool.write_geometry(output_path);
}
