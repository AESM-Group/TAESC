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
    opts.set("chi0_method", "conventional");
    opts.set("anacon_method", "aaa");
    opts.set("anacon_aaa_tolerance", "1e-10");
    opts.set("anacon_ridge_lambda", "1e-6");
    opts.set("anacon_ridge_den_weight", "0.5");
    opts.set("n_bands_exclude_chi0", "2");
    opts.set("n_bands_exclude_sigc", "3");
    opts.set("output_exx_mat_k", "true");
    opts.set("output_exx_mat_r", "on");

    {
        const TaescQsgwControls c = taesc::QsgwTask::default_controls();
        if (c.max_iterations != 10 || c.min_iterations != 1 ||
            c.update_hartree != 1 || c.state_low != 0 || c.mixer == nullptr)
            return 3;
    }

    if (taesc::get_output_level() != TAESC_VERBOSE_WARN) return 2;

    try {
        opts.set("not_a_taesc_option", "1");
    } catch (const std::invalid_argument&) {
        return 0;
    }
    return 1;
}
