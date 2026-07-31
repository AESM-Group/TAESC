#pragma once
/**
 * @file taesc_compute.h
 * @brief TAESC compute facade for LibRPA.
 */

#include "taesc_handler.h"
#include "taesc_options.h"

#ifdef __cplusplus
extern "C" {
#endif

void taesc_get_imaginary_frequency_grids(TaescHandler *h,
                                         const TaescOptions *p_opts,
                                         double *omegas,
                                         double *weights);

double taesc_get_rpa_correlation_energy(TaescHandler *h,
                                        const TaescOptions *p_opts,
                                        int n_ibz_kpoints,
                                        double *rpa_corr_ibzk_contrib_re,
                                        double *rpa_corr_ibzk_contrib_im);

void taesc_build_exx(TaescHandler *h, const TaescOptions *p_opts);

void taesc_get_exx_pot_kgrid(TaescHandler *h, const TaescOptions *p_opts,
                             const int n_spins, const int n_kpts_this,
                             const int *iks_this, int i_state_low,
                             int i_state_high, double *vexx);

void taesc_get_exx_pot_band_k(TaescHandler *h, const TaescOptions *p_opts,
                              const int n_spins, const int n_kpts_band_this,
                              const int *iks_band_this, int i_state_low,
                              int i_state_high, double *vexx_band);

void taesc_build_g0w0_sigma(TaescHandler *h, const TaescOptions *p_opts);

void taesc_get_g0w0_sigc_kgrid(TaescHandler *h, const TaescOptions *p_opts,
                               const int n_spins, const int n_kpts_this,
                               const int *iks_this, int i_state_low,
                               int i_state_high, const double *vxc,
                               const double *vexx, double *sigc_re,
                               double *sigc_im);

void taesc_get_g0w0_qpe_kgrid(TaescHandler *h, const TaescOptions *p_opts,
                              const int n_spins, const int n_kpts_this,
                              const int *iks_this, int i_state_low,
                              int i_state_high, const double *vxc,
                              const double *vexx, double *sigc_re,
                              double *sigc_im, double *eqp);

void taesc_get_g0w0_spectral_function_kgrid(
    TaescHandler *h, const TaescOptions *p_opts, const int n_spins,
    const int n_kpts_this, const int *iks_this, int i_state_low,
    int i_state_high, const int n_omegas, const double *omegas,
    const double *vxc, const double *vexx, double *spectral_function,
    double *sigc);

void taesc_get_g0w0_sigc_band_k(TaescHandler *h, const TaescOptions *p_opts,
                                const int n_spins, const int n_kpts_band_this,
                                const int *iks_band_this, int i_state_low,
                                int i_state_high, const double *vxc_band,
                                const double *vexx_band, double *sigc_band_re,
                                double *sigc_band_im);

void taesc_get_g0w0_qpe_band_k(
    TaescHandler *h, const TaescOptions *p_opts, const int n_spins,
    const int n_kpts_band_this, const int *iks_band_this, int i_state_low,
    int i_state_high, const double *vxc_band, const double *vexx_band,
    double *sigc_band_re, double *sigc_band_im, double *eqp_band);

void taesc_get_g0w0_spectral_function_band_k(
    TaescHandler *h, const TaescOptions *p_opts, const int n_spins,
    const int n_kpts_band_this, const int *iks_band_this, int i_state_low,
    int i_state_high, const int n_omegas, const double *omegas,
    const double *vxc_band, const double *vexx_band,
    double *spectral_function_band, double *sigc_band);

#ifdef __cplusplus
}
#endif
