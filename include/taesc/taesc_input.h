#pragma once
/**
 * @file taesc_input.h
 * @brief TAESC input-data facade for LibRPA.
 */

#include <stddef.h>

#include "taesc_enums.h"
#include "taesc_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void taesc_set_scf_dimension(TaescHandler *h, int nspins, int nkpts,
                             int nstates, int nbasis, int nspinor);

void taesc_set_wg_ekb_efermi(TaescHandler *h, int nspins, int nkpts,
                             int nstates, const double *wg, const double *ekb,
                             double efermi);

void taesc_set_wfc(TaescHandler *h, int ispin, int ik, int nstates_local,
                   int nbasis_local, const double *wfc_real,
                   const double *wfc_imag);

void taesc_set_wfc_spinor(TaescHandler *h, int ik, int nstates_local,
                          int nbasis_local, const double *wfc_up_real,
                          const double *wfc_up_imag,
                          const double *wfc_dn_real,
                          const double *wfc_dn_imag);

void taesc_set_wfc_packed(TaescHandler *h, int ispin, int ik, int nstates_local,
                          int nbasis_local, const double *wfc_ri);

void taesc_set_wfc_spinor_packed(TaescHandler *h, int ik, int nstates_local,
                                 int nbasis_local, const double *wfc_up_ri,
                                 const double *wfc_dn_ri);

void taesc_set_ao_basis_wfc(TaescHandler *h, int natoms,
                            const size_t *nbs_wfc, const int *nshells,
                            const int *l_shells);

void taesc_set_ao_basis_aux(TaescHandler *h, int natoms,
                            const size_t *nbs_aux, const int *nshells,
                            const int *l_shells);

void taesc_set_ao_basis_aux_shrink(TaescHandler *h, int natoms,
                                   const size_t *nbs_aux_shrink,
                                   const int *nshells,
                                   const int *l_shells);

void taesc_set_basis_convention(TaescHandler *h, int bloch_phase,
                                int bloch_ratom, TaescAngularOrder order,
                                TaescRshCoeff nega_m, TaescRshCoeff posi_m);

void taesc_set_symmetry_operations(TaescHandler *h, int n_symops,
                                   int row_conv, const int *rotmats,
                                   const double *trans);

void taesc_set_latvec_and_G(TaescHandler *h, const double lat_mat[9],
                            const double G_mat[9]);

void taesc_set_atoms(TaescHandler *h, int natoms, const int *types,
                     const double *pos_cart);

void taesc_set_kgrids_kvec(TaescHandler *h, int nk1, int nk2, int nk3,
                           int nkpts, const double *kvecs,
                           const double *kweights);

void taesc_set_kq_mapping(TaescHandler *h, int nkpts, const int *map_q_ks);

void taesc_set_lri_coeff(TaescHandler *h, int I, int J, int nbasis_i,
                         int nbasis_j, int naux_mu, const int R[3],
                         const double *Cs_in, int shrink_aux);

void taesc_set_aux_bare_coulomb_k_atom_pair(TaescHandler *h, int ik, int I,
                                            int J, int naux_mu, int naux_nu,
                                            const double *Vq_real_in,
                                            const double *Vq_imag_in,
                                            double vq_threshold);

void taesc_set_aux_bare_coulomb_k_atom_pair_packed(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_ri_in, double vq_threshold);

void taesc_set_aux_cut_coulomb_k_atom_pair(TaescHandler *h, int ik, int I,
                                           int J, int naux_mu, int naux_nu,
                                           const double *Vq_real_in,
                                           const double *Vq_imag_in,
                                           double vq_threshold);

void taesc_set_aux_cut_coulomb_k_atom_pair_packed(
    TaescHandler *h, int ik, int I, int J, int naux_mu, int naux_nu,
    const double *Vq_ri_in, double vq_threshold);

void taesc_set_aux_bare_coulomb_k_2d_block(TaescHandler *h, int ik,
                                           int mu_begin, int mu_end,
                                           int nu_begin, int nu_end,
                                           const double *Vq_real_in,
                                           const double *Vq_imag_in);

void taesc_set_aux_bare_coulomb_k_2d_block_packed(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_ri_in);

void taesc_set_aux_cut_coulomb_k_2d_block(TaescHandler *h, int ik, int mu_begin,
                                          int mu_end, int nu_begin, int nu_end,
                                          const double *Vq_real_in,
                                          const double *Vq_imag_in);

void taesc_set_aux_cut_coulomb_k_2d_block_packed(
    TaescHandler *h, int ik, int mu_begin, int mu_end, int nu_begin,
    int nu_end, const double *Vq_ri_in);

void taesc_set_dielect_func_imagfreq(TaescHandler *h, int nfreq,
                                     const double *omegas_imag,
                                     const double *dielect_func);

void taesc_set_velocity_matrix(TaescHandler *h, int n_spins, int n_kpts,
                               int n_states, const double *velocity_real,
                               const double *velocity_imag);

void taesc_set_velocity_matrix_packed(TaescHandler *h, int n_spins,
                                      int n_kpts, int n_states,
                                      const double *velocity_ri);

void taesc_set_band_kvec(TaescHandler *h, int n_kpts_band,
                         const double *kfrac_band);

void taesc_set_band_occ_eigval(TaescHandler *h, int n_spins, int n_kpts_band,
                               int n_states, const double *occ,
                               const double *eig);

void taesc_set_wfc_band(TaescHandler *h, int ispin, int ik_band,
                        int nstates_local, int nbasis_local,
                        const double *wfc_real, const double *wfc_imag);

void taesc_set_wfc_band_spinor(TaescHandler *h, int ik_band,
                               int nstates_local, int nbasis_local,
                               const double *wfc_up_real,
                               const double *wfc_up_imag,
                               const double *wfc_dn_real,
                               const double *wfc_dn_imag);

void taesc_set_wfc_band_packed(TaescHandler *h, int ispin, int ik_band,
                               int nstates_local, int nbasis_local,
                               const double *wfc_ri);

void taesc_set_wfc_band_spinor_packed(TaescHandler *h, int ik_band,
                                      int nstates_local, int nbasis_local,
                                      const double *wfc_up_ri,
                                      const double *wfc_dn_ri);

void taesc_reset_band_data(TaescHandler *h);

#ifdef __cplusplus
}
#endif
