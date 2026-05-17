#pragma once
/**
 * @file taesc_handler.h
 * @brief TAESC handler facade.
 */

#include <mpi.h>

typedef struct
{
    /** Internal instance ID used by the manager. Do not modify manually. */
    const int instance_id_;
} TaescHandler;


#ifdef __cplusplus
extern "C" {
#endif

TaescHandler* taesc_create_handler(MPI_Comm comm);

void taesc_destroy_handler(TaescHandler *h);

#ifdef __cplusplus
}
#endif
