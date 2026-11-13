#pragma once
/**
 * @file taesc.hpp
 * @brief C++ namespace facade for TAESC.
 */

#include "taesc_options.h"
#include "taesc_handler.h"
#include "taesc_enums.h"
#include "taesc_qsgw.h"

#include <complex>
#include <cstddef>
#include <functional>
#include <vector>

namespace taesc
{

struct G0W0QpeResult
{
    std::vector<std::complex<double>> sigc;
    std::vector<double> eqp;
};

struct G0W0SpectralFunctionResult
{
    std::vector<std::complex<double>> sigc;
    std::vector<double> spectral_function;
};

struct MeanfieldDimensions
{
    int n_spins = 0, n_kpts = 0, n_states = 0, n_aos = 0, n_spinor = 0;
};

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
void init_global(TaescSwitch switch_redirect_stdout = TAESC_SWITCH_OFF,
                 const char *redirect_path = "stdout",
                 TaescSwitch switch_process_output = TAESC_SWITCH_ON);
void set_output_level(TaescVerbose output_level);
TaescVerbose get_output_level(void);
void finalize_global(void);
void test(void);
void print_profile(void);

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

    void set_scf_dimension(int nspins, int nkpts, int nstates, int nbasis,
                           int nspinor = 1);
    void set_wg_ekb_efermi(int nspins, int nkpts, int nstates,
                           const double *wg, const double *ekb,
                           double efermi);
    void set_wfc(int ispin, int ik, int nstates_local, int nbasis_local,
                 const double *wfc_real, const double *wfc_imag);
    void set_wfc_spinor(int ik, int nstates_local, int nbasis_local,
                        const double *wfc_up_real,
                        const double *wfc_up_imag,
                        const double *wfc_dn_real,
                        const double *wfc_dn_imag);
    void set_wfc_packed(int ispin, int ik, int nstates_local, int nbasis_local,
                        const std::complex<double> *wfc);
    void set_wfc_spinor_packed(int ik, int nstates_local, int nbasis_local,
                               const std::complex<double> *wfc_up,
                               const std::complex<double> *wfc_dn);
    void set_ao_basis_wfc(
        const std::vector<std::size_t> &nbs_wfc,
        const std::vector<std::vector<int>> &l_shells = {});
    void set_ao_basis_aux(
        const std::vector<std::size_t> &nbs_aux,
        const std::vector<std::vector<int>> &l_shells = {});
    void set_ao_basis_aux_shrink(
        const std::vector<std::size_t> &nbs_aux_shrink,
        const std::vector<std::vector<int>> &l_shells = {});
    void set_basis_convention(int bloch_phase, int bloch_ratom,
                              TaescAngularOrder order, TaescRshCoeff nega_m,
                              TaescRshCoeff posi_m);
    void set_symmetry_operations(int n_symops, int row_conv,
                                 const int *rotmats,
                                 const double *trans = nullptr);
    void set_latvec_and_G(const double lat_mat[9], const double G_mat[9]);
    void set_atoms(const std::vector<int> &types,
                   const std::vector<double> &pos_cart);
    void set_kgrids_kvec(int nk1, int nk2, int nk3, int nkpts,
                         const double *kvecs,
                         const double *kweights = nullptr);
    void set_kgrids_kvec(int nk1, int nk2, int nk3,
                         const std::vector<double> &kvecs,
                         const std::vector<double> &kweights = {});
    void set_kq_mapping(const std::vector<int> &map_q_ks);
    void set_lri_coeff(int I, int J, int nbasis_i,
                       int nbasis_j, int naux_mu, const int R[3],
                       const double *Cs_in, int shrink_aux = 0);
    void set_aux_bare_coulomb_k_atom_pair(
        int ik, int I, int J, int naux_mu, int naux_nu,
        const double *Vq_real_in, const double *Vq_imag_in,
        double vq_threshold);
    void set_aux_bare_coulomb_k_atom_pair_packed(
        int ik, int I, int J, int naux_mu, int naux_nu,
        const std::complex<double> *Vq, double vq_threshold);
    void set_aux_cut_coulomb_k_atom_pair(
        int ik, int I, int J, int naux_mu, int naux_nu,
        const double *Vq_real_in, const double *Vq_imag_in,
        double vq_threshold);
    void set_aux_cut_coulomb_k_atom_pair_packed(
        int ik, int I, int J, int naux_mu, int naux_nu,
        const std::complex<double> *Vq, double vq_threshold);
    void set_aux_bare_coulomb_k_2d_block(
        int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
        const double *Vq_real_in, const double *Vq_imag_in);
    void set_aux_bare_coulomb_k_2d_block_packed(
        int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
        const std::complex<double> *Vq);
    void set_aux_cut_coulomb_k_2d_block(
        int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
        const double *Vq_real_in, const double *Vq_imag_in);
    void set_aux_cut_coulomb_k_2d_block_packed(
        int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
        const std::complex<double> *Vq);
    void set_dielect_func_imagfreq(const std::vector<double> &omegas_imag,
                                   const std::vector<double> &dielect_func);
    void set_velocity_matrix(int n_spins, int n_kpts, int n_states,
                             const double *velocity_real,
                             const double *velocity_imag);
    void set_velocity_matrix_packed(int n_spins, int n_kpts, int n_states,
                                    const std::complex<double> *velocity);
    void set_band_kvec(int n_kpts_band, const double *kfrac_band);
    void set_band_occ_eigval(int n_spins, int n_kpts_band, int n_states,
                             const double *occ, const double *eig);
    void set_wfc_band(int ispin, int ik_band, int nstates_local,
                      int nbasis_local, const double *wfc_real,
                      const double *wfc_imag);
    void set_wfc_band_spinor(int ik_band, int nstates_local,
                             int nbasis_local, const double *wfc_up_real,
                             const double *wfc_up_imag,
                             const double *wfc_dn_real,
                             const double *wfc_dn_imag);
    void set_wfc_band_packed(int ispin, int ik_band, int nstates_local,
                             int nbasis_local,
                             const std::complex<double> *wfc);
    void set_wfc_band_spinor_packed(
        int ik_band, int nstates_local, int nbasis_local,
        const std::complex<double> *wfc_up,
        const std::complex<double> *wfc_dn);
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
    G0W0QpeResult get_g0w0_qpe_kgrid(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_this, int i_state_low, int i_state_high,
        const std::vector<double> &vxc, const std::vector<double> &vexx);
    std::vector<double> get_g0w0_spectral_function_kgrid(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_this, int i_state_low, int i_state_high,
        const std::vector<double> &omegas, const std::vector<double> &vxc,
        const std::vector<double> &vexx);
    G0W0SpectralFunctionResult
    get_g0w0_spectral_function_with_sigc_kgrid(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_this, int i_state_low, int i_state_high,
        const std::vector<double> &omegas, const std::vector<double> &vxc,
        const std::vector<double> &vexx);
    std::vector<std::complex<double>> get_g0w0_sigc_band_k(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_band_this, int i_state_low,
        int i_state_high, const std::vector<double> &vxc_band,
        const std::vector<double> &vexx_band);
    G0W0QpeResult get_g0w0_qpe_band_k(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_band_this, int i_state_low,
        int i_state_high, const std::vector<double> &vxc_band,
        const std::vector<double> &vexx_band);
    std::vector<double> get_g0w0_spectral_function_band_k(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_band_this, int i_state_low,
        int i_state_high, const std::vector<double> &omegas,
        const std::vector<double> &vxc_band,
        const std::vector<double> &vexx_band);
    G0W0SpectralFunctionResult
    get_g0w0_spectral_function_with_sigc_band_k(
        const Options &opts, const int n_spins,
        const std::vector<int> &iks_band_this, int i_state_low,
        int i_state_high, const std::vector<double> &omegas,
        const std::vector<double> &vxc_band,
        const std::vector<double> &vexx_band);

    /** Mean-field dimensions. band_path selects the band input. */
    MeanfieldDimensions get_meanfield_dimensions(bool band_path = false) const;
    /** Periodic Hartree AO matrices in Hartree, layout
     *  [requested k][AO row][AO column] (row-major), zero-based k indices. */
    std::vector<std::complex<double>> compute_hartree_ao(
        const Options &opts, const std::vector<int> &iks, int n_aos);
};

/** Receives QSGW outputs on rank zero. Return zero on success. */
using QsgwOutputSink = std::function<int(const TaescQsgwOutput &)>;

/** RAII wrapper of a QSGW task. The handler must outlive the task. */
class QsgwTask
{
private:
    TaescQsgwTask *task_ = nullptr;
    QsgwOutputSink sink_;

public:
    QsgwTask(Handler &h, const Options &opts,
             const TaescQsgwControls *controls = nullptr);
    QsgwTask(const QsgwTask &) = delete;
    QsgwTask &operator=(const QsgwTask &) = delete;
    ~QsgwTask();

    static TaescQsgwControls default_controls();

    void prepare();
    /** Collective. input is only read on handler rank zero. */
    void set_input(const TaescQsgwInput *input);
    void set_output_sink(QsgwOutputSink sink);
    void run();
};


}  // namespace taesc
