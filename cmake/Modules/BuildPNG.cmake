include(ExternalProject)

ExternalProject_Add(
    PNG_EXTERNAL
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/PNG
    DEPENDS ZLIB
    URL "http://prdownloads.sourceforge.net/libpng/libpng-1.6.37.tar.gz?download"
    CMAKE_ARGS
        -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_INSTALL_LIBDIR=lib
        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DZLIB_SHARED_LIBS=${ZLIB_SHARED_LIBS}
        -DPNG_SHARED=${PNG_SHARED_LIBS})

set(PNG_FOUND TRUE)
set(PNG_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    if(WIN32)
    elseif(APPLE)
    else()
        set(PNG_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libpng16d.a)
    endif()
else()
    if(WIN32)
    elseif(APPLE)
    else()
        set(PNG_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libpng16.a)
    endif()
endif()
set(PNG_LIBRARIES ${PNG_LIBRARY} ${ZLIB_LIBRARIES})

if(PNG_FOUND AND NOT TARGET PNG::PNG)
    add_library(PNG::PNG UNKNOWN IMPORTED)
    add_dependencies(PNG::PNG PNG_EXTERNAL)
    set_target_properties(PNG::PNG PROPERTIES
        IMPORTED_LOCATION "${PNG_LIBRARY}"
        INTERFACE_LINK_LIBRARIES ZLIB 
        INTERFACE_INCLUDE_DIRECTORIES "${PNG_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS PNG_FOUND)
endif()
if(PNG_FOUND AND NOT TARGET PNG)
    add_library(PNG INTERFACE)
    target_link_libraries(PNG INTERFACE PNG::PNG)
endif()

