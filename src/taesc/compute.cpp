#include <taesc/taesc_compute.h>

#include <librpa_compute.h>

#include "handler_bridge.h"
#include "options_bridge.h"

using taesc_internal::get_librpa_handler;
using taesc_internal::get_librpa_options;

void taesc_get_imaginary_frequency_grids(TaescHandler *h,
                                         const TaescOptions *p_opts,
                                         double *omegas,
                                         double *weights)
{
    librpa_get_imaginary_frequency_grids(get_librpa_handler(h),
                                         get_librpa_options(p_opts),
                                         omegas, weights);
}

double taesc_get_rpa_correlation_energy(
    TaescHandler *h, const TaescOptions *p_opts, int n_ibz_kpoints,
    double *rpa_corr_ibzk_contrib_re, double *rpa_corr_ibzk_contrib_im)
{
    return librpa_get_rpa_correlation_energy(get_librpa_handler(h),
                                             get_librpa_options(p_opts),
                                             n_ibz_kpoints,
                                             rpa_corr_ibzk_contrib_re,
                                             rpa_corr_ibzk_contrib_im);
}

void taesc_build_exx(TaescHandler *h, const TaescOptions *p_opts)
{
    librpa_build_exx(get_librpa_handler(h), get_librpa_options(p_opts));
}

void taesc_get_exx_pot_kgrid(TaescHandler *h, const TaescOptions *p_opts,
                             const int n_spins, const int n_kpts_this,
                             const int *iks_this, int i_state_low,
                             int i_state_high, double *vexx)
{
    librpa_get_exx_pot_kgrid(get_librpa_handler(h), get_librpa_options(p_opts),
                             n_spins,
                             n_kpts_this, iks_this, i_state_low,
                             i_state_high, vexx);
}

void taesc_get_exx_pot_band_k(TaescHandler *h, const TaescOptions *p_opts,
                              const int n_spins,
                              const int n_kpts_band_this,
                              const int *iks_band_this, int i_state_low,
                              int i_state_high, double *vexx_band)
{
    librpa_get_exx_pot_band_k(get_librpa_handler(h), get_librpa_options(p_opts),
                              n_spins,
                              n_kpts_band_this, iks_band_this, i_state_low,
                              i_state_high, vexx_band);
}

void taesc_build_g0w0_sigma(TaescHandler *h, const TaescOptions *p_opts)
{
    librpa_build_g0w0_sigma(get_librpa_handler(h), get_librpa_options(p_opts));
}

void taesc_get_g0w0_sigc_kgrid(
    TaescHandler *h, const TaescOptions *p_opts, const int n_spins,
    const int n_kpts_this, const int *iks_this, int i_state_low,
    int i_state_high, const double *vxc, const double *vexx,
    double *sigc_re, double *sigc_im)
{
    librpa_get_g0w0_sigc_kgrid(get_librpa_handler(h),
                               get_librpa_options(p_opts), n_spins,
                               n_kpts_this, iks_this, i_state_low,
                               i_state_high, vxc, vexx, sigc_re, sigc_im);
}

void taesc_get_g0w0_sigc_band_k(
    TaescHandler *h, const TaescOptions *p_opts, const int n_spins,
    const int n_kpts_band_this, const int *iks_band_this, int i_state_low,
    int i_state_high, const double *vxc_band, const double *vexx_band,
    double *sigc_band_re, double *sigc_band_im)
{
    librpa_get_g0w0_sigc_band_k(get_librpa_handler(h),
                                get_librpa_options(p_opts), n_spins,
                                n_kpts_band_this, iks_band_this,
                                i_state_low, i_state_high, vxc_band,
                                vexx_band, sigc_band_re, sigc_band_im);
}
