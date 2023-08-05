add_library(lust SHARED lookuptables.cpp)

set_target_properties(lust PROPERTIES PUBLIC_HEADER "\
exponents.hpp;lookuptablejsons.hpp;json.hpp;lust.hpp;lookuptables.hpp;config.hpp\
")

target_link_libraries(lust PUBLIC FunC::FunC)

target_include_directories(lust PUBLIC
    $<INSTALL_INTERFACE:.>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_options(lust PRIVATE -Wall -Wextra -pedantic) # -Werror)

include(GNUInstallDirs) # defines CMAKE_INSTALL_XXX
install(TARGETS lust
  EXPORT lust-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LUST
)
set_target_properties(lust PROPERTIES EXPORT_NAME LUST)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/LUST)
install(EXPORT lust-targets
  FILE
    LUSTTargets.cmake
  NAMESPACE
    LUST::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/lust-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/lust-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/lust-config.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/lust-config.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

# Exporting from the build tree
#configure_file(${CMAKE_SOURCE_DIR}/cmake/FindQuadmath.cmake
#  ${CMAKE_CURRENT_BINARY_DIR}/FindQuadmath.cmake
#  COPYONLY)

export(EXPORT lust-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/LUSTTargets.cmake
  NAMESPACE LUST::)

# Register package in the User Package Registry (TODO is that helpful?)
export(PACKAGE LUST)

