cmake_minimum_required(VERSION 3.5)

project(engine)

############################################################
# Set compiler flags
############################################################
set(OWN_GXX_FLAGS "-Wall -Wextra -fstack-protector-all -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS} -pg -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")

############################################################
# List all sources
############################################################
set(engine_sources
		engine.cc
        easy_image/easy_image.h easy_image/easy_image.cc
		ini_configuration/ini_configuration.h ini_configuration/ini_configuration.cc
		2D_Lines/line_structs.cpp
		2D_L_System/L_System.h 2D_L_System/L_System.cpp
		L2D_Parser/l_parser.h L2D_Parser/l_parser.cc
		Figure/Figure.h Figure/Figure.cpp
		vector3d/vector3d.h vector3d/vector3d.cc
		3D_L_System/L_System_3D.h 3D_L_System/L_System_3D.cpp)

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable( ${exe_name} ${engine_sources} )
install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/ )
