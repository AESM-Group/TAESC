#pragma once

#include <taesc/taesc_options.h>
#include "../LibRPA/include/librpa_options.h"

#include <vector>
#include <memory>

namespace taesc_internal
{

// Working as a bridge between TAESC options and runtime options of underlying components
struct TaescOptionsBridge
{
    LibrpaOptions librpa_options;
};

extern std::vector<std::shared_ptr<TaescOptionsBridge>> manager_opts_bridge;

LibrpaOptions* get_librpa_options(TaescOptions *opts);

const LibrpaOptions* get_librpa_options(const TaescOptions *opts);

TaescOptions* push_back_opts_bridge();

void destroy_opts_bridge(TaescOptions* opts);

} // namespace taesc_internal
