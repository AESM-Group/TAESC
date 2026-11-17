add_library(taesc
  taesc/taesc.cpp
  taesc/compute.cpp
  taesc/global.cpp
  taesc/handler_bridge.cpp
  taesc/handler.cpp
  taesc/input.cpp
  taesc/options_bridge.cpp
  taesc/options.cpp
  taesc/qsgw.cpp
)

if(TAESC_ENABLE_LIBRPA)
  add_compile_definitions("TAESC_ENABLE_LIBRPA")
  set(LIBRPA_USE_LIBRI ON)
  set(LIBRPA_ENABLE_DRIVER ON)
  set(ENABLE_GREENX_CTEST OFF)
  set(LIBRPA_ENABLE_FORTRAN_BIND "${TAESC_ENABLE_FORTRAN_BIND}")
  set(LIBRPA_ENABLE_TEST "${TAESC_ENABLE_TEST}")
  set(LIBRPA_FORTRAN_DP "${TAESC_FORTRAN_DP}")
  # TAESC owns LibRI, LibComm (submodules) and cereal (downloaded by
  # cmake/TAESCCereal.cmake). Supply them as parent targets, so
  # that LibRPA compiles against these trees in place: no copy, no patch overlay.
  # LibRPA then selects MPI_THREAD_MULTIPLE automatically when
  # LIBRPA_MPI_THREAD_LEVEL is empty. Set it explicitly in HPC build scripts.
  function(taesc_header_target name dir)
    if(NOT EXISTS "${dir}")
      message(FATAL_ERROR "${name}: ${dir} not found. Run `git submodule update --init --recursive`.")
    endif()
    if(NOT TARGET ${name})
      add_library(${name} INTERFACE IMPORTED)
      set_target_properties(${name} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${dir}")
    endif()
  endfunction()
  taesc_header_target(LibRI::LibRI "${CMAKE_CURRENT_SOURCE_DIR}/LibRI/include")
  taesc_header_target(LibComm::LibComm "${CMAKE_CURRENT_SOURCE_DIR}/LibComm/include")
  include("${CMAKE_SOURCE_DIR}/cmake/TAESCCereal.cmake")

  # LibDDLA is only built for GPU backends. Use the submodule unless the user
  # supplies a prebuilt installation (LIBDDLA_PATH).
  if((LIBRPA_USE_HIP OR LIBRPA_USE_CUDA) AND NOT LIBDDLA_PATH)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LibDDLA/include/ddla/ddla.h")
      message(FATAL_ERROR "LibDDLA submodule is empty. Run `git submodule update --init src/LibDDLA`.")
    endif()
    set(LIBDDLA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/LibDDLA" CACHE PATH "LibDDLA source tree")
  endif()

  # greenX and ELPA are managed by LibRPA: it downloads the pinned archives,
  # checks their SHA-256, applies its patches and builds them. cereal uses the
  # same archive cache, downloaded by cmake/TAESCCereal.cmake. The TAESC options
  # are authoritative: normal variables override the cache entries that LibRPA
  # creates, so changing them on a later configure takes effect. The archive
  # cache is not tracked. For offline builds, fill it with
  # scripts/prepare_thirdparty.sh and configure with
  # -DTAESC_THIRDPARTY_DOWNLOAD=OFF.
  set(LIBRPA_THIRDPARTY_DOWNLOAD "${TAESC_THIRDPARTY_DOWNLOAD}")
  set(LIBRPA_THIRDPARTY_ARCHIVE_DIR "${TAESC_THIRDPARTY_ARCHIVE_DIR}")
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
