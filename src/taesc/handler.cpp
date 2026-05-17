#include <taesc/taesc_handler.h>

#include "handler.h"
#include "handler_bridge.h"

TaescHandler* taesc_create_handler(MPI_Comm comm)
{
    return taesc_internal::push_back_hdl_bridge(comm);
}

TaescHandler* taesc_create_handler_fortran(MPI_Fint *f_comm)
{
    MPI_Comm c_comm = MPI_Comm_f2c(*f_comm);
    return taesc_create_handler(c_comm);
}

void taesc_destroy_handler(TaescHandler *h)
{
    if (!h) {
        return;
    }
    taesc_internal::destroy_hdl_bridge(h);
    delete h;
}
