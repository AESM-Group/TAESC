#include "handler_bridge.h"
#include "librpa_handler.h"
#include "taesc/taesc_handler.h"

namespace taesc_internal
{

// manager[0] is a reserved sentinel (always nullptr); real instances start at 1.
std::vector<std::shared_ptr<TaescHandlerBridge>> manager_hdl_bridge{nullptr};

LibrpaHandler* get_librpa_options(TaescHandler *h)
{
    if (h == nullptr) return nullptr;
    return manager_hdl_bridge.at(h->instance_id_)->librpa_handler;
}

const LibrpaHandler* get_librpa_options(const TaescHandler *h)
{
    if (h == nullptr) return nullptr;
    return manager_hdl_bridge.at(h->instance_id_)->librpa_handler;
}

TaescHandler* push_back_hdl_bridge(MPI_Comm comm)
{
    // create a new instance and append it to the manager
    int instance_id = manager_hdl_bridge.size();
    auto p = std::make_shared<TaescHandlerBridge>();

    // initialize hanlders of underlying working components
    p->librpa_handler = librpa_create_handler(comm);

    manager_hdl_bridge.emplace_back(p);
    // initialize a binding handler
    TaescHandler* h = new TaescHandler{instance_id};
    return h;
}

void destroy_hdl_bridge(TaescHandler* h)
{
    if (!h) return;
    // destroy the instance
    if (h->instance_id_ > 0)
    {
        const auto id = h->instance_id_;
        const int total_size = manager_hdl_bridge.size();
        // Invalid handler that was manually created with hand-picked id,
        // either oversubscription
        if (id >= total_size) return;
        auto p = manager_hdl_bridge[id];
        librpa_destroy_handler(get_librpa_handler(h));
        // or pointed to an already released instance
        p.reset();
        // free the instance and point it to null pointer
        manager_hdl_bridge[id] = nullptr;
    }
}

}
