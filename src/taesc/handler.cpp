#include <taesc/taesc_handler.h>

#include "options_bridge.h"

extern "C" {

TaescHandler* taesc_create_handler(MPI_Comm comm)
{
    return new TaescHandler{librpa_create_handler(comm)};
}

void taesc_destroy_handler(TaescHandler *h)
{
    if (!h) {
        return;
    }
    librpa_destroy_handler(h->librpa_handler);
    delete h;
}

}
