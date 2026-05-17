#pragma once
/**
 * @file taesc.hpp
 * @brief C++ namespace facade for TAESC.
 */

#include "taesc_options.h"
#include "taesc_handler.h"

#include <complex>
#include <cstddef>
#include <vector>

namespace taesc
{

class Options
{
private:
    TaescOptions *opts_;

public:
    Options();
    Options(const Options&) = delete;
    Options& operator=(const Options&) = delete;
    Options(Options&& other) noexcept;
    Options& operator=(Options&& other) noexcept;
    ~Options();

    const TaescOptions* get_c_options() const;
    TaescOptions* get_c_options();
    void set(const char *key, const char *value);
};

const char* get_build_info(void);
int get_major_version(void);
int get_minor_version(void);
int get_patch_version(void);
void init_global();
void finalize_global(void);

class Handler
{
private:
    TaescHandler *h_;

public:
    Handler();
    explicit Handler(MPI_Comm comm);
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;
    ~Handler();

    const TaescHandler* get_c_handler() const;
    TaescHandler* get_c_handler();

    void init(MPI_Comm comm);
    void free();

    void set_scf_dimension(int nspins, int nkpts, int nstates, int nbasis);
    void set_wg_ekb_efermi(int nspins, int nkpts, int nstates,
                           const double *wg, const double *ekb,
                           double efermi);
    void set_wfc(int ispin, int ik, int nstates_local, int nbasis_local,
                 const double *wfc_real, const double *wfc_imag);
    void set_wfc_packed(int ispin, int ik, int nstates_local, int nbasis_local,
                        const std::complex<double> *wfc);
    void set_ao_basis_wfc(const std::vector<std::size_t> &nbs_wfc);
    void set_ao_basis_aux(const std::vector<std::size_t> &nbs_aux);
    void set_latvec_and_G(const double lat_mat[9], const double G_mat[9]);
    void set_atoms(const std::vector<int> &types,
                   const std::vector<double> &pos_cart);
    void set_kgrids_kvec(int nk1, int nk2, int nk3, const double *kvecs);
    void set_ibz_mapping(const std::vector<int> &map_ibzk);
    void set_lri_coeff(int I, int J, int nbasis_i,
                       int nbasis_j, int naux_mu, const int R[3],
                       const double *Cs_in);
    void set_aux_bare_coulomb_k_atom_pair(
        int ik, int I, int J, int naux_mu, int naux_nu,
        const double *Vq_real_in, const double *Vq_imag_in,
        double vq_threshold);
    void set_aux_cut_coulomb_k_atom_pair(
        int ik, int I, int J, int naux_mu, int naux_nu,
        const double *Vq_real_in, const double *Vq_imag_in,
        double vq_threshold);
    void set_aux_bare_coulomb_k_2d_block(
        int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
        const double *Vq_real_in, const double *Vq_imag_in);
    void set_aux_cut_coulomb_k_2d_block(
        int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
        const double *Vq_real_in, const double *Vq_imag_in);
    void set_dielect_func_imagfreq(const std::vector<double> &omegas_imag,
                                   const std::vector<double> &dielect_func);
    void set_band_kvec(int n_kpts_band, const double *kfrac_band);
    void set_band_occ_eigval(int n_spins, int n_kpts_band, int n_states,
                             const double *occ, const double *eig);
    void set_wfc_band(int ispin, int ik_band, int nstates_local,
                      int nbasis_local, const double *wfc_real,
                      const double *wfc_imag);
    void set_wfc_band_packed(int ispin, int ik_band, int nstates_local,
                             int nbasis_local,
                             const std::complex<double> *wfc);
    void reset_band_data();

    void get_imaginary_frequency_grids(const Options &opts,
                                       std::vector<double> &omegas,
                                       std::vector<double> &weights);
    double get_rpa_correlation_energy(
        const Options &opts,
        std::vector<std::complex<double>> &rpa_corr_ibzk_contrib);
    void build_exx(const Options &opts);
    std::vector<double> get_exx_pot_kgrid(const Options &opts,
                                          const int n_spins,
                                          const std::vector<int> &iks_this,
                                          int i_state_low,
                                          int i_state_high);
    std::vector<double> get_exx_pot_band_k(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_band_this, int i_state_low,
        int i_state_high);
    void build_g0w0_sigma(const Options &opts);
    std::vector<std::complex<double>> get_g0w0_sigc_kgrid(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_this, int i_state_low, int i_state_high,
        const std::vector<double> &vxc, const std::vector<double> &vexx);
    std::vector<std::complex<double>> get_g0w0_sigc_band_k(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_band_this, int i_state_low,
        int i_state_high, const std::vector<double> &vxc_band,
        const std::vector<double> &vexx_band);
};

}  // namespace taesc
