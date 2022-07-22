# header only for now, but I really want to experiment with c++20's modules
add_library(lust INTERFACE expint.hpp
  )

target_link_libraries(lust INTERFACE FunC::FunC Boost::boost)
target_include_directories(lust INTERFACE
    $<INSTALL_INTERFACE:.>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_options(lust INTERFACE -Wall -Wextra -pedantic -Werror)

include(GNUInstallDirs) # defines CMAKE_INSTALL_XXX
install(TARGETS lust
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lust)

