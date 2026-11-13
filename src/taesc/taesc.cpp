#include "taesc/taesc.hpp"

#include "taesc/taesc_compute.h"
#include "taesc/taesc_global.h"
#include "taesc/taesc_input.h"
#include "taesc/taesc_options.h"

#include "options_bridge.h"

#include <limits>
#include <stdexcept>
#include <utility>

namespace taesc
{

Options::Options()
    : opts_(taesc_create_options())
{
}

Options::Options(Options&& other) noexcept
    : opts_(other.opts_)
{
    other.opts_ = nullptr;
}

Options& Options::operator=(Options&& other) noexcept
{
    if (this != &other) {
        taesc_destroy_options(opts_);
        opts_ = other.opts_;
        other.opts_ = nullptr;
    }
    return *this;
}

Options::~Options()
{
    taesc_destroy_options(opts_);
}

const TaescOptions* Options::get_c_options() const
{
    return opts_;
}

TaescOptions* Options::get_c_options()
{
    return opts_;
}

void Options::set(const char *key, const char *value)
{
    taesc_set_option(opts_, key, value);
}

const char* get_build_info(void)
{
    return taesc_get_build_info();
}

int get_major_version(void)
{
    return taesc_get_major_version();
}

int get_minor_version(void)
{
    return taesc_get_minor_version();
}

int get_patch_version(void)
{
    return taesc_get_patch_version();
}

void init_global(TaescSwitch switch_redirect_stdout, const char *redirect_path,
                 TaescSwitch switch_process_output)
{
    taesc_init_global(switch_redirect_stdout, redirect_path,
                      switch_process_output);
}

void set_output_level(TaescVerbose output_level)
{
    taesc_set_output_level(output_level);
}

TaescVerbose get_output_level(void)
{
    return taesc_get_output_level();
}

void finalize_global(void)
{
    taesc_finalize_global();
}

void test(void)
{
    taesc_test();
}

void print_profile(void)
{
    taesc_print_profile();
}

Handler::Handler() : h_(nullptr) {}

Handler::Handler(MPI_Comm comm) : h_(nullptr)
{
    init(comm);
}

Handler::~Handler()
{
    free();
}

const TaescHandler* Handler::get_c_handler() const
{
    return h_;
}

TaescHandler* Handler::get_c_handler()
{
    return h_;
}

void Handler::init(MPI_Comm comm)
{
    if (h_) {
        throw std::runtime_error("double creating handler");
    }
    h_ = taesc_create_handler(comm);
}

void Handler::free()
{
    if (h_) {
        taesc_destroy_handler(h_);
    }
    h_ = nullptr;
}

void Handler::set_scf_dimension(int nspins, int nkpts, int nstates, int nbasis,
                                int nspinor)
{
    taesc_set_scf_dimension(h_, nspins, nkpts, nstates, nbasis, nspinor);
}

void Handler::set_wfc_spinor(int ik, int nstates_local, int nbasis_local,
                             const double *wfc_up_real,
                             const double *wfc_up_imag,
                             const double *wfc_dn_real,
                             const double *wfc_dn_imag)
{
    taesc_set_wfc_spinor(h_, ik, nstates_local, nbasis_local, wfc_up_real,
                         wfc_up_imag, wfc_dn_real, wfc_dn_imag);
}

void Handler::set_wg_ekb_efermi(int nspins, int nkpts, int nstates,
                                const double *wg, const double *ekb,
                                double efermi)
{
    taesc_set_wg_ekb_efermi(h_, nspins, nkpts, nstates, wg, ekb, efermi);
}

void Handler::set_wfc(int ispin, int ik, int nstates_local, int nbasis_local,
                      const double *wfc_real, const double *wfc_imag)
{
    taesc_set_wfc(h_, ispin, ik, nstates_local, nbasis_local,
                  wfc_real, wfc_imag);
}

void Handler::set_wfc_packed(int ispin, int ik, int nstates_local,
                             int nbasis_local,
                             const std::complex<double> *wfc)
{
    taesc_set_wfc_packed(h_, ispin, ik, nstates_local, nbasis_local,
                         reinterpret_cast<const double*>(wfc));
}

void Handler::set_wfc_spinor_packed(int ik, int nstates_local,
                                    int nbasis_local,
                                    const std::complex<double> *wfc_up,
                                    const std::complex<double> *wfc_dn)
{
    taesc_set_wfc_spinor_packed(
        h_, ik, nstates_local, nbasis_local,
        reinterpret_cast<const double*>(wfc_up),
        reinterpret_cast<const double*>(wfc_dn));
}

namespace
{

void get_l_shell_buffers(const std::size_t natoms,
                         const std::vector<std::vector<int>> &l_shells,
                         std::vector<int> &nshells,
                         std::vector<int> &flat_l_shells)
{
    if (l_shells.empty()) return;
    if (l_shells.size() != natoms)
        throw std::invalid_argument(
            "l-shell metadata must have one entry per atom");
    nshells.reserve(natoms);
    for (const auto &atom_l_shells : l_shells) {
        nshells.push_back(static_cast<int>(atom_l_shells.size()));
        flat_l_shells.insert(flat_l_shells.end(), atom_l_shells.begin(),
                             atom_l_shells.end());
    }
}

}  // namespace

void Handler::set_ao_basis_wfc(
    const std::vector<std::size_t> &nbs_wfc,
    const std::vector<std::vector<int>> &l_shells)
{
    std::vector<int> nshells;
    std::vector<int> flat_l_shells;
    get_l_shell_buffers(nbs_wfc.size(), l_shells, nshells, flat_l_shells);
    taesc_set_ao_basis_wfc(
        h_, static_cast<int>(nbs_wfc.size()), nbs_wfc.data(),
        nshells.empty() ? nullptr : nshells.data(),
        flat_l_shells.empty() ? nullptr : flat_l_shells.data());
}

void Handler::set_ao_basis_aux(
    const std::vector<std::size_t> &nbs_aux,
    const std::vector<std::vector<int>> &l_shells)
{
    std::vector<int> nshells;
    std::vector<int> flat_l_shells;
    get_l_shell_buffers(nbs_aux.size(), l_shells, nshells, flat_l_shells);
    taesc_set_ao_basis_aux(
        h_, static_cast<int>(nbs_aux.size()), nbs_aux.data(),
        nshells.empty() ? nullptr : nshells.data(),
        flat_l_shells.empty() ? nullptr : flat_l_shells.data());
}

void Handler::set_ao_basis_aux_shrink(
    const std::vector<std::size_t> &nbs_aux_shrink,
    const std::vector<std::vector<int>> &l_shells)
{
    std::vector<int> nshells;
    std::vector<int> flat_l_shells;
    get_l_shell_buffers(nbs_aux_shrink.size(), l_shells, nshells,
                        flat_l_shells);
    taesc_set_ao_basis_aux_shrink(
        h_, static_cast<int>(nbs_aux_shrink.size()), nbs_aux_shrink.data(),
        nshells.empty() ? nullptr : nshells.data(),
        flat_l_shells.empty() ? nullptr : flat_l_shells.data());
}

void Handler::set_basis_convention(int bloch_phase, int bloch_ratom,
                                   TaescAngularOrder order,
                                   TaescRshCoeff nega_m,
                                   TaescRshCoeff posi_m)
{
    taesc_set_basis_convention(h_, bloch_phase, bloch_ratom, order, nega_m,
                               posi_m);
}

void Handler::set_symmetry_operations(int n_symops, int row_conv,
                                      const int *rotmats,
                                      const double *trans)
{
    taesc_set_symmetry_operations(h_, n_symops, row_conv, rotmats, trans);
}

void Handler::set_latvec_and_G(const double lat_mat[9], const double G_mat[9])
{
    taesc_set_latvec_and_G(h_, lat_mat, G_mat);
}

void Handler::set_atoms(const std::vector<int> &types,
                        const std::vector<double> &pos_cart)
{
    taesc_set_atoms(h_, static_cast<int>(types.size()),
                    types.data(), pos_cart.data());
}

void Handler::set_kgrids_kvec(int nk1, int nk2, int nk3, int nkpts,
                              const double *kvecs, const double *kweights)
{
    taesc_set_kgrids_kvec(h_, nk1, nk2, nk3, nkpts, kvecs, kweights);
}

void Handler::set_kgrids_kvec(int nk1, int nk2, int nk3,
                              const std::vector<double> &kvecs,
                              const std::vector<double> &kweights)
{
    if (kvecs.size() % 3 != 0)
        throw std::invalid_argument("invalid k-point vector buffer size");
    const auto nkpts = kvecs.size() / 3;
    if (!kweights.empty() && kweights.size() != nkpts)
        throw std::invalid_argument(
            "k-point weight count does not match loaded k-point count");
    if (nkpts > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::invalid_argument("loaded k-point count exceeds integer range");
    taesc_set_kgrids_kvec(
        h_, nk1, nk2, nk3, static_cast<int>(nkpts), kvecs.data(),
        kweights.empty() ? nullptr : kweights.data());
}

void Handler::set_kq_mapping(const std::vector<int> &map_q_ks)
{
    taesc_set_kq_mapping(h_, static_cast<int>(map_q_ks.size()),
                         map_q_ks.data());
}

void Handler::set_lri_coeff(int I, int J,
                            int nbasis_i, int nbasis_j, int naux_mu,
                            const int R[3], const double *Cs_in,
                            int shrink_aux)
{
    taesc_set_lri_coeff(h_, I, J, nbasis_i, nbasis_j,
                        naux_mu, R, Cs_in, shrink_aux);
}

void Handler::set_aux_bare_coulomb_k_atom_pair(
    int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    taesc_set_aux_bare_coulomb_k_atom_pair(h_, ik, I, J, naux_mu, naux_nu,
                                           Vq_real_in, Vq_imag_in,
                                           vq_threshold);
}

void Handler::set_aux_bare_coulomb_k_atom_pair_packed(
    int ik, int I, int J, int naux_mu, int naux_nu,
    const std::complex<double> *Vq, double vq_threshold)
{
    taesc_set_aux_bare_coulomb_k_atom_pair_packed(
        h_, ik, I, J, naux_mu, naux_nu,
        reinterpret_cast<const double*>(Vq), vq_threshold);
}

void Handler::set_aux_cut_coulomb_k_atom_pair(
    int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    taesc_set_aux_cut_coulomb_k_atom_pair(h_, ik, I, J, naux_mu, naux_nu,
                                          Vq_real_in, Vq_imag_in,
                                          vq_threshold);
}

void Handler::set_aux_cut_coulomb_k_atom_pair_packed(
    int ik, int I, int J, int naux_mu, int naux_nu,
    const std::complex<double> *Vq, double vq_threshold)
{
    taesc_set_aux_cut_coulomb_k_atom_pair_packed(
        h_, ik, I, J, naux_mu, naux_nu,
        reinterpret_cast<const double*>(Vq), vq_threshold);
}

void Handler::set_aux_bare_coulomb_k_2d_block(
    int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
    const double *Vq_real_in, const double *Vq_imag_in)
{
    taesc_set_aux_bare_coulomb_k_2d_block(h_, ik, mu_begin, mu_end,
                                          nu_begin, nu_end,
                                          Vq_real_in, Vq_imag_in);
}

void Handler::set_aux_bare_coulomb_k_2d_block_packed(
    int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
    const std::complex<double> *Vq)
{
    taesc_set_aux_bare_coulomb_k_2d_block_packed(
        h_, ik, mu_begin, mu_end, nu_begin, nu_end,
        reinterpret_cast<const double*>(Vq));
}

void Handler::set_aux_cut_coulomb_k_2d_block(
    int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
    const double *Vq_real_in, const double *Vq_imag_in)
{
    taesc_set_aux_cut_coulomb_k_2d_block(h_, ik, mu_begin, mu_end,
                                         nu_begin, nu_end,
                                         Vq_real_in, Vq_imag_in);
}

void Handler::set_aux_cut_coulomb_k_2d_block_packed(
    int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
    const std::complex<double> *Vq)
{
    taesc_set_aux_cut_coulomb_k_2d_block_packed(
        h_, ik, mu_begin, mu_end, nu_begin, nu_end,
        reinterpret_cast<const double*>(Vq));
}

void Handler::set_dielect_func_imagfreq(
    const std::vector<double> &omegas_imag,
    const std::vector<double> &dielect_func)
{
    taesc_set_dielect_func_imagfreq(h_, static_cast<int>(omegas_imag.size()),
                                    omegas_imag.data(), dielect_func.data());
}

void Handler::set_velocity_matrix(int n_spins, int n_kpts, int n_states,
                                  const double *velocity_real,
                                  const double *velocity_imag)
{
    taesc_set_velocity_matrix(h_, n_spins, n_kpts, n_states, velocity_real,
                              velocity_imag);
}

void Handler::set_velocity_matrix_packed(
    int n_spins, int n_kpts, int n_states,
    const std::complex<double> *velocity)
{
    taesc_set_velocity_matrix_packed(
        h_, n_spins, n_kpts, n_states,
        reinterpret_cast<const double*>(velocity));
}

void Handler::set_band_kvec(int n_kpts_band, const double *kfrac_band)
{
    taesc_set_band_kvec(h_, n_kpts_band, kfrac_band);
}

void Handler::set_band_occ_eigval(int n_spins, int n_kpts_band, int n_states,
                                  const double *occ, const double *eig)
{
    taesc_set_band_occ_eigval(h_, n_spins, n_kpts_band, n_states, occ, eig);
}

void Handler::set_wfc_band(int ispin, int ik_band, int nstates_local,
                           int nbasis_local, const double *wfc_real,
                           const double *wfc_imag)
{
    taesc_set_wfc_band(h_, ispin, ik_band, nstates_local, nbasis_local,
                       wfc_real, wfc_imag);
}

void Handler::set_wfc_band_spinor(int ik_band, int nstates_local,
                                  int nbasis_local,
                                  const double *wfc_up_real,
                                  const double *wfc_up_imag,
                                  const double *wfc_dn_real,
                                  const double *wfc_dn_imag)
{
    taesc_set_wfc_band_spinor(h_, ik_band, nstates_local, nbasis_local,
                              wfc_up_real, wfc_up_imag, wfc_dn_real,
                              wfc_dn_imag);
}

void Handler::set_wfc_band_packed(int ispin, int ik_band, int nstates_local,
                                  int nbasis_local,
                                  const std::complex<double> *wfc)
{
    taesc_set_wfc_band_packed(h_, ispin, ik_band, nstates_local,
                              nbasis_local,
                              reinterpret_cast<const double*>(wfc));
}

void Handler::set_wfc_band_spinor_packed(
    int ik_band, int nstates_local, int nbasis_local,
    const std::complex<double> *wfc_up,
    const std::complex<double> *wfc_dn)
{
    taesc_set_wfc_band_spinor_packed(
        h_, ik_band, nstates_local, nbasis_local,
        reinterpret_cast<const double*>(wfc_up),
        reinterpret_cast<const double*>(wfc_dn));
}

void Handler::reset_band_data()
{
    taesc_reset_band_data(h_);
}

void Handler::get_imaginary_frequency_grids(const Options &opts,
                                            std::vector<double> &omegas,
                                            std::vector<double> &weights)
{
    const auto opts_librpa = taesc_internal::get_librpa_options(opts.get_c_options());
    const int nfreq = opts_librpa->nfreq;
    omegas.resize(nfreq);
    weights.resize(nfreq);
    taesc_get_imaginary_frequency_grids(h_, opts.get_c_options(), omegas.data(),
                                        weights.data());
}

double Handler::get_rpa_correlation_energy(
    const Options &opts,
    std::vector<std::complex<double>> &rpa_corr_ibzk_contrib)
{
    const int nq = static_cast<int>(rpa_corr_ibzk_contrib.size());
    std::vector<double> re(nq);
    std::vector<double> im(nq);
    const double total = taesc_get_rpa_correlation_energy(
        h_, opts.get_c_options(), nq, re.data(), im.data());
    for (int iq = 0; iq < nq; iq++) {
        rpa_corr_ibzk_contrib[iq] = std::complex<double>{re[iq], im[iq]};
    }
    return total;
}

void Handler::build_exx(const Options &opts)
{
    taesc_build_exx(h_, opts.get_c_options());
}

std::vector<double> Handler::get_exx_pot_kgrid(
    const Options &opts, const int n_spins,
    const std::vector<int> &iks_this, int i_state_low, int i_state_high)
{
    const int n_kpts_this = static_cast<int>(iks_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    std::vector<double> vexx(n_spins * n_kpts_this * n_states_calc);
    taesc_get_exx_pot_kgrid(h_, opts.get_c_options(), n_spins, n_kpts_this,
                            iks_this.data(), i_state_low, i_state_high,
                            vexx.data());
    return vexx;
}

std::vector<double> Handler::get_exx_pot_band_k(
    const Options &opts, const int n_spins,
    const std::vector<int> &iks_band_this, int i_state_low, int i_state_high)
{
    const int n_kpts_band_this = static_cast<int>(iks_band_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    std::vector<double> vexx_band(n_spins * n_kpts_band_this * n_states_calc);
    taesc_get_exx_pot_band_k(h_, opts.get_c_options(), n_spins, n_kpts_band_this,
                             iks_band_this.data(), i_state_low, i_state_high,
                             vexx_band.data());
    return vexx_band;
}

void Handler::build_g0w0_sigma(const Options &opts)
{
    taesc_build_g0w0_sigma(h_, opts.get_c_options());
}

std::vector<std::complex<double>> Handler::get_g0w0_sigc_kgrid(
    const Options &opts, const int n_spins, const std::vector<int> &iks_this,
    int i_state_low, int i_state_high, const std::vector<double> &vxc,
    const std::vector<double> &vexx)
{
    const int n_kpts_this = static_cast<int>(iks_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const std::size_t n = n_spins * n_kpts_this * n_states_calc;
    std::vector<double> sigc_re(n);
    std::vector<double> sigc_im(n);
    taesc_get_g0w0_sigc_kgrid(h_, opts.get_c_options(), n_spins, n_kpts_this,
                              iks_this.data(), i_state_low, i_state_high,
                              vxc.data(), vexx.data(), sigc_re.data(),
                              sigc_im.data());
    std::vector<std::complex<double>> sigc(n);
    for (std::size_t i = 0; i < n; i++) {
        sigc[i] = std::complex<double>{sigc_re[i], sigc_im[i]};
    }
    return sigc;
}

G0W0QpeResult Handler::get_g0w0_qpe_kgrid(
    const Options &opts, const int n_spins, const std::vector<int> &iks_this,
    int i_state_low, int i_state_high, const std::vector<double> &vxc,
    const std::vector<double> &vexx)
{
    const int n_kpts_this = static_cast<int>(iks_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const std::size_t n = n_spins * n_kpts_this * n_states_calc;
    std::vector<double> sigc_re(n);
    std::vector<double> sigc_im(n);
    std::vector<double> eqp(n);
    taesc_get_g0w0_qpe_kgrid(
        h_, opts.get_c_options(), n_spins, n_kpts_this, iks_this.data(),
        i_state_low, i_state_high, vxc.data(), vexx.data(), sigc_re.data(),
        sigc_im.data(), eqp.data());
    std::vector<std::complex<double>> sigc(n);
    for (std::size_t i = 0; i < n; ++i)
        sigc[i] = {sigc_re[i], sigc_im[i]};
    return {std::move(sigc), std::move(eqp)};
}

std::vector<double> Handler::get_g0w0_spectral_function_kgrid(
    const Options &opts, const int n_spins, const std::vector<int> &iks_this,
    int i_state_low, int i_state_high, const std::vector<double> &omegas,
    const std::vector<double> &vxc, const std::vector<double> &vexx)
{
    const int n_kpts_this = static_cast<int>(iks_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const int n_omegas = static_cast<int>(omegas.size());
    const std::size_t n = static_cast<std::size_t>(n_spins) * n_kpts_this
                          * n_states_calc * n_omegas;
    std::vector<double> spectral_function(n);
    taesc_get_g0w0_spectral_function_kgrid(
        h_, opts.get_c_options(), n_spins, n_kpts_this, iks_this.data(),
        i_state_low, i_state_high, n_omegas, omegas.data(), vxc.data(),
        vexx.data(), spectral_function.data(), nullptr);
    return spectral_function;
}

G0W0SpectralFunctionResult
Handler::get_g0w0_spectral_function_with_sigc_kgrid(
    const Options &opts, const int n_spins, const std::vector<int> &iks_this,
    int i_state_low, int i_state_high, const std::vector<double> &omegas,
    const std::vector<double> &vxc, const std::vector<double> &vexx)
{
    const int n_kpts_this = static_cast<int>(iks_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const int n_omegas = static_cast<int>(omegas.size());
    const std::size_t n = static_cast<std::size_t>(n_spins) * n_kpts_this
                          * n_states_calc * n_omegas;
    std::vector<std::complex<double>> sigc(n);
    std::vector<double> spectral_function(n);
    taesc_get_g0w0_spectral_function_kgrid(
        h_, opts.get_c_options(), n_spins, n_kpts_this, iks_this.data(),
        i_state_low, i_state_high, n_omegas, omegas.data(), vxc.data(),
        vexx.data(), spectral_function.data(),
        reinterpret_cast<double*>(sigc.data()));
    return {std::move(sigc), std::move(spectral_function)};
}

std::vector<std::complex<double>> Handler::get_g0w0_sigc_band_k(
    const Options &opts, const int n_spins,
    const std::vector<int> &iks_band_this, int i_state_low, int i_state_high,
    const std::vector<double> &vxc_band, const std::vector<double> &vexx_band)
{
    const int n_kpts_band_this = static_cast<int>(iks_band_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const std::size_t n = n_spins * n_kpts_band_this * n_states_calc;
    std::vector<double> sigc_band_re(n);
    std::vector<double> sigc_band_im(n);
    taesc_get_g0w0_sigc_band_k(h_, opts.get_c_options(), n_spins,
                               n_kpts_band_this,
                               iks_band_this.data(), i_state_low,
                               i_state_high, vxc_band.data(),
                               vexx_band.data(), sigc_band_re.data(),
                               sigc_band_im.data());
    std::vector<std::complex<double>> sigc_band(n);
    for (std::size_t i = 0; i < n; i++) {
        sigc_band[i] = std::complex<double>{sigc_band_re[i],
                                            sigc_band_im[i]};
    }
    return sigc_band;
}

G0W0QpeResult Handler::get_g0w0_qpe_band_k(
    const Options &opts, const int n_spins,
    const std::vector<int> &iks_band_this, int i_state_low,
    int i_state_high, const std::vector<double> &vxc_band,
    const std::vector<double> &vexx_band)
{
    const int n_kpts_band_this = static_cast<int>(iks_band_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const std::size_t n = n_spins * n_kpts_band_this * n_states_calc;
    std::vector<double> sigc_re(n);
    std::vector<double> sigc_im(n);
    std::vector<double> eqp(n);
    taesc_get_g0w0_qpe_band_k(
        h_, opts.get_c_options(), n_spins, n_kpts_band_this,
        iks_band_this.data(), i_state_low, i_state_high, vxc_band.data(),
        vexx_band.data(), sigc_re.data(), sigc_im.data(), eqp.data());
    std::vector<std::complex<double>> sigc(n);
    for (std::size_t i = 0; i < n; ++i)
        sigc[i] = {sigc_re[i], sigc_im[i]};
    return {std::move(sigc), std::move(eqp)};
}

std::vector<double> Handler::get_g0w0_spectral_function_band_k(
    const Options &opts, const int n_spins,
    const std::vector<int> &iks_band_this, int i_state_low,
    int i_state_high, const std::vector<double> &omegas,
    const std::vector<double> &vxc_band,
    const std::vector<double> &vexx_band)
{
    const int n_kpts_band_this = static_cast<int>(iks_band_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const int n_omegas = static_cast<int>(omegas.size());
    const std::size_t n = static_cast<std::size_t>(n_spins)
                          * n_kpts_band_this * n_states_calc * n_omegas;
    std::vector<double> spectral_function(n);
    taesc_get_g0w0_spectral_function_band_k(
        h_, opts.get_c_options(), n_spins, n_kpts_band_this,
        iks_band_this.data(), i_state_low, i_state_high, n_omegas,
        omegas.data(), vxc_band.data(), vexx_band.data(),
        spectral_function.data(), nullptr);
    return spectral_function;
}

G0W0SpectralFunctionResult
Handler::get_g0w0_spectral_function_with_sigc_band_k(
    const Options &opts, const int n_spins,
    const std::vector<int> &iks_band_this, int i_state_low,
    int i_state_high, const std::vector<double> &omegas,
    const std::vector<double> &vxc_band,
    const std::vector<double> &vexx_band)
{
    const int n_kpts_band_this = static_cast<int>(iks_band_this.size());
    const int n_states_calc = i_state_high - i_state_low;
    const int n_omegas = static_cast<int>(omegas.size());
    const std::size_t n = static_cast<std::size_t>(n_spins)
                          * n_kpts_band_this * n_states_calc * n_omegas;
    std::vector<std::complex<double>> sigc(n);
    std::vector<double> spectral_function(n);
    taesc_get_g0w0_spectral_function_band_k(
        h_, opts.get_c_options(), n_spins, n_kpts_band_this,
        iks_band_this.data(), i_state_low, i_state_high, n_omegas,
        omegas.data(), vxc_band.data(), vexx_band.data(),
        spectral_function.data(), reinterpret_cast<double*>(sigc.data()));
    return {std::move(sigc), std::move(spectral_function)};
}

MeanfieldDimensions Handler::get_meanfield_dimensions(bool band_path) const
{
    MeanfieldDimensions d;
    taesc_get_meanfield_dimensions(h_, band_path ? 1 : 0, &d.n_spins,
                                   &d.n_kpts, &d.n_states, &d.n_aos,
                                   &d.n_spinor);
    return d;
}

std::vector<std::complex<double>> Handler::compute_hartree_ao(
    const Options &opts, const std::vector<int> &iks, int n_aos)
{
    const std::size_t n = iks.size() * static_cast<std::size_t>(n_aos) * n_aos;
    std::vector<double> re(n), im(n);
    taesc_compute_hartree_ao(h_, opts.get_c_options(),
                             static_cast<int>(iks.size()), iks.data(), n_aos,
                             re.data(), im.data());
    std::vector<std::complex<double>> out(n);
    for (std::size_t i = 0; i < n; i++) out[i] = {re[i], im[i]};
    return out;
}

namespace
{
int qsgw_sink_bridge(const TaescQsgwOutput *out, void *user_data)
{
    return (*static_cast<QsgwOutputSink *>(user_data))(*out);
}
}  // namespace

TaescQsgwControls QsgwTask::default_controls()
{
    TaescQsgwControls c;
    taesc_init_qsgw_controls(&c);
    return c;
}

QsgwTask::QsgwTask(Handler &h, const Options &opts,
                   const TaescQsgwControls *controls)
    : task_(taesc_create_qsgw_task(h.get_c_handler(), opts.get_c_options(),
                                   controls))
{
}

QsgwTask::~QsgwTask()
{
    taesc_delete_qsgw_task(task_);
}

void QsgwTask::prepare()
{
    taesc_prepare_qsgw_task(task_);
}

void QsgwTask::set_input(const TaescQsgwInput *input)
{
    taesc_set_qsgw_input(task_, input);
}

void QsgwTask::set_output_sink(QsgwOutputSink sink)
{
    sink_ = std::move(sink);
    taesc_set_qsgw_output_sink(task_, sink_ ? qsgw_sink_bridge : nullptr,
                               &sink_);
}

void QsgwTask::run()
{
    taesc_run_qsgw_task(task_);
}

}  // namespace taesc
