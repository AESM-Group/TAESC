#include "taesc/taesc.hpp"

#include "taesc/taesc_compute.h"
#include "taesc/taesc_global.h"
#include "taesc/taesc_input.h"
#include "taesc/taesc_options.h"

#include "options_bridge.h"

#include <stdexcept>

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

void init_global()
{
    taesc_init_global();
}

void finalize_global(void)
{
    taesc_finalize_global();
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

void Handler::set_scf_dimension(int nspins, int nkpts, int nstates, int nbasis)
{
    taesc_set_scf_dimension(h_, nspins, nkpts, nstates, nbasis);
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

void Handler::set_ao_basis_wfc(const std::vector<std::size_t> &nbs_wfc)
{
    taesc_set_ao_basis_wfc(h_, static_cast<int>(nbs_wfc.size()),
                           nbs_wfc.data());
}

void Handler::set_ao_basis_aux(const std::vector<std::size_t> &nbs_aux)
{
    taesc_set_ao_basis_aux(h_, static_cast<int>(nbs_aux.size()),
                           nbs_aux.data());
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

void Handler::set_kgrids_kvec(int nk1, int nk2, int nk3, const double *kvecs)
{
    taesc_set_kgrids_kvec(h_, nk1, nk2, nk3, kvecs);
}

void Handler::set_ibz_mapping(const std::vector<int> &map_ibzk)
{
    taesc_set_ibz_mapping(h_, static_cast<int>(map_ibzk.size()),
                          map_ibzk.data());
}

void Handler::set_lri_coeff(int I, int J,
                            int nbasis_i, int nbasis_j, int naux_mu,
                            const int R[3], const double *Cs_in)
{
    taesc_set_lri_coeff(h_, I, J, nbasis_i, nbasis_j,
                        naux_mu, R, Cs_in);
}

void Handler::set_aux_bare_coulomb_k_atom_pair(
    int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    taesc_set_aux_bare_coulomb_k_atom_pair(h_, ik, I, J, naux_mu, naux_nu,
                                           Vq_real_in, Vq_imag_in,
                                           vq_threshold);
}

void Handler::set_aux_cut_coulomb_k_atom_pair(
    int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    taesc_set_aux_cut_coulomb_k_atom_pair(h_, ik, I, J, naux_mu, naux_nu,
                                          Vq_real_in, Vq_imag_in,
                                          vq_threshold);
}

void Handler::set_aux_bare_coulomb_k_2d_block(
    int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
    const double *Vq_real_in, const double *Vq_imag_in)
{
    taesc_set_aux_bare_coulomb_k_2d_block(h_, ik, mu_begin, mu_end,
                                          nu_begin, nu_end,
                                          Vq_real_in, Vq_imag_in);
}

void Handler::set_aux_cut_coulomb_k_2d_block(
    int ik, int mu_begin, int mu_end, int nu_begin, int nu_end,
    const double *Vq_real_in, const double *Vq_imag_in)
{
    taesc_set_aux_cut_coulomb_k_2d_block(h_, ik, mu_begin, mu_end,
                                         nu_begin, nu_end,
                                         Vq_real_in, Vq_imag_in);
}

void Handler::set_dielect_func_imagfreq(
    const std::vector<double> &omegas_imag,
    const std::vector<double> &dielect_func)
{
    taesc_set_dielect_func_imagfreq(h_, static_cast<int>(omegas_imag.size()),
                                    omegas_imag.data(), dielect_func.data());
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

void Handler::set_wfc_band_packed(int ispin, int ik_band, int nstates_local,
                                  int nbasis_local,
                                  const std::complex<double> *wfc)
{
    taesc_set_wfc_band_packed(h_, ispin, ik_band, nstates_local,
                              nbasis_local,
                              reinterpret_cast<const double*>(wfc));
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

}  // namespace taesc
