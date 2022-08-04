
#          Copyright Ted Middleton 2022.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.0 )
project( mainframe )

enable_testing()

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

if (MSVC)
    # C4456: declaration of *** hides previous local declaration
    # C4459: declaration of *** hides global declaration
    #  C445X aren't useful checks. Scoping is a *good* thing.
    # C4702: unreachable code
    #  C4702 is broken for "if constexpr ()" in msvc 17.1.2
    add_compile_options(/W3 /WX /wd4456 /wd4459 /wd4702)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set( version 0.1.0 )

add_library( csvparse STATIC
   csvparse/csvparse.hpp 
   csvparse/csvparse.cpp 
   )

add_subdirectory( tests )

target_include_directories( csvparse
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

install( TARGETS csvparse
         EXPORT csvparse-targets
    )

install(
    FILES 
        csvparse/csvparse.hpp 
    DESTINATION include/csvparse 
    )

install( EXPORT csvparse-targets
         DESTINATION lib/cmake/csvparse
    )

set_property( TARGET csvparse PROPERTY VERSION ${version} )
set_property( TARGET csvparse PROPERTY SOVERSION 0 )
set_property( TARGET csvparse PROPERTY 
    INTERFACE_csvparse_MAJOR_VERSION 0 )
set_property( TARGET csvparse PROPERTY 
    COMPATIBLE_INTERFACE_STRING csvparse_MAJOR_VERSION )

export(EXPORT csvparse-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/csvparse-targets.cmake"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/csvparse-config-version.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
    )

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/csvparse-config.cmake"
    INSTALL_DESTINATION lib/cmake/csvparse
    )

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/csvparse-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/csvparse-config-version.cmake"
    DESTINATION lib/cmake/csvparse
    )

