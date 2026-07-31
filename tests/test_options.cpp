#include <taesc/taesc.hpp>

#include <stdexcept>

int main()
{
    taesc::Options opts;
    opts.set("restart_from_dir", "restart");
    opts.set("output_level", "warn");
    opts.set("tfgrids_type", "split-GL");
    opts.set("anacon_tfgrids_type", "minimax");
    opts.set("libri_chi0_collect_max_bytes", "1048576");
    opts.set("qpe_solver_thres", "1e-7");
    opts.set("use_qpe_adaptive_damp", "true");
    opts.set("rpa_headwing_mode", "head_only");

    if (taesc::get_output_level() != TAESC_VERBOSE_WARN) return 2;

    try {
        opts.set("not_a_taesc_option", "1");
    } catch (const std::invalid_argument&) {
        return 0;
    }
    return 1;
}
