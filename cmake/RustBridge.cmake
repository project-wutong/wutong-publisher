# SPDX-License-Identifier: AGPL-3.0-only
# Build a cxx-based Rust staticlib with Cargo and expose it as an imported CMake target.
#   wutong_rust_bridge(TARGET <name> PACKAGE <cargo-package>)
find_program(CARGO_EXECUTABLE cargo REQUIRED)
function(wutong_rust_bridge)
  cmake_parse_arguments(A "" "TARGET;PACKAGE" "" ${ARGN})
  string(REPLACE "-" "_" _lib "${A_PACKAGE}")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(_profile debug)
    set(_flags "")
  else()
    set(_profile release)
    set(_flags --release)
  endif()
  set(_tdir "${CMAKE_BINARY_DIR}/cargo")
  if(WIN32)
    set(_a "${_tdir}/${_profile}/${_lib}.lib")
  else()
    set(_a "${_tdir}/${_profile}/lib${_lib}.a")
  endif()
  add_custom_target(${A_TARGET}_cargo
    COMMAND ${CARGO_EXECUTABLE} build ${_flags} --locked -p ${A_PACKAGE} --target-dir "${_tdir}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    BYPRODUCTS "${_a}"
    COMMENT "cargo build -p ${A_PACKAGE}"
    USES_TERMINAL)
  file(MAKE_DIRECTORY "${_tdir}/cxxbridge")
  add_library(${A_TARGET} STATIC IMPORTED GLOBAL)
  set_target_properties(${A_TARGET} PROPERTIES
    IMPORTED_LOCATION "${_a}"
    INTERFACE_INCLUDE_DIRECTORIES "${_tdir}/cxxbridge")
  add_dependencies(${A_TARGET} ${A_TARGET}_cargo)
  if(UNIX AND NOT APPLE)
    set_property(TARGET ${A_TARGET} APPEND PROPERTY INTERFACE_LINK_LIBRARIES pthread dl m)
  elseif(APPLE)
    set_property(TARGET ${A_TARGET} APPEND PROPERTY INTERFACE_LINK_LIBRARIES "-framework CoreFoundation" "-framework Security")
  elseif(WIN32)
    set_property(TARGET ${A_TARGET} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ws2_32 userenv bcrypt ntdll advapi32)
  endif()
endfunction()
