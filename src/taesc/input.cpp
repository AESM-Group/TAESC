#include <taesc/taesc_input.h>

#include <librpa_input.h>

#include "handler_bridge.h"

using taesc_internal::get_librpa_handler;

void taesc_set_scf_dimension(TaescHandler *h, int nspins, int nkpts,
                             int nstates, int nbasis)
{
    librpa_set_scf_dimension(get_librpa_handler(h), nspins, nkpts, nstates, nbasis);
}

void taesc_set_wg_ekb_efermi(TaescHandler *h, int nspins, int nkpts,
                             int nstates, const double *wg,
                             const double *ekb, double efermi)
{
    librpa_set_wg_ekb_efermi(get_librpa_handler(h), nspins, nkpts, nstates, wg, ekb,
                              efermi);
}

void taesc_set_wfc(TaescHandler *h, int ispin, int ik, int nstates_local,
                   int nbasis_local, const double *wfc_real,
                   const double *wfc_imag)
{
    librpa_set_wfc(get_librpa_handler(h), ispin, ik, nstates_local, nbasis_local,
                   wfc_real, wfc_imag);
}

void taesc_set_wfc_packed(TaescHandler *h, int ispin, int ik,
                          int nstates_local, int nbasis_local,
                          const double *wfc_ri)
{
    librpa_set_wfc_packed(get_librpa_handler(h), ispin, ik, nstates_local,
                           nbasis_local, wfc_ri);
}

void taesc_set_ao_basis_wfc(TaescHandler *h, int natoms,
                            const size_t *nbs_wfc)
{
    librpa_set_ao_basis_wfc(get_librpa_handler(h), natoms, nbs_wfc);
}

void taesc_set_ao_basis_aux(TaescHandler *h, int natoms,
                            const size_t *nbs_aux)
{
    librpa_set_ao_basis_aux(get_librpa_handler(h), natoms, nbs_aux);
}

void taesc_set_latvec_and_G(TaescHandler *h, const double lat_mat[9],
                            const double G_mat[9])
{
    librpa_set_latvec_and_G(get_librpa_handler(h), lat_mat, G_mat);
}

void taesc_set_atoms(TaescHandler *h, int natoms, const int *types,
                     const double *pos_cart)
{
    librpa_set_atoms(get_librpa_handler(h), natoms, types, pos_cart);
}

void taesc_set_kgrids_kvec(TaescHandler *h, int nk1, int nk2, int nk3,
                           const double *kvecs)
{
    librpa_set_kgrids_kvec(get_librpa_handler(h), nk1, nk2, nk3, kvecs);
}

void taesc_set_ibz_mapping(TaescHandler *h, int nkpts,
                           const int *map_ibzk)
{
    librpa_set_ibz_mapping(get_librpa_handler(h), nkpts, map_ibzk);
}

void taesc_set_lri_coeff(TaescHandler *h, TaescParallelRouting routing,
                         int I, int J, int nbasis_i, int nbasis_j,
                         int naux_mu, const int R[3], const double *Cs_in)
{
    librpa_set_lri_coeff(get_librpa_handler(h),
                         taesc_to_librpa_parallel_routing(routing), I, J,
                         nbasis_i, nbasis_j, naux_mu, R, Cs_in);
}

void taesc_set_aux_bare_coulomb_k_atom_pair(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    librpa_set_aux_bare_coulomb_k_atom_pair(
        get_librpa_handler(h), ik, I, J, naux_mu, naux_nu, Vq_real_in,
        Vq_imag_in, vq_threshold);
}

void taesc_set_aux_cut_coulomb_k_atom_pair(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    librpa_set_aux_cut_coulomb_k_atom_pair(
        get_librpa_handler(h), ik, I, J, naux_mu, naux_nu, Vq_real_in,
        Vq_imag_in, vq_threshold);
}

void taesc_set_aux_bare_coulomb_k_2d_block(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_real_in, const double *Vq_imag_in)
{
    librpa_set_aux_bare_coulomb_k_2d_block(get_librpa_handler(h), ik, mu_begin, mu_end,
                                           nu_begin, nu_end,
                                           Vq_real_in, Vq_imag_in);
}

void taesc_set_aux_cut_coulomb_k_2d_block(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_real_in, const double *Vq_imag_in)
{
    librpa_set_aux_cut_coulomb_k_2d_block(get_librpa_handler(h), ik, mu_begin, mu_end,
                                          nu_begin, nu_end,
                                          Vq_real_in, Vq_imag_in);
}

void taesc_set_dielect_func_imagfreq(TaescHandler *h, int nfreq,
                                     const double *omegas_imag,
                                     const double *dielect_func)
{
    librpa_set_dielect_func_imagfreq(get_librpa_handler(h), nfreq, omegas_imag,
                                      dielect_func);
}

void taesc_set_band_kvec(TaescHandler *h, int n_kpts_band,
                         const double *kfrac_band)
{
    librpa_set_band_kvec(get_librpa_handler(h), n_kpts_band, kfrac_band);
}

void taesc_set_band_occ_eigval(TaescHandler *h, int n_spins,
                               int n_kpts_band, int n_states,
                               const double *occ, const double *eig)
{
    librpa_set_band_occ_eigval(get_librpa_handler(h), n_spins, n_kpts_band,
                                n_states, occ, eig);
}

void taesc_set_wfc_band(TaescHandler *h, int ispin, int ik_band,
                        int nstates_local, int nbasis_local,
                        const double *wfc_real, const double *wfc_imag)
{
    librpa_set_wfc_band(get_librpa_handler(h), ispin, ik_band, nstates_local,
                         nbasis_local, wfc_real, wfc_imag);
}

void taesc_set_wfc_band_packed(TaescHandler *h, int ispin, int ik_band,
                               int nstates_local, int nbasis_local,
                               const double *wfc_ri)
{
    librpa_set_wfc_band_packed(get_librpa_handler(h), ispin, ik_band,
                                nstates_local, nbasis_local, wfc_ri);
}

void taesc_reset_band_data(TaescHandler *h)
{
    librpa_reset_band_data(get_librpa_handler(h));
}
