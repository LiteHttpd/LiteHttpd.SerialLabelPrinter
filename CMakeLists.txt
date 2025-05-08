cmake_minimum_required (VERSION 3.15)

project (LiteHttpd.SerialLabelPrinter VERSION 0.0.1 LANGUAGES CXX C)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release"  CACHE STRING "Choose the type of build." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 11)
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_C_STANDARD_REQUIRED ON)

set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if (MSVC)
    add_compile_definitions ("_CRT_SECURE_NO_WARNINGS")
endif (MSVC)

# Dev Kit
if (NOT TARGET LiteHttpdDev::core)
	add_subdirectory (sdk)
endif ()

# CJsonObject
set (CJSON_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/CJsonObject/cJSON.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/CJsonObject/CJsonObject.cpp"
)
set (CJSON_INC "${CMAKE_CURRENT_SOURCE_DIR}/CJsonObject")

# Source
file (GLOB_RECURSE LABELPRINTER_SRC CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.rc"
)

# Target
add_library (LiteHttpd.SerialLabelPrinter SHARED ${LABELPRINTER_SRC} ${CJSON_SRC})
target_include_directories (LiteHttpd.SerialLabelPrinter PRIVATE ${CJSON_INC})
target_compile_definitions (LiteHttpd.SerialLabelPrinter PRIVATE
	"PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
	"PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
	"PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}"
)
target_link_libraries (LiteHttpd.SerialLabelPrinter PRIVATE LiteHttpdDev::core)

if (NOT MSVC)
	target_compile_options (LiteHttpd.SerialLabelPrinter PRIVATE
		-pthread -fPIC)
endif (NOT MSVC)
if (WIN32)
    target_link_libraries (LiteHttpd.SerialLabelPrinter PRIVATE Dbghelp setupapi)
endif (WIN32)
if (MSVC)
	target_link_options (LiteHttpd.SerialLabelPrinter PRIVATE 
		/SUBSYSTEM:CONSOLE
	)
endif (MSVC)

set_target_properties (LiteHttpd.SerialLabelPrinter PROPERTIES PREFIX "" IMPORT_PREFIX "")

# Output Directory
if (DEFINED OUTPUT_DIR)
	set_target_properties (LiteHttpd.SerialLabelPrinter PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
		ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
	)
	if (MSVC)
		set_target_properties (LiteHttpd.SerialLabelPrinter PROPERTIES
			PDB_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
		)	
	endif ()
endif ()