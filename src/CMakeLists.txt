add_library(taesc
  taesc/taesc.cpp
  taesc/compute.cpp
  taesc/global.cpp
  taesc/handler_bridge.cpp
  taesc/handler.cpp
  taesc/input.cpp
  taesc/options_bridge.cpp
  taesc/options.cpp
)

if(TAESC_ENABLE_LIBRPA)
  add_compile_definitions("TAESC_ENABLE_LIBRPA")
  set(LIBRPA_USE_LIBRI ON)
  set(LIBRPA_ENABLE_DRIVER ON)
  set(ENABLE_GREENX_CTEST OFF)
  set(LIBRPA_ENABLE_FORTRAN_BIND "${TAESC_ENABLE_FORTRAN_BIND}")
  set(LIBRPA_ENABLE_TEST "${TAESC_ENABLE_TEST}")
  set(LIBRPA_FORTRAN_DP "${TAESC_FORTRAN_DP}")
  set(LIBCOMM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/LibComm/include")
  set(LIBRI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/LibRI/include")
  set(CEREAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/cereal-d81e2f7/include")
  # Keep downloaded third-party archives outside the build directory and the
  # LibRPA checkout, so that they survive both being recreated.
  set(LIBRPA_THIRDPARTY_ARCHIVE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/archives"
      CACHE PATH "Directory of LibRPA third-party source archives")
  add_subdirectory(LibRPA)

  target_include_directories(taesc
    PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibRPA/include>
  )

  target_link_libraries(taesc PRIVATE rpa_lib)
endif()

# if(TARGET MPI::MPI_CXX)
#   target_link_libraries(taesc PUBLIC MPI::MPI_CXX)
# endif()
