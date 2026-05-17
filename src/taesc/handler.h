#pragma once
#include <taesc/taesc_handler.h>

#ifdef __cplusplus
extern "C" {
#endif

// Only for internal usage in the Fortran binding
TaescHandler* taesc_create_handler_fortran(MPI_Fint *f_comm);

#ifdef __cplusplus
}
#endif


