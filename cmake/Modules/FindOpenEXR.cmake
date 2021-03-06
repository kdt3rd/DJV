# Find the OpenEXR library.
#
# This module defines the following variables:
#
# * OPENEXR_FOUND
# * OPENEXR_INCLUDE_DIRS
# * OPENEXR_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenEXR::IlmImf
#
# This module defines the following interfaces:
#
# * OpenEXR

find_package(ZLIB REQUIRED)
find_package(IlmBase REQUIRED)

find_path(OPENEXR_INCLUDE_DIR
    NAMES ImfIO.h
    PATH_SUFFIXES OpenEXR)
set(OPENEXR_INCLUDE_DIRS
    ${OPENEXR_INCLUDE_DIR}
    ${ILMBASE_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS})

if(OPENEXR_SHARED_LIBS)
    find_library(OPENEXR_LIBRARY NAMES IlmImf)
else()
    find_library(OPENEXR_LIBRARY NAMES IlmImf_s)
endif()
set(OPENEXR_LIBRARIES
    ${OPENEXR_LIBRARY}
    ${ILMBASE_LIBRARIES}
    ${ZLIB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenEXR
    REQUIRED_VARS OPENEXR_INCLUDE_DIR OPENEXR_LIBRARY)
mark_as_advanced(OPENEXR_INCLUDE_DIR OPENEXR_LIBRARY)

if(OPENEXR_FOUND AND NOT TARGET OpenEXR::IlmImf)
    add_library(OpenEXR::IlmImf UNKNOWN IMPORTED)
    set_target_properties(OpenEXR::IlmImf PROPERTIES
        IMPORTED_LOCATION "${OPENEXR_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "IlmBase;ZLIB"
        INTERFACE_INCLUDE_DIRECTORIES "${OPENEXR_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS OPENEXR_FOUND)
endif()
if(OPENEXR_FOUND AND NOT TARGET OpenEXR)
    add_library(OpenEXR INTERFACE)
    target_link_libraries(OpenEXR INTERFACE OpenEXR::IlmImf)
endif()

