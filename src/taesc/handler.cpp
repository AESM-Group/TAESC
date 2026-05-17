#include <taesc/taesc_handler.h>

#include "handler_bridge.h"

TaescHandler* taesc_create_handler(MPI_Comm comm)
{
    return taesc_internal::push_back_hdl_bridge(comm);
}

void taesc_destroy_handler(TaescHandler *h)
{
    if (!h) {
        return;
    }
    taesc_internal::destroy_hdl_bridge(h);
    delete h;
}
