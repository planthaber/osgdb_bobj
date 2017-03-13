

rock_library(osgdb_bobj
    SOURCES ReaderWriterBOBJ.cpp
    HEADERS ReaderWriterBOBJ.hpp
    DEPS_PKGCONFIG openscenegraph-osgDB openscenegraph-osgUtil)

set_target_properties(osgdb_bobj PROPERTIES PREFIX "")

