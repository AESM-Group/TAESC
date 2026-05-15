#include "options_bridge.h"

namespace taesc_internal
{

// manager[0] is a reserved sentinel (always nullptr); real instances start at 1.
std::vector<std::shared_ptr<TaescOptionsBridge>> manager_opts_bridge{nullptr};

LibrpaOptions* get_librpa_options(TaescOptions *opts)
{
    if (opts == nullptr) return nullptr;
    return &(manager_opts_bridge.at(opts->instance_id_)->librpa_options);
}

const LibrpaOptions* get_librpa_options(const TaescOptions *opts)
{
    if (opts == nullptr) return nullptr;
    return &(manager_opts_bridge.at(opts->instance_id_)->librpa_options);
}

TaescOptions* push_back_opts_bridge()
{
    // create a new instance and append it to the manager
    int instance_id = manager_opts_bridge.size();
    auto p = std::make_shared<TaescOptionsBridge>();

    // initialize values of options
    librpa_init_options(&p->librpa_options);

    manager_opts_bridge.emplace_back(p);
    // initialize a binding handler
    TaescOptions* h = new TaescOptions{instance_id};
    return h;
}

void destroy_opts_bridge(TaescOptions* opts)
{
    if (!opts) return;
    // destroy the instance
    if (opts->instance_id_ > 0)
    {
        const auto id = opts->instance_id_;
        const int total_size = manager_opts_bridge.size();
        // Invalid handler that was manually created with hand-picked id,
        // either oversubscription
        if (id >= total_size) return;
        auto p = manager_opts_bridge[id];
        // or pointed to an already released instance
        p.reset();
        // free the instance and point it to null pointer
        manager_opts_bridge[id] = nullptr;
    }
}

}
