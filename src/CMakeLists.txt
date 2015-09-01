rock_library(plugin_bobj
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(plugin_bobj_bin Main.cpp
    DEPS plugin_bobj)

