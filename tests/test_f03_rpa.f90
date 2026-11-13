! Smoke test of the Fortran host interface: feed a small model system to
! TaescHandler, run RPA and check that the new options are accepted and take
! effect. The model system is the one of the LibRPA Fortran binding test.
program test_f03_rpa
   use mpi
   use taesc_f03
   implicit none

   integer, parameter :: nspins = 1
   integer, parameter :: nk1 = 3, nk2 = 1, nk3 = 1
   integer, parameter :: nkpts = nk1 * nk2 * nk3
   integer, parameter :: nstates = 3
   integer, parameter :: nbasis = nstates
   integer, parameter :: naux = 9
   integer, parameter :: natoms = 2
   integer, parameter :: nfreq = 8
   real(dp), parameter :: pi = atan(1.0_dp) * 4.0_dp

   integer :: i, ierr, ispin, ik, ik1, istate, ia1, ia2, r(3), provided
   integer :: nfail
   type(TaescHandler) :: h
   type(TaescOptions) :: opts
   real(dp), allocatable :: wg(:,:,:), ekb(:,:,:), ri_coeff(:,:,:), omegas(:), df(:)
   real(dp) :: latt(3, 3), recplatt(3, 3), posi_cart(3, natoms), kpoints(3, nkpts)
   real(dp) :: kweights(nkpts)
   integer :: nbs_wfc(natoms), nbs_aux(natoms), types(natoms), map_q_ks(nkpts)
   complex(dp) :: wfc(nbasis, nstates)
   complex(dp), allocatable :: vq(:,:), contrib_ibzk(:)
   real(dp) :: efermi, e_default, e_noop, e_exclude

   nfail = 0
   call MPI_Init_thread(MPI_THREAD_MULTIPLE, provided, ierr)
   call taesc_init_global(.true., "output.txt", .true.)

   write(*,*) "TAESC version:", taesc_get_major_version(), &
                                taesc_get_minor_version(), &
                                taesc_get_patch_version()

   call opts%init()
   call opts%set("output_dir", "taesc_f03.d")
   call opts%set("parallel_routing", "libri")
   call opts%set("tfgrids_type", "minimax")
   call opts%set("nfreq", nfreq)
   call opts%set("use_scalapack_ecrpa", .true.)

   call h%init(MPI_COMM_WORLD)

   call h%set_scf_dimension(nspins, nkpts, nstates, nbasis)
   block
      integer :: md_ns, md_nk, md_nst, md_nao, md_nsp
      type(TaescQsgwControls) :: qctl
      call h%get_meanfield_dimensions(md_ns, md_nk, md_nst, md_nao, md_nsp)
      if (md_ns /= nspins .or. md_nk /= nkpts .or. md_nst /= nstates .or. &
          md_nao /= nbasis .or. md_nsp /= 1) then
         write(*,*) "get_meanfield_dimensions mismatch", md_ns, md_nk, md_nst, md_nao, md_nsp
         nfail = nfail + 1
      end if
      call qctl%init()
      if (qctl%max_iterations /= 10 .or. qctl%state_low /= 1 .or. .not. qctl%update_hartree) then
         write(*,*) "unexpected QSGW control defaults"
         nfail = nfail + 1
      end if
   end block
   allocate(wg(nstates, nkpts, nspins), ekb(nstates, nkpts, nspins))
   wg(:, :, :) = 0.0_dp
   wg(1, :, :) = 2.0_dp
   efermi = 0.0_dp
   do ispin = 1, nspins
      do ik = 1, nkpts
         do istate = 1, nstates
            ekb(istate, ik, ispin) = efermi + sign(istate + ik - 0.5_dp, real(istate, dp) - 1.5_dp)
         end do
      end do
   end do
   call h%set_wg_ekb_efermi(nspins, nkpts, nstates, wg, ekb, efermi)

   wfc(:, :) = (0.0_dp, 0.0_dp)
   do istate = 1, nstates
      wfc(istate, istate) = (1.0_dp, 0.0_dp)
   end do
   do ispin = 1, nspins
      do ik = 1, nkpts
         call h%set_wfc(ispin, ik, nstates, nbasis, wfc)
      end do
   end do

   nbs_wfc = [1, 2]
   nbs_aux = [3, 6]
   if (sum(nbs_wfc) /= nbasis .or. sum(nbs_aux) /= naux) error stop "bad basis"
   call h%set_ao_basis_wfc(natoms, nbs_wfc)
   call h%set_ao_basis_aux(natoms, nbs_aux)

   latt(:, :) = 0.0_dp
   recplatt(:, :) = 0.0_dp
   latt(1, 1) = 2.0_dp
   latt(1, 2) = -1.0_dp
   latt(2, 2) = sqrt(3.0_dp)
   latt(3, 3) = 10.0_dp
   recplatt(1, 1) = pi
   recplatt(2, 1) = pi / sqrt(3.0_dp)
   recplatt(2, 2) = 2.0_dp * pi / sqrt(3.0_dp)
   recplatt(3, 3) = 0.2_dp * pi
   call h%set_latvec_and_G(latt, recplatt)

   kpoints(:, :) = 0.0_dp
   kweights(:) = 1.0_dp / nkpts
   kpoints(:, 2) = recplatt(:, 1) / 3.0_dp
   kpoints(:, 3) = 2.0_dp * recplatt(:, 1) / 3.0_dp
   call h%set_kgrids_kvec(nk1, nk2, nk3, kpoints, kweights)
   map_q_ks = [1, 2, 2]
   call h%set_kq_mapping(nkpts, map_q_ks)

   types = [1, 2]
   posi_cart(:, :) = 0.0_dp
   posi_cart(2, 2) = 2.0_dp / sqrt(3.0_dp)
   posi_cart(3, 2) = 5.0_dp
   call h%set_atoms(natoms, types, posi_cart)

   do ia1 = 1, natoms
      do ia2 = 1, natoms
         allocate(ri_coeff(nbs_aux(ia1), nbs_wfc(ia2), nbs_wfc(ia1)))
         ri_coeff(:, :, :) = 0.0_dp
         do ik1 = -nint((nk1 - 0.1) / 2.0), nint(nk1 / 2.0 - 0.6)
            r = [ik1, 0, 0]
            ri_coeff(1, 1, 1) = 0.01_dp * real(100 * ia1 + 10 * ia2 + ik1, kind=dp)
            call h%set_lri_coeff(ia1, ia2, nbs_wfc(ia1), nbs_wfc(ia2), &
                                 nbs_aux(ia1), r, ri_coeff)
         end do
         deallocate(ri_coeff)
      end do
   end do

   do ia1 = 1, natoms
      do ia2 = 1, natoms
         allocate(vq(nbs_aux(ia1), nbs_aux(ia2)))
         vq(:, :) = (0.0_dp, 0.0_dp)
         do ik = 1, nkpts
            vq(1, 1) = real(10 * ik + ia1 + 0.1_dp * ia2, kind=dp)
            call h%set_aux_bare_coulomb_k_atom_pair(ik, ia1, ia2, &
                 nbs_aux(ia1), nbs_aux(ia2), vq, 0.0_dp)
            vq(1, 1) = -vq(1, 1)
            call h%set_aux_cut_coulomb_k_atom_pair(ik, ia1, ia2, &
                 nbs_aux(ia1), nbs_aux(ia2), vq, 0.0_dp)
         end do
         deallocate(vq)
      end do
   end do

   allocate(omegas(nfreq), df(nfreq))
   omegas = [(real(i, kind=dp), i = 1, nfreq)]
   df = 1.0_dp
   call h%set_dielect_func_imagfreq(omegas, df)
   deallocate(omegas, df)

   allocate(contrib_ibzk(2))

   ! 1. Default options.
   e_default = h%get_rpa_correlation_energy(opts, contrib_ibzk)
   write(*,*) "RPA energy, default options:", e_default

   ! 2. New options at values that must not change the result.
   call opts%set("chi0_method", "auto")
   call opts%set("anacon_method", "pade")
   call opts%set("anacon_aaa_tolerance", 1.0e-10_dp)
   call opts%set("anacon_ridge_lambda", 1.0e-12_dp)
   call opts%set("anacon_ridge_den_weight", 10.0_dp)
   call opts%set("n_bands_exclude_chi0", 0)
   call opts%set("n_bands_exclude_sigc", 0)
   call opts%set("output_exx_mat_k", .false.)
   call opts%set("output_exx_mat_r", .false.)
   e_noop = h%get_rpa_correlation_energy(opts, contrib_ibzk)
   write(*,*) "RPA energy, no-op new options:", e_noop
   if (abs(e_noop - e_default) > 1.0e-10_dp * max(1.0_dp, abs(e_default))) then
      write(*,*) "FAIL: no-op options changed the energy"
      nfail = nfail + 1
   end if

   ! 3. An option that must take effect. It excludes the lowest bands from chi0.
   ! The only occupied band of the model is the lowest one, so no response is
   ! left and the correlation energy must vanish.
   call opts%set("n_bands_exclude_chi0", 1)
   e_exclude = h%get_rpa_correlation_energy(opts, contrib_ibzk)
   write(*,*) "RPA energy, n_bands_exclude_chi0 = 1:", e_exclude
   if (abs(e_exclude) > 1.0e-10_dp) then
      write(*,*) "FAIL: n_bands_exclude_chi0 did not remove the response"
      nfail = nfail + 1
   end if

   deallocate(contrib_ibzk, wg, ekb)
   call h%free()
   call opts%free()

   if (nfail == 0) then
      write(*,*) "test_f03_rpa succeeded"
   else
      write(*,*) "test_f03_rpa failed"
   end if
   call taesc_finalize_global()
   call MPI_Finalize(ierr)
   if (nfail /= 0) stop 1
end program test_f03_rpa
