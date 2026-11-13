#pragma once
/**
 * @file taesc_qsgw.h
 * @brief TAESC facade of the LibRPA QSGW task, periodic Hartree AO matrices
 *        and mean-field dimension query.
 *
 * Conventions follow LibRPA: energies are Hartree unless stated, indices are
 * zero based, complex arrays are interleaved [real, imag]. The structures
 * below mirror the LibRPA ones. All pointers are borrowed and must stay valid
 * until the call that receives them returns.
 */

#include <stddef.h>

#include "taesc_handler.h"
#include "taesc_options.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TaescQsgwTask TaescQsgwTask;

/** QSGW runtime controls. See taesc_init_qsgw_controls for the defaults.
 *  The strings are copied when the task is created. */
typedef struct
{
    int min_iterations, max_iterations, unoccupied_keep, cut_mode;
    double mixing_beta, cut_shift_ha, convergence_tolerance_ev;
    const char *mixer, *velocity_mode, *restart_file, *checkpoint_file;
    int output_matrices, output_qp, export_hamiltonian;
    /** Zero-based state window [state_low, state_high). */
    int state_low, state_high, interband_velocity;
    int expand_ibz;
    int update_hartree;
} TaescQsgwControls;

/** Fill the controls with the defaults of the underlying LibRPA version. */
void taesc_init_qsgw_controls(TaescQsgwControls *controls);

/** One square matrix of one spin and k point. row_major is 0 or 1. */
typedef struct
{
    int spin, kpoint, dimension, row_major;
    const double *values_ri;
} TaescQsgwMatrix;

/** One directed, unwrapped LRI coefficient block of the Hartree input. */
typedef struct
{
    int atom_i, atom_j, translation[3];
    int n_aos_i, n_aos_j, n_aux_i;
    const double *values;
} TaescQsgwLriBlock;

typedef struct
{
    int translation[3];
    const double *values_ri;
    int row_major;
} TaescLocalizedBlock;

typedef struct
{
    int dimension;
    int n_blocks;
    const TaescLocalizedBlock *blocks;
} TaescLocalizedOperator;

/** Original H/S/r operators for native velocities (Hartree, Bohr). */
typedef struct
{
    int n_spins;
    const TaescLocalizedOperator *hamiltonian; /* n_spins entries */
    TaescLocalizedOperator overlap;
    TaescLocalizedOperator position[3];
    double inplane_area_bohr2;
    double auxiliary_monopole_norm_squared;
} TaescNativeVelocityInput;

/** Host input of a QSGW task. Supplied on handler rank zero only. Set unused
 *  members to zero or NULL (initialize with memset or `= {0}`). */
typedef struct
{
    int n_vxc, n_vxc_band;
    const TaescQsgwMatrix *vxc, *vxc_band;
    int vxc_in_ao_basis;
    const TaescNativeVelocityInput *native_operators;
    double inplane_area_bohr2, auxiliary_monopole_norm_squared;
    int hartree_n_aux;
    const double *hartree_coulomb_q0_ri;
    int n_hartree_lri_blocks;
    const TaescQsgwLriBlock *hartree_lri_blocks;
} TaescQsgwInput;

typedef enum
{
    TAESC_QSGW_ITERATIONS,
    TAESC_QSGW_EIGENVALUES,
    TAESC_QSGW_MATRICES,
    TAESC_QSGW_QUASIPARTICLES,
    TAESC_QSGW_ENERGY_QP,
    TAESC_QSGW_GW_BAND,
    TAESC_QSGW_KS_BAND,
    TAESC_QSGW_EXX_BAND,
    TAESC_QSGW_QSGW_BAND,
    TAESC_QSGW_HAMILTONIAN,
    TAESC_QSGW_HEAD_HAMILTONIAN,
    TAESC_QSGW_HARTREE
} TaescQsgwOutputKind;

/** Borrowed output on rank zero, valid during the callback only. */
typedef struct
{
    TaescQsgwOutputKind kind;
    int iteration, spin, append;
    const char *text;
    size_t text_size;
    /** Present for Hamiltonian-like outputs, otherwise NULL. Same layout as
     *  the LibRPA operator; valid during the callback only. */
    const TaescLocalizedOperator *hamiltonian;
} TaescQsgwOutput;

/** Return zero on success. The callback must not use the handler or task. */
typedef int (*TaescQsgwOutputSink)(const TaescQsgwOutput *, void *user_data);

/** The handler must outlive the task. Collective on the handler communicator. */
TaescQsgwTask *taesc_create_qsgw_task(TaescHandler *h, const TaescOptions *p_opts,
                                      const TaescQsgwControls *controls);
void taesc_prepare_qsgw_task(TaescQsgwTask *task);
void taesc_set_qsgw_input(TaescQsgwTask *task, const TaescQsgwInput *root_input);
void taesc_set_qsgw_output_sink(TaescQsgwTask *task, TaescQsgwOutputSink sink,
                                void *user_data);
void taesc_run_qsgw_task(TaescQsgwTask *task);
void taesc_delete_qsgw_task(TaescQsgwTask *task);

/** Physical spin-summed periodic Hartree potential in the AO basis (Hartree).
 *  Output layout: row-major [requested k][AO row][AO column]; each buffer
 *  holds n_kpoints*n_aos*n_aos doubles. kpoints are zero-based. */
void taesc_compute_hartree_ao(TaescHandler *h, const TaescOptions *p_opts,
                              int n_kpoints, const int *kpoints, int n_aos,
                              double *values_re, double *values_im);

/** Noncollective query of the mean-field dimensions. band_path != 0 selects
 *  the band input. */
void taesc_get_meanfield_dimensions(const TaescHandler *h, int band_path,
                                    int *nspins, int *nkpts, int *nstates,
                                    int *naos, int *nspinor);

#ifdef __cplusplus
}
#endif
