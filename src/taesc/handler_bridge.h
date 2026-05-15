#pragma once

#include <taesc/taesc_handler.h>

#include "../LibRPA/include/librpa_handler.h"

#include <vector>
#include <memory>

namespace taesc_internal
{

struct TaescHandlerBridge
{
    LibrpaHandler *librpa_handler;
};

extern std::vector<std::shared_ptr<TaescHandlerBridge>> manager_hdl_bridge;

LibrpaHandler* get_librpa_handler(TaescHandler *h);

const LibrpaHandler* get_librpa_handler(const TaescHandler *h);

TaescHandler* push_back_hdl_bridge(MPI_Comm comm);

void destroy_hdl_bridge(TaescHandler* h);

} // namespace taesc_internal
