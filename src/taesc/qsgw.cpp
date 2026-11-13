#include <taesc/taesc_qsgw.h>

#include <librpa_compute.h>
#include <librpa_input.h>
#include <librpa_options.h>

#include <vector>

#include "handler_bridge.h"
#include "options_bridge.h"

using taesc_internal::get_librpa_handler;
using taesc_internal::get_librpa_options;

struct TaescQsgwTask
{
    LibrpaQsgwTask *task = nullptr;
    TaescQsgwOutputSink sink = nullptr;
    void *user_data = nullptr;
};

namespace
{

static_assert(sizeof(TaescQsgwOutputKind) == sizeof(LibrpaQsgwOutputKind),
              "QSGW output kind enums must match");
static_assert(static_cast<int>(TAESC_QSGW_HARTREE) ==
                  static_cast<int>(LIBRPA_QSGW_HARTREE),
              "QSGW output kind enums must match");
static_assert(static_cast<int>(TAESC_QSGW_ITERATIONS) ==
                  static_cast<int>(LIBRPA_QSGW_ITERATIONS),
              "QSGW output kind enums must match");

LibrpaLocalizedOperator to_librpa(
    const TaescLocalizedOperator &op,
    std::vector<std::vector<LibrpaLocalizedBlock>> &storage)
{
    storage.emplace_back();
    auto &blocks = storage.back();
    for (int i = 0; i < op.n_blocks; i++)
    {
        const TaescLocalizedBlock &b = op.blocks[i];
        blocks.push_back({{b.translation[0], b.translation[1], b.translation[2]},
                          b.values_ri, b.row_major});
    }
    return {op.dimension, op.n_blocks, blocks.data()};
}

int sink_trampoline(const LibrpaQsgwOutput *out, void *user_data)
{
    auto *t = static_cast<TaescQsgwTask *>(user_data);
    TaescQsgwOutput o;
    o.kind = static_cast<TaescQsgwOutputKind>(out->kind);
    o.iteration = out->iteration;
    o.spin = out->spin;
    o.append = out->append;
    o.text = out->text;
    o.text_size = out->text_size;
    std::vector<TaescLocalizedBlock> blocks;
    TaescLocalizedOperator op{};
    o.hamiltonian = nullptr;
    if (out->hamiltonian != nullptr)
    {
        for (int i = 0; i < out->hamiltonian->n_blocks; i++)
        {
            const LibrpaLocalizedBlock &b = out->hamiltonian->blocks[i];
            blocks.push_back({{b.translation[0], b.translation[1], b.translation[2]},
                              b.values_ri, b.row_major});
        }
        op = {out->hamiltonian->dimension, out->hamiltonian->n_blocks,
              blocks.data()};
        o.hamiltonian = &op;
    }
    return t->sink(&o, t->user_data);
}

} // namespace

void taesc_init_qsgw_controls(TaescQsgwControls *controls)
{
    LibrpaQsgwControls c;
    librpa_init_qsgw_controls(&c);
    *controls = {c.min_iterations, c.max_iterations, c.unoccupied_keep,
                 c.cut_mode, c.mixing_beta, c.cut_shift_ha,
                 c.convergence_tolerance_ev, c.mixer, c.velocity_mode,
                 c.restart_file, c.checkpoint_file, c.output_matrices,
                 c.output_qp, c.export_hamiltonian, c.state_low, c.state_high,
                 c.interband_velocity, c.expand_ibz, c.update_hartree};
}

TaescQsgwTask *taesc_create_qsgw_task(TaescHandler *h,
                                      const TaescOptions *p_opts,
                                      const TaescQsgwControls *controls)
{
    LibrpaQsgwControls c;
    librpa_init_qsgw_controls(&c);
    if (controls != nullptr)
    {
        c.min_iterations = controls->min_iterations;
        c.max_iterations = controls->max_iterations;
        c.unoccupied_keep = controls->unoccupied_keep;
        c.cut_mode = controls->cut_mode;
        c.mixing_beta = controls->mixing_beta;
        c.cut_shift_ha = controls->cut_shift_ha;
        c.convergence_tolerance_ev = controls->convergence_tolerance_ev;
        if (controls->mixer) c.mixer = controls->mixer;
        if (controls->velocity_mode) c.velocity_mode = controls->velocity_mode;
        if (controls->restart_file) c.restart_file = controls->restart_file;
        if (controls->checkpoint_file) c.checkpoint_file = controls->checkpoint_file;
        c.output_matrices = controls->output_matrices;
        c.output_qp = controls->output_qp;
        c.export_hamiltonian = controls->export_hamiltonian;
        c.state_low = controls->state_low;
        c.state_high = controls->state_high;
        c.interband_velocity = controls->interband_velocity;
        c.expand_ibz = controls->expand_ibz;
        c.update_hartree = controls->update_hartree;
    }
    LibrpaQsgwTask *inner = librpa_create_qsgw_task(
        get_librpa_handler(h), get_librpa_options(p_opts), &c);
    auto *t = new TaescQsgwTask;
    t->task = inner;
    return t;
}

void taesc_prepare_qsgw_task(TaescQsgwTask *task)
{
    librpa_prepare_qsgw_task(task->task);
}

void taesc_set_qsgw_input(TaescQsgwTask *task, const TaescQsgwInput *in)
{
    // Buffers are significant on handler rank zero only. Copy the descriptors,
    // not the numerical data, because LibRPA copies the data in the call.
    std::vector<LibrpaQsgwMatrix> vxc, vxc_band;
    std::vector<LibrpaQsgwLriBlock> lri;
    std::vector<std::vector<LibrpaLocalizedBlock>> storage;
    std::vector<LibrpaLocalizedOperator> hamiltonians;
    LibrpaNativeVelocityInput nat{};
    LibrpaQsgwInput out{};

    if (in != nullptr)
    {
        for (int i = 0; i < in->n_vxc; i++)
            vxc.push_back({in->vxc[i].spin, in->vxc[i].kpoint,
                           in->vxc[i].dimension, in->vxc[i].row_major,
                           in->vxc[i].values_ri});
        for (int i = 0; i < in->n_vxc_band; i++)
            vxc_band.push_back({in->vxc_band[i].spin, in->vxc_band[i].kpoint,
                                in->vxc_band[i].dimension,
                                in->vxc_band[i].row_major,
                                in->vxc_band[i].values_ri});
        for (int i = 0; i < in->n_hartree_lri_blocks; i++)
        {
            const TaescQsgwLriBlock &b = in->hartree_lri_blocks[i];
            lri.push_back({b.atom_i, b.atom_j,
                           {b.translation[0], b.translation[1], b.translation[2]},
                           b.n_aos_i, b.n_aos_j, b.n_aux_i, b.values});
        }
        out.n_vxc = in->n_vxc;
        out.n_vxc_band = in->n_vxc_band;
        out.vxc = vxc.data();
        out.vxc_band = vxc_band.data();
        out.vxc_in_ao_basis = in->vxc_in_ao_basis;
        out.inplane_area_bohr2 = in->inplane_area_bohr2;
        out.auxiliary_monopole_norm_squared = in->auxiliary_monopole_norm_squared;
        out.hartree_n_aux = in->hartree_n_aux;
        out.hartree_coulomb_q0_ri = in->hartree_coulomb_q0_ri;
        out.n_hartree_lri_blocks = in->n_hartree_lri_blocks;
        out.hartree_lri_blocks = lri.data();
        if (in->native_operators != nullptr)
        {
            const TaescNativeVelocityInput *n = in->native_operators;
            storage.reserve(n->n_spins + 4);
            hamiltonians.reserve(n->n_spins);
            for (int i = 0; i < n->n_spins; i++)
                hamiltonians.push_back(to_librpa(n->hamiltonian[i], storage));
            nat.n_spins = n->n_spins;
            nat.hamiltonian = hamiltonians.data();
            nat.overlap = to_librpa(n->overlap, storage);
            for (int i = 0; i < 3; i++)
                nat.position[i] = to_librpa(n->position[i], storage);
            nat.inplane_area_bohr2 = n->inplane_area_bohr2;
            nat.auxiliary_monopole_norm_squared =
                n->auxiliary_monopole_norm_squared;
            out.native_operators = &nat;
        }
    }
    librpa_set_qsgw_input(task->task, in != nullptr ? &out : nullptr);
}

void taesc_set_qsgw_output_sink(TaescQsgwTask *task, TaescQsgwOutputSink sink,
                                void *user_data)
{
    task->sink = sink;
    task->user_data = user_data;
    if (sink != nullptr)
        librpa_set_qsgw_output_sink(task->task, sink_trampoline, task);
    else
        librpa_set_qsgw_output_sink(task->task, nullptr, nullptr);
}

void taesc_run_qsgw_task(TaescQsgwTask *task)
{
    librpa_run_qsgw_task(task->task);
}

void taesc_delete_qsgw_task(TaescQsgwTask *task)
{
    if (task == nullptr) return;
    librpa_delete_qsgw_task(task->task);
    delete task;
}

void taesc_compute_hartree_ao(TaescHandler *h, const TaescOptions *p_opts,
                              int n_kpoints, const int *kpoints, int n_aos,
                              double *values_re, double *values_im)
{
    librpa_compute_hartree_ao(get_librpa_handler(h), get_librpa_options(p_opts),
                              n_kpoints, kpoints, n_aos, values_re, values_im);
}

void taesc_get_meanfield_dimensions(const TaescHandler *h, int band_path,
                                    int *nspins, int *nkpts, int *nstates,
                                    int *naos, int *nspinor)
{
    librpa_get_meanfield_dimensions(get_librpa_handler(h), band_path, nspins,
                                    nkpts, nstates, naos, nspinor);
}
