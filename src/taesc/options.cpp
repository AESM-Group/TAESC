#include <taesc/taesc_options.h>
#include "../LibRPA/include/librpa_options.h"

#include "options_bridge.h"

#include <set>
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

} // namespace taesc_internal

void taesc_set_option(TaescOptions *opts, const char *key, const char *value)
{
    using std::string;
    using std::set;
    using std::unordered_map;
    using taesc_internal::get_librpa_options;
    using taesc_internal::parse_librpa_switch;

    const string key_s(key), val_s(value);

    auto p_opts_librpa = get_librpa_options(opts);

    // Options that should be aligned across different components
    if (key_s == "output_dir")
    {
        librpa_set_output_dir(p_opts_librpa, value);
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
            {"auto", LibrpaParallelRouting::AUTO},
            {"rtau", LibrpaParallelRouting::RTAU},
            {"atompair", LibrpaParallelRouting::ATOMPAIR},
            {"libri", LibrpaParallelRouting::LIBRI},
        };
        p_opts_librpa->parallel_routing = kv.at(val_s);
        return;
    }
    if (key_s == "tfgrids_type")
    {
        static const unordered_map<string, LibrpaTimeFreqGrid> kv
        {
            {"GL", LibrpaTimeFreqGrid::GaussLegendre},
            {"GC-I", LibrpaTimeFreqGrid::GaussChebyshevI},
            {"GC-II", LibrpaTimeFreqGrid::GaussChebyshevII},
            {"minimax", LibrpaTimeFreqGrid::Minimax},
            {"evenspaced", LibrpaTimeFreqGrid::EvenSpaced},
            {"evenspaced_tf", LibrpaTimeFreqGrid::EvenSpaced_TF},
        };
        p_opts_librpa->tfgrids_type = kv.at(val_s);
        return;
    }

    static const unordered_map<string, int LibrpaOptions::*> map_librpa_opts_int{
        mem_pair(LibrpaOptions, nfreq),
        mem_pair(LibrpaOptions, n_params_anacon),
        mem_pair(LibrpaOptions, option_dielect_func),
    };
    {
        auto it = map_librpa_opts_int.find(key_s);
        if (it != map_librpa_opts_int.cend())
        {
            (p_opts_librpa->*(it->second)) = std::stoi(val_s);
            return;
        }
    }

    // const set<string> opts_librpa_switch{};
    static const unordered_map<string, double LibrpaOptions::*> map_librpa_opts_double{
        mem_pair(LibrpaOptions, vq_threshold),
        mem_pair(LibrpaOptions, tfgrids_freq_min),
        mem_pair(LibrpaOptions, tfgrids_freq_max),
        mem_pair(LibrpaOptions, tfgrids_freq_interval),
        mem_pair(LibrpaOptions, tfgrids_time_min),
        mem_pair(LibrpaOptions, tfgrids_time_interval),
        mem_pair(LibrpaOptions, gf_threshold),
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
        mem_pair(LibrpaOptions, use_scalapack_ecrpa),
        mem_pair(LibrpaOptions, use_scalapack_gw_wc),
        mem_pair(LibrpaOptions, replace_w_head),
        // mem_pair(LibrpaOptions, output_gw_sigc_mat),
        // mem_pair(LibrpaOptions, output_gw_sigc_mat_rt),
        // mem_pair(LibrpaOptions, output_gw_sigc_mat_rf),
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
