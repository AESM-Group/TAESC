if(BUILD_SHARED_LIBS)
  add_library(taesc_f03_lib SHARED)
else()
  add_library(taesc_f03_lib STATIC)
endif()

set_target_properties(taesc_f03_lib PROPERTIES
  OUTPUT_NAME taesc_f03
  VERSION ${CMAKE_PROJECT_VERSION}
  SOVERSION ${CMAKE_PROJECT_VERSION}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/taesc_f03.f90
  ${CMAKE_CURRENT_BINARY_DIR}/taesc_f03.f90
  @ONLY
)

target_sources(taesc_f03_lib
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/taesc_f03.f90
)

set_target_properties(taesc_f03_lib PROPERTIES
  Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(taesc_f03_lib PUBLIC taesc)

target_include_directories(taesc_f03_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

install(TARGETS taesc_f03_lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/taesc_f03.mod
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/taesc)
