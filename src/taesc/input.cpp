#include <taesc/taesc_input.h>

#include <librpa_input.h>

#include "handler_bridge.h"

using taesc_internal::get_librpa_handler;

void taesc_set_scf_dimension(TaescHandler *h, int nspins, int nkpts,
                             int nstates, int nbasis, int nspinor)
{
    librpa_set_scf_dimension(get_librpa_handler(h), nspins, nkpts, nstates,
                             nbasis, nspinor);
}

void taesc_set_wfc_spinor(TaescHandler *h, int ik, int nstates_local,
                          int nbasis_local, const double *wfc_up_real,
                          const double *wfc_up_imag,
                          const double *wfc_dn_real,
                          const double *wfc_dn_imag)
{
    librpa_set_wfc_spinor(get_librpa_handler(h), ik, nstates_local,
                          nbasis_local, wfc_up_real, wfc_up_imag,
                          wfc_dn_real, wfc_dn_imag);
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

void taesc_set_wfc_spinor_packed(TaescHandler *h, int ik,
                                 int nstates_local, int nbasis_local,
                                 const double *wfc_up_ri,
                                 const double *wfc_dn_ri)
{
    librpa_set_wfc_spinor_packed(get_librpa_handler(h), ik, nstates_local,
                                 nbasis_local, wfc_up_ri, wfc_dn_ri);
}

void taesc_set_ao_basis_wfc(TaescHandler *h, int natoms,
                            const size_t *nbs_wfc, const int *nshells,
                            const int *l_shells)
{
    librpa_set_ao_basis_wfc(get_librpa_handler(h), natoms, nbs_wfc,
                            nshells, l_shells);
}

void taesc_set_ao_basis_aux(TaescHandler *h, int natoms,
                            const size_t *nbs_aux, const int *nshells,
                            const int *l_shells)
{
    librpa_set_ao_basis_aux(get_librpa_handler(h), natoms, nbs_aux,
                            nshells, l_shells);
}

void taesc_set_ao_basis_aux_shrink(TaescHandler *h, int natoms,
                                   const size_t *nbs_aux_shrink,
                                   const int *nshells,
                                   const int *l_shells)
{
    librpa_set_ao_basis_aux_shrink(get_librpa_handler(h), natoms,
                                   nbs_aux_shrink, nshells, l_shells);
}

void taesc_set_basis_convention(TaescHandler *h, int bloch_phase,
                                int bloch_ratom, TaescAngularOrder order,
                                TaescRshCoeff nega_m, TaescRshCoeff posi_m)
{
    librpa_set_basis_convention(
        get_librpa_handler(h), bloch_phase, bloch_ratom,
        static_cast<LibrpaAngularOrder>(order),
        static_cast<LibrpaRshCoeff>(nega_m),
        static_cast<LibrpaRshCoeff>(posi_m));
}

void taesc_set_symmetry_operations(TaescHandler *h, int n_symops,
                                   int row_conv, const int *rotmats,
                                   const double *trans)
{
    librpa_set_symmetry_operations(get_librpa_handler(h), n_symops,
                                   row_conv, rotmats, trans);
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
                           int nkpts, const double *kvecs,
                           const double *kweights)
{
    librpa_set_kgrids_kvec(get_librpa_handler(h), nk1, nk2, nk3, nkpts,
                           kvecs, kweights);
}

void taesc_set_kq_mapping(TaescHandler *h, int nkpts, const int *map_q_ks)
{
    librpa_set_kq_mapping(get_librpa_handler(h), nkpts, map_q_ks);
}

void taesc_set_lri_coeff(TaescHandler *h, int I, int J, int nbasis_i, int nbasis_j,
                         int naux_mu, const int R[3], const double *Cs_in,
                         int shrink_aux)
{
    librpa_set_lri_coeff(get_librpa_handler(h), LIBRPA_ROUTING_LIBRI, I, J,
                         nbasis_i, nbasis_j, naux_mu, R, Cs_in, shrink_aux);
}

void taesc_set_aux_bare_coulomb_k_atom_pair_packed(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_ri_in, double vq_threshold)
{
    librpa_set_aux_bare_coulomb_k_atom_pair_packed(
        get_librpa_handler(h), ik, I, J, naux_mu, naux_nu, Vq_ri_in,
        vq_threshold);
}

void taesc_set_aux_bare_coulomb_k_atom_pair(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    librpa_set_aux_bare_coulomb_k_atom_pair(
        get_librpa_handler(h), ik, I, J, naux_mu, naux_nu, Vq_real_in,
        Vq_imag_in, vq_threshold);
}

void taesc_set_aux_cut_coulomb_k_atom_pair_packed(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_ri_in, double vq_threshold)
{
    librpa_set_aux_cut_coulomb_k_atom_pair_packed(
        get_librpa_handler(h), ik, I, J, naux_mu, naux_nu, Vq_ri_in,
        vq_threshold);
}

void taesc_set_aux_cut_coulomb_k_atom_pair(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_real_in, const double *Vq_imag_in, double vq_threshold)
{
    librpa_set_aux_cut_coulomb_k_atom_pair(
        get_librpa_handler(h), ik, I, J, naux_mu, naux_nu, Vq_real_in,
        Vq_imag_in, vq_threshold);
}

void taesc_set_aux_bare_coulomb_k_2d_block_packed(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_ri_in)
{
    librpa_set_aux_bare_coulomb_k_2d_block_packed(
        get_librpa_handler(h), ik, mu_begin, mu_end, nu_begin, nu_end,
        Vq_ri_in);
}

void taesc_set_aux_bare_coulomb_k_2d_block(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_real_in, const double *Vq_imag_in)
{
    librpa_set_aux_bare_coulomb_k_2d_block(get_librpa_handler(h), ik, mu_begin, mu_end,
                                           nu_begin, nu_end,
                                           Vq_real_in, Vq_imag_in);
}

void taesc_set_aux_cut_coulomb_k_2d_block_packed(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_ri_in)
{
    librpa_set_aux_cut_coulomb_k_2d_block_packed(
        get_librpa_handler(h), ik, mu_begin, mu_end, nu_begin, nu_end,
        Vq_ri_in);
}

void taesc_set_aux_cut_coulomb_k_2d_block(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_real_in, const double *Vq_imag_in)
{
    librpa_set_aux_cut_coulomb_k_2d_block(get_librpa_handler(h), ik, mu_begin, mu_end,
                                          nu_begin, nu_end,
                                          Vq_real_in, Vq_imag_in);
}

void taesc_set_velocity_matrix(TaescHandler *h, int n_spins, int n_kpts,
                               int n_states, const double *velocity_real,
                               const double *velocity_imag)
{
    librpa_set_velocity_matrix(get_librpa_handler(h), n_spins, n_kpts,
                               n_states, velocity_real, velocity_imag);
}

void taesc_set_velocity_matrix_packed(TaescHandler *h, int n_spins,
                                      int n_kpts, int n_states,
                                      const double *velocity_ri)
{
    librpa_set_velocity_matrix_packed(get_librpa_handler(h), n_spins,
                                      n_kpts, n_states, velocity_ri);
}

void taesc_set_dielect_func_imagfreq(TaescHandler *h, int nfreq,
                                     const double *omegas_imag,
                                     const double *dielect_func)
{
    librpa_set_dielect_func_imagfreq(get_librpa_handler(h), nfreq, omegas_imag,
                                      dielect_func);
}

void taesc_set_wfc_band_spinor(TaescHandler *h, int ik_band,
                               int nstates_local, int nbasis_local,
                               const double *wfc_up_real,
                               const double *wfc_up_imag,
                               const double *wfc_dn_real,
                               const double *wfc_dn_imag)
{
    librpa_set_wfc_band_spinor(get_librpa_handler(h), ik_band,
                               nstates_local, nbasis_local, wfc_up_real,
                               wfc_up_imag, wfc_dn_real, wfc_dn_imag);
}

void taesc_set_band_kvec(TaescHandler *h, int n_kpts_band,
                         const double *kfrac_band)
{
    librpa_set_band_kvec(get_librpa_handler(h), n_kpts_band, kfrac_band);
}

void taesc_set_wfc_band_spinor_packed(TaescHandler *h, int ik_band,
                                      int nstates_local, int nbasis_local,
                                      const double *wfc_up_ri,
                                      const double *wfc_dn_ri)
{
    librpa_set_wfc_band_spinor_packed(get_librpa_handler(h), ik_band,
                                      nstates_local, nbasis_local,
                                      wfc_up_ri, wfc_dn_ri);
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
