#include <taesc/taesc_options.h>
#include "../LibRPA/include/librpa_global.h"
#include "../LibRPA/include/librpa_options.h"

#include "options_bridge.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <unordered_map>

TaescOptions* taesc_create_options(void)
{
    return taesc_internal::push_back_opts_bridge();
}

void taesc_destroy_options(TaescOptions *opts)
{
    if (!opts) return;
    taesc_internal::destroy_opts_bridge(opts);
    delete opts;
}

namespace taesc_internal
{

#define mem_pair(type, name) {#name, &type::name}

static LibrpaSwitch parse_librpa_switch(const std::string& s)
{

    if (s == "true" || s == "on" || s == "1")
        return LIBRPA_SWITCH_ON;
    if (s == "false" || s == "off" || s == "0")
        return LIBRPA_SWITCH_OFF;
    throw std::invalid_argument("Invalid switch value: " + s);
}

static LibrpaTimeFreqGrid parse_librpa_tfgrid(const std::string& s)
{
    static const std::unordered_map<std::string, LibrpaTimeFreqGrid> kv{
        {"unset", LIBRPA_TFGRID_UNSET},
        {"GL", LIBRPA_TFGRID_GAUSS_LEGENDRE},
        {"split-GL", LIBRPA_TFGRID_SPLIT_GAUSS_LEGENDRE},
        {"GC-I", LIBRPA_TFGRID_GAUSS_CHEBYSHEV_I},
        {"GL-II", LIBRPA_TFGRID_GAUSS_CHEBYSHEV_II},
        {"GC-II", LIBRPA_TFGRID_GAUSS_CHEBYSHEV_II},
        {"minimax", LIBRPA_TFGRID_MINIMAX},
        {"evenspaced", LIBRPA_TFGRID_EVEN_SPACED},
        {"evenspaced_tf", LIBRPA_TFGRID_EVEN_SPACED_TF},
    };
    return kv.at(s);
}

} // namespace taesc_internal

void taesc_set_option(TaescOptions *opts, const char *key, const char *value)
{
    using std::string;
    using std::unordered_map;
    using taesc_internal::get_librpa_options;
    using taesc_internal::parse_librpa_switch;
    using taesc_internal::parse_librpa_tfgrid;

    const string key_s(key), val_s(value);

    auto p_opts_librpa = get_librpa_options(opts);

    // Options that should be aligned across different components
    if (key_s == "output_dir")
    {
        librpa_set_output_dir(p_opts_librpa, value);
        return;
    }
    if (key_s == "restart_from_dir")
    {
        librpa_set_restart_from_dir(p_opts_librpa, value);
        return;
    }
    if (key_s == "output_level")
    {
        static const unordered_map<string, LibrpaVerbose> kv{
            {"debug", LIBRPA_VERBOSE_DEBUG},
            {"info", LIBRPA_VERBOSE_INFO},
            {"warn", LIBRPA_VERBOSE_WARN},
            {"warning", LIBRPA_VERBOSE_WARN},
            {"critical", LIBRPA_VERBOSE_CRITICAL},
            {"silent", LIBRPA_VERBOSE_SILENT},
        };
        librpa_set_output_level(kv.at(val_s));
        return;
    }
    if (key_s == "use_kpara_scf_eigvec")
    {
        p_opts_librpa->use_kpara_scf_eigvec = parse_librpa_switch(val_s);
        return;
    }

    // LibRPA specific
    if (key_s == "parallel_routing")
    {
        static const unordered_map<string, LibrpaParallelRouting> kv
        {
            {"auto", LIBRPA_ROUTING_AUTO},
            {"rtau", LIBRPA_ROUTING_RTAU},
            {"atompair", LIBRPA_ROUTING_ATOMPAIR},
            {"libri", LIBRPA_ROUTING_LIBRI},
        };
        p_opts_librpa->parallel_routing = kv.at(val_s);
        return;
    }
    if (key_s == "tfgrids_type")
    {
        p_opts_librpa->tfgrids_type = parse_librpa_tfgrid(val_s);
        return;
    }
    if (key_s == "anacon_tfgrids_type")
    {
        p_opts_librpa->anacon_tfgrids_type = parse_librpa_tfgrid(val_s);
        return;
    }
    if (key_s == "rpa_headwing_mode")
    {
        if (val_s != "qavg" && val_s != "head_only")
            throw std::invalid_argument(
                "rpa_headwing_mode must be qavg or head_only");
        std::strncpy(p_opts_librpa->rpa_headwing_mode, value,
                     LIBRPA_MAX_STRLEN - 1);
        p_opts_librpa->rpa_headwing_mode[LIBRPA_MAX_STRLEN - 1] = '\0';
        return;
    }

    static const unordered_map<string, int LibrpaOptions::*> map_librpa_opts_int{
        mem_pair(LibrpaOptions, nfreq),
        mem_pair(LibrpaOptions, n_bands_chi0),
        mem_pair(LibrpaOptions, n_bands_sigc),
        mem_pair(LibrpaOptions, option_bvk_remap),
        mem_pair(LibrpaOptions, libri_chi0_collect_s0_chunk),
        mem_pair(LibrpaOptions, n_params_anacon),
        mem_pair(LibrpaOptions, n_params_anacon_resample),
        mem_pair(LibrpaOptions, anacon_nfreq),
        mem_pair(LibrpaOptions, option_qpe_solver),
        mem_pair(LibrpaOptions, qpe_solver_n_iter_max),
        mem_pair(LibrpaOptions, istate_ref_hedin_shift),
        mem_pair(LibrpaOptions, option_dielect_func),
        mem_pair(LibrpaOptions, rpa_headwing_body_start),
        mem_pair(LibrpaOptions, istate_output_mat_start),
        mem_pair(LibrpaOptions, istate_output_mat_end),
        mem_pair(LibrpaOptions, ifreq_output_wc_start),
        mem_pair(LibrpaOptions, ifreq_output_wc_end),
    };
    {
        auto it = map_librpa_opts_int.find(key_s);
        if (it != map_librpa_opts_int.cend())
        {
            (p_opts_librpa->*(it->second)) = std::stoi(val_s);
            return;
        }
    }

    if (key_s == "libri_chi0_collect_max_bytes")
    {
        p_opts_librpa->libri_chi0_collect_max_bytes = std::stoll(val_s);
        return;
    }

    static const unordered_map<string, double LibrpaOptions::*> map_librpa_opts_double{
        mem_pair(LibrpaOptions, vq_threshold),
        mem_pair(LibrpaOptions, tfgrids_freq_min),
        mem_pair(LibrpaOptions, tfgrids_freq_max),
        mem_pair(LibrpaOptions, tfgrids_freq_interval),
        mem_pair(LibrpaOptions, tfgrids_time_min),
        mem_pair(LibrpaOptions, tfgrids_time_interval),
        mem_pair(LibrpaOptions, minimax_emin),
        mem_pair(LibrpaOptions, minimax_emax),
        mem_pair(LibrpaOptions, minimax_regulation),
        mem_pair(LibrpaOptions, gf_threshold),
        mem_pair(LibrpaOptions, qpe_solver_thres),
        mem_pair(LibrpaOptions, qpe_solver_damp_factor),
        mem_pair(LibrpaOptions, sf_gf_omega_shift),
        mem_pair(LibrpaOptions, sf_sigc_omega_shift),
        mem_pair(LibrpaOptions, sqrt_coulomb_threshold),
        mem_pair(LibrpaOptions, libri_chi0_threshold_C),
        mem_pair(LibrpaOptions, libri_chi0_threshold_G),
        mem_pair(LibrpaOptions, libri_exx_threshold_C),
        mem_pair(LibrpaOptions, libri_exx_threshold_D),
        mem_pair(LibrpaOptions, libri_exx_threshold_V),
        mem_pair(LibrpaOptions, libri_g0w0_threshold_C),
        mem_pair(LibrpaOptions, libri_g0w0_threshold_G),
        mem_pair(LibrpaOptions, libri_g0w0_threshold_Wc),
    };
    {
        auto it = map_librpa_opts_double.find(key_s);
        if (it != map_librpa_opts_double.cend())
        {
            (p_opts_librpa->*(it->second)) = std::stod(val_s);
            return;
        }
    }

    static const unordered_map<string, LibrpaSwitch LibrpaOptions::*> map_librpa_opts_switch{
        mem_pair(LibrpaOptions, use_fullcoul_eps),
        mem_pair(LibrpaOptions, use_fullcoul_exx),
        mem_pair(LibrpaOptions, use_fullcoul_wc),
        mem_pair(LibrpaOptions, use_symmetry_exx),
        mem_pair(LibrpaOptions, use_symmetry_gw),
        mem_pair(LibrpaOptions, use_symmetry_rpa),
        mem_pair(LibrpaOptions, output_abacus_gw_gf),
        mem_pair(LibrpaOptions, use_shrink_abfs),
        mem_pair(LibrpaOptions, use_shrink_chi),
        mem_pair(LibrpaOptions, use_scalapack_ecrpa),
        mem_pair(LibrpaOptions, use_scalapack_gw_wc),
        mem_pair(LibrpaOptions, use_cholesky_gw_wc),
        mem_pair(LibrpaOptions, use_gpu_replace_scalapack),
        mem_pair(LibrpaOptions, use_elpa_sqrt_coulomb),
        mem_pair(LibrpaOptions, read_sigc_mat_rf),
        mem_pair(LibrpaOptions, replace_w_head),
        mem_pair(LibrpaOptions, use_2d_dielectric),
        mem_pair(LibrpaOptions, use_qpe_adaptive_damp),
        mem_pair(LibrpaOptions, use_qpe_legacy_update),
        mem_pair(LibrpaOptions, override_qpe_solver_nan),
        mem_pair(LibrpaOptions, use_hedin_shift),
        mem_pair(LibrpaOptions, output_gw_sigc_ks_kf),
        mem_pair(LibrpaOptions, output_gw_sigc_ks_mat_kf),
        mem_pair(LibrpaOptions, output_exx_ks_mat_k),
        mem_pair(LibrpaOptions, output_gw_sigc_mat_kf),
        mem_pair(LibrpaOptions, output_gw_sigc_mat_rt),
        mem_pair(LibrpaOptions, output_gw_sigc_mat_rf),
        mem_pair(LibrpaOptions, output_wc_rf),
        mem_pair(LibrpaOptions, output_wc_rf_atom_pair),
    };
    {
        auto it = map_librpa_opts_switch.find(key_s);
        if (it != map_librpa_opts_switch.cend())
        {
            (p_opts_librpa->*(it->second)) = parse_librpa_switch(val_s);
            return;
        }
    }

    throw std::invalid_argument("Unknown TAESC option: " + key_s);
}

#undef mem_pair
