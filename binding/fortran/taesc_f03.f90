module taesc_f03

   use iso_c_binding, only: c_associated, c_char, c_double, c_f_pointer, &
      c_int, c_null_char, c_null_ptr, c_ptr, c_size_t
   implicit none

   private

   public :: TaescOptions
   public :: TaescHandler
   public :: taesc_init_global
   public :: taesc_finalize_global
   ! public :: taesc_print_profile
   public :: taesc_get_build_info
   public :: taesc_get_major_version
   public :: taesc_get_minor_version
   public :: taesc_get_patch_version

   integer, parameter, public :: dp = @TAESC_FORTRAN_DP@
   integer, parameter, public :: TAESC_MAX_STRLEN = 200

   integer, parameter :: TAESC_BUILD_INFO_MAXLEN = 4096

   type, public :: TaescOptions
      private
      type(c_ptr) :: ptr_c_options = c_null_ptr
   contains
      procedure, public :: init => taesc_options_init
      procedure, public :: free => taesc_options_free
      procedure, public :: is_valid => taesc_options_is_valid
      procedure, private :: set_string => taesc_options_set_string
      procedure, private :: set_integer => taesc_options_set_integer
      procedure, private :: set_real => taesc_options_set_real
      procedure, private :: set_logical => taesc_options_set_logical
      generic, public :: set => set_string, set_integer, set_real, set_logical
      final :: taesc_options_finalize
   end type TaescOptions

   type, public :: TaescHandler
      private
      type(c_ptr) :: ptr_c_handle = c_null_ptr
   contains
      procedure, public :: init => taesc_handler_init
      procedure, public :: free => taesc_handler_free
      procedure, public :: is_valid => taesc_handler_is_valid
      procedure, public :: set_scf_dimension => taesc_set_scf_dimension
      procedure, public :: set_wg_ekb_efermi => taesc_set_wg_ekb_efermi
      procedure, public :: set_wfc => taesc_set_wfc
      procedure, public :: set_wfc_real_imag => taesc_set_wfc_real_imag
      procedure, public :: set_ao_basis_wfc => taesc_set_ao_basis_wfc
      procedure, public :: set_ao_basis_aux => taesc_set_ao_basis_aux
      procedure, public :: set_latvec_and_G => taesc_set_latvec_and_G
      procedure, public :: set_atoms => taesc_set_atoms
      procedure, public :: set_kgrids_kvec => taesc_set_kgrids_kvec
      procedure, public :: set_ibz_mapping => taesc_set_ibz_mapping
      procedure, public :: set_lri_coeff => taesc_set_lri_coeff
      procedure, public :: set_aux_bare_coulomb_k_atom_pair
      procedure, public :: set_aux_cut_coulomb_k_atom_pair
      procedure, public :: set_aux_bare_coulomb_k_2d_block
      procedure, public :: set_aux_cut_coulomb_k_2d_block
      procedure, public :: set_dielect_func_imagfreq
      procedure, public :: set_band_kvec => taesc_set_band_kvec
      procedure, public :: set_band_occ_eigval => taesc_set_band_occ_eigval
      procedure, public :: set_wfc_band => taesc_set_wfc_band
      procedure, public :: set_wfc_band_real_imag => taesc_set_wfc_band_real_imag
      procedure, public :: reset_band_data => taesc_reset_band_data
      procedure, public :: get_imaginary_frequency_grids
      procedure, public :: get_rpa_correlation_energy
      procedure, public :: build_exx => taesc_build_exx
      procedure, public :: get_exx_pot_kgrid
      procedure, public :: get_exx_pot_band_k
      procedure, public :: build_g0w0_sigma => taesc_build_g0w0_sigma
      procedure, public :: get_g0w0_sigc_kgrid
      procedure, public :: get_g0w0_sigc_band_k
      final :: taesc_handler_finalize
   end type TaescHandler

   interface
      function taesc_create_options_c() bind(c, name="taesc_create_options")
         import :: c_ptr
         type(c_ptr) :: taesc_create_options_c
      end function taesc_create_options_c

      subroutine taesc_destroy_options_c(opts) bind(c, name="taesc_destroy_options")
         import :: c_ptr
         type(c_ptr), value :: opts
      end subroutine taesc_destroy_options_c

      subroutine taesc_set_option_c(opts, key, value) bind(c, name="taesc_set_option")
         import :: c_char, c_ptr
         type(c_ptr), value :: opts
         character(kind=c_char), dimension(*), intent(in) :: key
         character(kind=c_char), dimension(*), intent(in) :: value
      end subroutine taesc_set_option_c

      function taesc_create_handler_fortran_c(f_comm) &
            bind(c, name="taesc_create_handler_fortran")
         import :: c_int, c_ptr
         integer(c_int) :: f_comm
         type(c_ptr) :: taesc_create_handler_fortran_c
      end function taesc_create_handler_fortran_c

      subroutine taesc_destroy_handler_c(h) bind(c, name="taesc_destroy_handler")
         import :: c_ptr
         type(c_ptr), value :: h
      end subroutine taesc_destroy_handler_c

      function taesc_get_build_info_c() bind(c, name="taesc_get_build_info")
         import :: c_ptr
         type(c_ptr) :: taesc_get_build_info_c
      end function taesc_get_build_info_c

      function taesc_get_major_version_c() bind(c, name="taesc_get_major_version")
         import :: c_int
         integer(c_int) :: taesc_get_major_version_c
      end function taesc_get_major_version_c

      function taesc_get_minor_version_c() bind(c, name="taesc_get_minor_version")
         import :: c_int
         integer(c_int) :: taesc_get_minor_version_c
      end function taesc_get_minor_version_c

      function taesc_get_patch_version_c() bind(c, name="taesc_get_patch_version")
         import :: c_int
         integer(c_int) :: taesc_get_patch_version_c
      end function taesc_get_patch_version_c

      subroutine taesc_init_global_c() bind(c, name="taesc_init_global")
      end subroutine taesc_init_global_c

      subroutine taesc_finalize_global_c() bind(c, name="taesc_finalize_global")
      end subroutine taesc_finalize_global_c

      subroutine taesc_print_profile_c() bind(c, name="taesc_print_profile")
      end subroutine taesc_print_profile_c
   end interface

   interface
      subroutine taesc_set_scf_dimension_c(h, nspins, nkpts, nstates, nbasis) &
            bind(c, name="taesc_set_scf_dimension")
         import :: c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: nspins, nkpts, nstates, nbasis
      end subroutine taesc_set_scf_dimension_c

      subroutine taesc_set_wg_ekb_efermi_c(h, nspins, nkpts, nstates, wg, ekb, efermi) &
            bind(c, name="taesc_set_wg_ekb_efermi")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: nspins, nkpts, nstates
         real(c_double), dimension(*), intent(in) :: wg
         real(c_double), dimension(*), intent(in) :: ekb
         real(c_double), value :: efermi
      end subroutine taesc_set_wg_ekb_efermi_c

      subroutine taesc_set_wfc_c(h, ispin, ik, nstates_local, nbasis_local, &
            wfc_real, wfc_imag) bind(c, name="taesc_set_wfc")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: ispin, ik, nstates_local, nbasis_local
         real(c_double), dimension(*), intent(in) :: wfc_real
         real(c_double), dimension(*), intent(in) :: wfc_imag
      end subroutine taesc_set_wfc_c

      subroutine taesc_set_ao_basis_wfc_c(h, natoms, nbs_wfc) &
            bind(c, name="taesc_set_ao_basis_wfc")
         import :: c_int, c_ptr, c_size_t
         type(c_ptr), value :: h
         integer(c_int), value :: natoms
         integer(c_size_t), dimension(*), intent(in) :: nbs_wfc
      end subroutine taesc_set_ao_basis_wfc_c

      subroutine taesc_set_ao_basis_aux_c(h, natoms, nbs_aux) &
            bind(c, name="taesc_set_ao_basis_aux")
         import :: c_int, c_ptr, c_size_t
         type(c_ptr), value :: h
         integer(c_int), value :: natoms
         integer(c_size_t), dimension(*), intent(in) :: nbs_aux
      end subroutine taesc_set_ao_basis_aux_c

      subroutine taesc_set_latvec_and_G_c(h, lat_mat, g_mat) &
            bind(c, name="taesc_set_latvec_and_G")
         import :: c_double, c_ptr
         type(c_ptr), value :: h
         real(c_double), dimension(9), intent(in) :: lat_mat, g_mat
      end subroutine taesc_set_latvec_and_G_c

      subroutine taesc_set_atoms_c(h, natoms, types, pos_cart) &
            bind(c, name="taesc_set_atoms")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: natoms
         integer(c_int), dimension(*), intent(in) :: types
         real(c_double), dimension(*), intent(in) :: pos_cart
      end subroutine taesc_set_atoms_c

      subroutine taesc_set_kgrids_kvec_c(h, nk1, nk2, nk3, kvecs) &
            bind(c, name="taesc_set_kgrids_kvec")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: nk1, nk2, nk3
         real(c_double), dimension(*), intent(in) :: kvecs
      end subroutine taesc_set_kgrids_kvec_c

      subroutine taesc_set_ibz_mapping_c(h, nkpts, map_ibzk) &
            bind(c, name="taesc_set_ibz_mapping")
         import :: c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: nkpts
         integer(c_int), dimension(*), intent(in) :: map_ibzk
      end subroutine taesc_set_ibz_mapping_c

      subroutine taesc_set_lri_coeff_c(h, i_atom, j_atom, nao_i, nao_j, &
            naux_i, r, coeff) bind(c, name="taesc_set_lri_coeff")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: i_atom, j_atom, nao_i, nao_j, naux_i
         integer(c_int), dimension(3), intent(in) :: r
         real(c_double), dimension(*), intent(in) :: coeff
      end subroutine taesc_set_lri_coeff_c

      subroutine taesc_set_aux_bare_coulomb_k_atom_pair_c(h, ik, i_atom, &
            j_atom, naux_i, naux_j, vq_real, vq_imag, vq_threshold) &
            bind(c, name="taesc_set_aux_bare_coulomb_k_atom_pair")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: ik, i_atom, j_atom, naux_i, naux_j
         real(c_double), dimension(*), intent(in) :: vq_real, vq_imag
         real(c_double), value :: vq_threshold
      end subroutine taesc_set_aux_bare_coulomb_k_atom_pair_c

      subroutine taesc_set_aux_cut_coulomb_k_atom_pair_c(h, ik, i_atom, &
            j_atom, naux_i, naux_j, vq_real, vq_imag, vq_threshold) &
            bind(c, name="taesc_set_aux_cut_coulomb_k_atom_pair")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: ik, i_atom, j_atom, naux_i, naux_j
         real(c_double), dimension(*), intent(in) :: vq_real, vq_imag
         real(c_double), value :: vq_threshold
      end subroutine taesc_set_aux_cut_coulomb_k_atom_pair_c

      subroutine taesc_set_aux_bare_coulomb_k_2d_block_c(h, ik, mu_begin, &
            mu_end, nu_begin, nu_end, vq_real, vq_imag) &
            bind(c, name="taesc_set_aux_bare_coulomb_k_2d_block")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: ik, mu_begin, mu_end, nu_begin, nu_end
         real(c_double), dimension(*), intent(in) :: vq_real, vq_imag
      end subroutine taesc_set_aux_bare_coulomb_k_2d_block_c

      subroutine taesc_set_aux_cut_coulomb_k_2d_block_c(h, ik, mu_begin, &
            mu_end, nu_begin, nu_end, vq_real, vq_imag) &
            bind(c, name="taesc_set_aux_cut_coulomb_k_2d_block")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: ik, mu_begin, mu_end, nu_begin, nu_end
         real(c_double), dimension(*), intent(in) :: vq_real, vq_imag
      end subroutine taesc_set_aux_cut_coulomb_k_2d_block_c

      subroutine taesc_set_dielect_func_imagfreq_c(h, nfreq, omegas_imag, &
            dielect_func) bind(c, name="taesc_set_dielect_func_imagfreq")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: nfreq
         real(c_double), dimension(*), intent(in) :: omegas_imag
         real(c_double), dimension(*), intent(in) :: dielect_func
      end subroutine taesc_set_dielect_func_imagfreq_c

      subroutine taesc_set_band_kvec_c(h, n_kpts_band, kfrac_band) &
            bind(c, name="taesc_set_band_kvec")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: n_kpts_band
         real(c_double), dimension(*), intent(in) :: kfrac_band
      end subroutine taesc_set_band_kvec_c

      subroutine taesc_set_band_occ_eigval_c(h, n_spins, n_kpts_band, &
            n_states, occ, eig) bind(c, name="taesc_set_band_occ_eigval")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: n_spins, n_kpts_band, n_states
         real(c_double), dimension(*), intent(in) :: occ
         real(c_double), dimension(*), intent(in) :: eig
      end subroutine taesc_set_band_occ_eigval_c

      subroutine taesc_set_wfc_band_c(h, ispin, ik_band, nstates_local, &
            nbasis_local, wfc_real, wfc_imag) bind(c, name="taesc_set_wfc_band")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         integer(c_int), value :: ispin, ik_band, nstates_local, nbasis_local
         real(c_double), dimension(*), intent(in) :: wfc_real
         real(c_double), dimension(*), intent(in) :: wfc_imag
      end subroutine taesc_set_wfc_band_c

      subroutine taesc_reset_band_data_c(h) bind(c, name="taesc_reset_band_data")
         import :: c_ptr
         type(c_ptr), value :: h
      end subroutine taesc_reset_band_data_c
   end interface

   interface
      subroutine taesc_get_imaginary_frequency_grids_c(h, opts, omegas, weights) &
            bind(c, name="taesc_get_imaginary_frequency_grids")
         import :: c_double, c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
         real(c_double), dimension(*), intent(inout) :: omegas, weights
      end subroutine taesc_get_imaginary_frequency_grids_c

      function taesc_get_rpa_correlation_energy_c(h, opts, n_ibz_kpoints, &
            contrib_re, contrib_im) bind(c, name="taesc_get_rpa_correlation_energy")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
         integer(c_int), value :: n_ibz_kpoints
         real(c_double), dimension(*), intent(inout) :: contrib_re, contrib_im
         real(c_double) :: taesc_get_rpa_correlation_energy_c
      end function taesc_get_rpa_correlation_energy_c

      subroutine taesc_build_exx_c(h, opts) bind(c, name="taesc_build_exx")
         import :: c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
      end subroutine taesc_build_exx_c

      subroutine taesc_get_exx_pot_kgrid_c(h, opts, n_spins, n_kpts_this, &
            iks_this, i_state_low, i_state_high, vexx) &
            bind(c, name="taesc_get_exx_pot_kgrid")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
         integer(c_int), value :: n_spins, n_kpts_this, i_state_low, i_state_high
         integer(c_int), dimension(*), intent(in) :: iks_this
         real(c_double), dimension(*), intent(inout) :: vexx
      end subroutine taesc_get_exx_pot_kgrid_c

      subroutine taesc_get_exx_pot_band_k_c(h, opts, n_spins, n_kpts_band_this, &
            iks_band_this, i_state_low, i_state_high, vexx_band) &
            bind(c, name="taesc_get_exx_pot_band_k")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
         integer(c_int), value :: n_spins, n_kpts_band_this
         integer(c_int), value :: i_state_low, i_state_high
         integer(c_int), dimension(*), intent(in) :: iks_band_this
         real(c_double), dimension(*), intent(inout) :: vexx_band
      end subroutine taesc_get_exx_pot_band_k_c

      subroutine taesc_build_g0w0_sigma_c(h, opts) &
            bind(c, name="taesc_build_g0w0_sigma")
         import :: c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
      end subroutine taesc_build_g0w0_sigma_c

      subroutine taesc_get_g0w0_sigc_kgrid_c(h, opts, n_spins, n_kpts_this, &
            iks_this, i_state_low, i_state_high, vxc, vexx, sigc_re, sigc_im) &
            bind(c, name="taesc_get_g0w0_sigc_kgrid")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
         integer(c_int), value :: n_spins, n_kpts_this, i_state_low, i_state_high
         integer(c_int), dimension(*), intent(in) :: iks_this
         real(c_double), dimension(*), intent(in) :: vxc, vexx
         real(c_double), dimension(*), intent(inout) :: sigc_re, sigc_im
      end subroutine taesc_get_g0w0_sigc_kgrid_c

      subroutine taesc_get_g0w0_sigc_band_k_c(h, opts, n_spins, n_kpts_band, &
            iks_band, i_state_low, i_state_high, vxc, vexx, sigc_re, sigc_im) &
            bind(c, name="taesc_get_g0w0_sigc_band_k")
         import :: c_double, c_int, c_ptr
         type(c_ptr), value :: h
         type(c_ptr), value :: opts
         integer(c_int), value :: n_spins, n_kpts_band, i_state_low, i_state_high
         integer(c_int), dimension(*), intent(in) :: iks_band
         real(c_double), dimension(*), intent(in) :: vxc, vexx
         real(c_double), dimension(*), intent(inout) :: sigc_re, sigc_im
      end subroutine taesc_get_g0w0_sigc_band_k_c
   end interface

contains

   function to_c_string(f_string) result(c_string)
      character(len=*), intent(in) :: f_string
      character(kind=c_char), allocatable :: c_string(:)
      integer :: i, n

      n = len_trim(f_string)
      allocate(c_string(n + 1))
      do i = 1, n
         c_string(i) = f_string(i:i)
      end do
      c_string(n + 1) = c_null_char
   end function to_c_string

   subroutine ensure_options(opts)
      class(TaescOptions), intent(inout) :: opts

      if (.not. c_associated(opts%ptr_c_options)) then
         call opts%init()
      end if
   end subroutine ensure_options

   subroutine require_handler(h, routine)
      class(TaescHandler), intent(in) :: h
      character(len=*), intent(in) :: routine

      if (.not. c_associated(h%ptr_c_handle)) then
         error stop trim(routine) // ": TaescHandler is not initialized"
      end if
   end subroutine require_handler

   subroutine require_matching_sizes(a_size, b_size, routine)
      integer, intent(in) :: a_size, b_size
      character(len=*), intent(in) :: routine

      if (a_size /= b_size) then
         error stop trim(routine) // ": array sizes do not match"
      end if
   end subroutine require_matching_sizes

   subroutine taesc_options_init(this)
      class(TaescOptions), intent(inout) :: this

      if (c_associated(this%ptr_c_options)) call this%free()
      this%ptr_c_options = taesc_create_options_c()
      if (.not. c_associated(this%ptr_c_options)) then
         error stop "taesc_options_init: failed to create TaescOptions"
      end if
   end subroutine taesc_options_init

   subroutine taesc_options_free(this)
      class(TaescOptions), intent(inout) :: this

      if (c_associated(this%ptr_c_options)) then
         call taesc_destroy_options_c(this%ptr_c_options)
         this%ptr_c_options = c_null_ptr
      end if
   end subroutine taesc_options_free

   logical function taesc_options_is_valid(this) result(valid)
      class(TaescOptions), intent(in) :: this

      valid = c_associated(this%ptr_c_options)
   end function taesc_options_is_valid

   subroutine taesc_options_finalize(this)
      type(TaescOptions), intent(inout) :: this

      call this%free()
   end subroutine taesc_options_finalize

   subroutine taesc_options_set_string(this, key, value)
      class(TaescOptions), intent(inout) :: this
      character(len=*), intent(in) :: key, value
      character(kind=c_char), allocatable :: key_c(:), value_c(:)

      call ensure_options(this)
      key_c = to_c_string(key)
      value_c = to_c_string(value)
      call taesc_set_option_c(this%ptr_c_options, key_c, value_c)
   end subroutine taesc_options_set_string

   subroutine taesc_options_set_integer(this, key, value)
      class(TaescOptions), intent(inout) :: this
      character(len=*), intent(in) :: key
      integer, intent(in) :: value
      character(len=TAESC_MAX_STRLEN) :: value_string

      write(value_string, "(I0)") value
      call taesc_options_set_string(this, key, trim(value_string))
   end subroutine taesc_options_set_integer

   subroutine taesc_options_set_real(this, key, value)
      class(TaescOptions), intent(inout) :: this
      character(len=*), intent(in) :: key
      real(dp), intent(in) :: value
      character(len=TAESC_MAX_STRLEN) :: value_string

      write(value_string, "(ES24.16E3)") value
      call taesc_options_set_string(this, key, adjustl(value_string))
   end subroutine taesc_options_set_real

   subroutine taesc_options_set_logical(this, key, value)
      class(TaescOptions), intent(inout) :: this
      character(len=*), intent(in) :: key
      logical, intent(in) :: value

      if (value) then
         call taesc_options_set_string(this, key, "true")
      else
         call taesc_options_set_string(this, key, "false")
      end if
   end subroutine taesc_options_set_logical

   subroutine taesc_handler_init(this, comm)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: comm
      integer(c_int) :: f_comm

      if (c_associated(this%ptr_c_handle)) call this%free()
      f_comm = int(comm, kind=c_int)
      this%ptr_c_handle = taesc_create_handler_fortran_c(f_comm)
      if (.not. c_associated(this%ptr_c_handle)) then
         error stop "taesc_handler_init: failed to create TaescHandler"
      end if
   end subroutine taesc_handler_init

   subroutine taesc_handler_free(this)
      class(TaescHandler), intent(inout) :: this

      if (c_associated(this%ptr_c_handle)) then
         call taesc_destroy_handler_c(this%ptr_c_handle)
         this%ptr_c_handle = c_null_ptr
      end if
   end subroutine taesc_handler_free

   logical function taesc_handler_is_valid(this) result(valid)
      class(TaescHandler), intent(in) :: this

      valid = c_associated(this%ptr_c_handle)
   end function taesc_handler_is_valid

   subroutine taesc_handler_finalize(this)
      type(TaescHandler), intent(inout) :: this

      call this%free()
   end subroutine taesc_handler_finalize

   subroutine taesc_init_global()
      call taesc_init_global_c()
   end subroutine taesc_init_global

   subroutine taesc_finalize_global()
      call taesc_finalize_global_c()
   end subroutine taesc_finalize_global

   subroutine taesc_print_profile()
      call taesc_print_profile_c()
   end subroutine taesc_print_profile

   function taesc_get_build_info() result(info)
      character(len=:), allocatable :: info
      type(c_ptr) :: c_info
      character(kind=c_char), pointer :: c_info_chars(:)
      integer :: i, n

      c_info = taesc_get_build_info_c()
      if (.not. c_associated(c_info)) then
         allocate(character(len=0) :: info)
         return
      end if

      call c_f_pointer(c_info, c_info_chars, [TAESC_BUILD_INFO_MAXLEN])
      n = 0
      do i = 1, TAESC_BUILD_INFO_MAXLEN
         if (c_info_chars(i) == c_null_char) exit
         n = n + 1
      end do

      allocate(character(len=n) :: info)
      do i = 1, n
         info(i:i) = c_info_chars(i)
      end do
   end function taesc_get_build_info

   integer function taesc_get_major_version() result(version)
      version = int(taesc_get_major_version_c())
   end function taesc_get_major_version

   integer function taesc_get_minor_version() result(version)
      version = int(taesc_get_minor_version_c())
   end function taesc_get_minor_version

   integer function taesc_get_patch_version() result(version)
      version = int(taesc_get_patch_version_c())
   end function taesc_get_patch_version

   subroutine taesc_set_scf_dimension(this, nspins, nkpts, nstates, nbasis)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: nspins, nkpts, nstates, nbasis

      call require_handler(this, "taesc_set_scf_dimension")
      call taesc_set_scf_dimension_c(this%ptr_c_handle, int(nspins, c_int), &
         int(nkpts, c_int), int(nstates, c_int), int(nbasis, c_int))
   end subroutine taesc_set_scf_dimension

   subroutine taesc_set_wg_ekb_efermi(this, nspins, nkpts, nstates, wg, ekb, efermi)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: nspins, nkpts, nstates
      real(dp), intent(in) :: wg(nstates, nkpts, nspins)
      real(dp), intent(in) :: ekb(nstates, nkpts, nspins)
      real(dp), intent(in) :: efermi
      real(c_double), allocatable :: wg_c(:,:,:), ekb_c(:,:,:)

      call require_handler(this, "taesc_set_wg_ekb_efermi")
      allocate(wg_c(nstates, nkpts, nspins), ekb_c(nstates, nkpts, nspins))
      wg_c = real(wg, kind=c_double)
      ekb_c = real(ekb, kind=c_double)
      call taesc_set_wg_ekb_efermi_c(this%ptr_c_handle, int(nspins, c_int), &
         int(nkpts, c_int), int(nstates, c_int), wg_c, ekb_c, real(efermi, c_double))
      deallocate(wg_c, ekb_c)
   end subroutine taesc_set_wg_ekb_efermi

   subroutine taesc_set_wfc(this, ispin, ik, nstates_local, nbasis_local, wfc)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ispin, ik, nstates_local, nbasis_local
      complex(dp), intent(in) :: wfc(nbasis_local, nstates_local)
      real(c_double), allocatable :: wfc_real(:,:), wfc_imag(:,:)

      call require_handler(this, "taesc_set_wfc")
      allocate(wfc_real(nbasis_local, nstates_local))
      allocate(wfc_imag(nbasis_local, nstates_local))
      wfc_real = real(wfc, kind=c_double)
      wfc_imag = real(aimag(wfc), kind=c_double)
      call taesc_set_wfc_c(this%ptr_c_handle, int(ispin - 1, c_int), &
         int(ik - 1, c_int), int(nstates_local, c_int), int(nbasis_local, c_int), &
         wfc_real, wfc_imag)
      deallocate(wfc_real, wfc_imag)
   end subroutine taesc_set_wfc

   subroutine taesc_set_wfc_real_imag(this, ispin, ik, nstates_local, &
         nbasis_local, wfc_real, wfc_imag)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ispin, ik, nstates_local, nbasis_local
      real(dp), intent(in) :: wfc_real(nbasis_local, nstates_local)
      real(dp), intent(in) :: wfc_imag(nbasis_local, nstates_local)
      real(c_double), allocatable :: wfc_real_c(:,:), wfc_imag_c(:,:)

      call require_handler(this, "taesc_set_wfc_real_imag")
      allocate(wfc_real_c(nbasis_local, nstates_local))
      allocate(wfc_imag_c(nbasis_local, nstates_local))
      wfc_real_c = real(wfc_real, kind=c_double)
      wfc_imag_c = real(wfc_imag, kind=c_double)
      call taesc_set_wfc_c(this%ptr_c_handle, int(ispin - 1, c_int), &
         int(ik - 1, c_int), int(nstates_local, c_int), int(nbasis_local, c_int), &
         wfc_real_c, wfc_imag_c)
      deallocate(wfc_real_c, wfc_imag_c)
   end subroutine taesc_set_wfc_real_imag

   subroutine set_ao_basis(this, natoms, nbs, is_aux)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: natoms
      integer, intent(in) :: nbs(natoms)
      logical, intent(in) :: is_aux
      integer(c_size_t), allocatable :: nbs_c(:)

      call require_handler(this, "set_ao_basis")
      allocate(nbs_c(natoms))
      nbs_c = int(nbs, kind=c_size_t)
      if (is_aux) then
         call taesc_set_ao_basis_aux_c(this%ptr_c_handle, int(natoms, c_int), nbs_c)
      else
         call taesc_set_ao_basis_wfc_c(this%ptr_c_handle, int(natoms, c_int), nbs_c)
      end if
      deallocate(nbs_c)
   end subroutine set_ao_basis

   subroutine taesc_set_ao_basis_wfc(this, natoms, nbs_wfc)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: natoms
      integer, intent(in) :: nbs_wfc(natoms)

      call set_ao_basis(this, natoms, nbs_wfc, .false.)
   end subroutine taesc_set_ao_basis_wfc

   subroutine taesc_set_ao_basis_aux(this, natoms, nbs_aux)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: natoms
      integer, intent(in) :: nbs_aux(natoms)

      call set_ao_basis(this, natoms, nbs_aux, .true.)
   end subroutine taesc_set_ao_basis_aux

   subroutine taesc_set_latvec_and_G(this, lat_mat, g_mat)
      class(TaescHandler), intent(inout) :: this
      real(dp), intent(in) :: lat_mat(3, 3), g_mat(3, 3)
      real(c_double) :: lat_mat_c(3, 3), g_mat_c(3, 3)

      call require_handler(this, "taesc_set_latvec_and_G")
      lat_mat_c = real(lat_mat, kind=c_double)
      g_mat_c = real(g_mat, kind=c_double)
      call taesc_set_latvec_and_G_c(this%ptr_c_handle, lat_mat_c, g_mat_c)
   end subroutine taesc_set_latvec_and_G

   subroutine taesc_set_atoms(this, natoms, types, pos_cart)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: natoms
      integer, intent(in) :: types(natoms)
      real(dp), intent(in) :: pos_cart(3, natoms)
      integer(c_int), allocatable :: types_c(:)
      real(c_double), allocatable :: pos_cart_c(:,:)

      call require_handler(this, "taesc_set_atoms")
      allocate(types_c(natoms), pos_cart_c(3, natoms))
      types_c = int(types, kind=c_int)
      pos_cart_c = real(pos_cart, kind=c_double)
      call taesc_set_atoms_c(this%ptr_c_handle, int(natoms, c_int), types_c, pos_cart_c)
      deallocate(types_c, pos_cart_c)
   end subroutine taesc_set_atoms

   subroutine taesc_set_kgrids_kvec(this, nk1, nk2, nk3, kvecs)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: nk1, nk2, nk3
      real(dp), intent(in) :: kvecs(3, nk1 * nk2 * nk3)
      real(c_double), allocatable :: kvecs_c(:,:)

      call require_handler(this, "taesc_set_kgrids_kvec")
      allocate(kvecs_c(3, nk1 * nk2 * nk3))
      kvecs_c = real(kvecs, kind=c_double)
      call taesc_set_kgrids_kvec_c(this%ptr_c_handle, int(nk1, c_int), &
         int(nk2, c_int), int(nk3, c_int), kvecs_c)
      deallocate(kvecs_c)
   end subroutine taesc_set_kgrids_kvec

   subroutine taesc_set_ibz_mapping(this, nkpts, map_ibzk)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: nkpts
      integer, intent(in) :: map_ibzk(nkpts)
      integer(c_int), allocatable :: map_ibzk_c(:)

      call require_handler(this, "taesc_set_ibz_mapping")
      allocate(map_ibzk_c(nkpts))
      map_ibzk_c = int(map_ibzk, kind=c_int) - 1
      call taesc_set_ibz_mapping_c(this%ptr_c_handle, int(nkpts, c_int), map_ibzk_c)
      deallocate(map_ibzk_c)
   end subroutine taesc_set_ibz_mapping

   subroutine taesc_set_lri_coeff(this, i_atom, j_atom, nao_i, nao_j, naux_i, r, coeff)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: i_atom, j_atom, nao_i, nao_j, naux_i
      integer, intent(in) :: r(3)
      real(dp), contiguous, intent(in) :: coeff(:, :, :)
      integer(c_int) :: r_c(3)
      real(c_double), allocatable :: coeff_c(:,:,:)

      call require_handler(this, "taesc_set_lri_coeff")
      if (size(coeff, 1) /= naux_i .or. size(coeff, 2) /= nao_j) then
         error stop "taesc_set_lri_coeff: coeff has wrong shape"
      end if
      if (size(coeff, 3) /= nao_i) error stop "taesc_set_lri_coeff: coeff has wrong shape"

      allocate(coeff_c(naux_i, nao_j, nao_i))
      r_c = int(r, kind=c_int)
      coeff_c = real(coeff, kind=c_double)
      call taesc_set_lri_coeff_c(this%ptr_c_handle, int(i_atom - 1, c_int), &
         int(j_atom - 1, c_int), int(nao_i, c_int), int(nao_j, c_int), &
         int(naux_i, c_int), r_c, coeff_c)
      deallocate(coeff_c)
   end subroutine taesc_set_lri_coeff

   subroutine set_aux_coulomb_k_atom_pair(this, ik, i_atom, j_atom, naux_i, &
         naux_j, vq, vq_threshold, is_cut)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ik, i_atom, j_atom, naux_i, naux_j
      complex(dp), intent(in) :: vq(naux_i, naux_j)
      real(dp), intent(in) :: vq_threshold
      logical, intent(in) :: is_cut
      real(c_double), allocatable :: vq_real(:,:), vq_imag(:,:)

      call require_handler(this, "set_aux_coulomb_k_atom_pair")
      allocate(vq_real(naux_j, naux_i), vq_imag(naux_j, naux_i))
      vq_real = transpose(real(vq, kind=c_double))
      vq_imag = transpose(real(aimag(vq), kind=c_double))
      if (is_cut) then
         call taesc_set_aux_cut_coulomb_k_atom_pair_c(this%ptr_c_handle, &
            int(ik - 1, c_int), int(i_atom - 1, c_int), int(j_atom - 1, c_int), &
            int(naux_i, c_int), int(naux_j, c_int), vq_real, vq_imag, &
            real(vq_threshold, kind=c_double))
      else
         call taesc_set_aux_bare_coulomb_k_atom_pair_c(this%ptr_c_handle, &
            int(ik - 1, c_int), int(i_atom - 1, c_int), int(j_atom - 1, c_int), &
            int(naux_i, c_int), int(naux_j, c_int), vq_real, vq_imag, &
            real(vq_threshold, kind=c_double))
      end if
      deallocate(vq_real, vq_imag)
   end subroutine set_aux_coulomb_k_atom_pair

   subroutine set_aux_bare_coulomb_k_atom_pair(this, ik, i_atom, j_atom, &
         naux_i, naux_j, vq, vq_threshold)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ik, i_atom, j_atom, naux_i, naux_j
      complex(dp), intent(in) :: vq(naux_i, naux_j)
      real(dp), intent(in) :: vq_threshold

      call set_aux_coulomb_k_atom_pair(this, ik, i_atom, j_atom, naux_i, &
         naux_j, vq, vq_threshold, .false.)
   end subroutine set_aux_bare_coulomb_k_atom_pair

   subroutine set_aux_cut_coulomb_k_atom_pair(this, ik, i_atom, j_atom, &
         naux_i, naux_j, vq, vq_threshold)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ik, i_atom, j_atom, naux_i, naux_j
      complex(dp), intent(in) :: vq(naux_i, naux_j)
      real(dp), intent(in) :: vq_threshold

      call set_aux_coulomb_k_atom_pair(this, ik, i_atom, j_atom, naux_i, &
         naux_j, vq, vq_threshold, .true.)
   end subroutine set_aux_cut_coulomb_k_atom_pair

   subroutine set_aux_coulomb_k_2d_block(this, ik, mu_begin, mu_end, &
         nu_begin, nu_end, vq, is_cut)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ik, mu_begin, mu_end, nu_begin, nu_end
      complex(dp), intent(in) :: vq(mu_end - mu_begin + 1, nu_end - nu_begin + 1)
      logical, intent(in) :: is_cut
      integer(c_int) :: mu_begin_c, mu_end_c, nu_begin_c, nu_end_c
      real(c_double), allocatable :: vq_real(:,:), vq_imag(:,:)

      call require_handler(this, "set_aux_coulomb_k_2d_block")
      mu_begin_c = int(mu_begin - 1, c_int)
      mu_end_c = int(mu_end, c_int)
      nu_begin_c = int(nu_begin - 1, c_int)
      nu_end_c = int(nu_end, c_int)
      allocate(vq_real(nu_end_c - nu_begin_c, mu_end_c - mu_begin_c))
      allocate(vq_imag(nu_end_c - nu_begin_c, mu_end_c - mu_begin_c))
      vq_real = transpose(real(vq, kind=c_double))
      vq_imag = transpose(real(aimag(vq), kind=c_double))

      if (is_cut) then
         call taesc_set_aux_cut_coulomb_k_2d_block_c(this%ptr_c_handle, &
            int(ik - 1, c_int), mu_begin_c, mu_end_c, nu_begin_c, nu_end_c, &
            vq_real, vq_imag)
      else
         call taesc_set_aux_bare_coulomb_k_2d_block_c(this%ptr_c_handle, &
            int(ik - 1, c_int), mu_begin_c, mu_end_c, nu_begin_c, nu_end_c, &
            vq_real, vq_imag)
      end if
      deallocate(vq_real, vq_imag)
   end subroutine set_aux_coulomb_k_2d_block

   subroutine set_aux_bare_coulomb_k_2d_block(this, ik, mu_begin, mu_end, &
         nu_begin, nu_end, vq)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ik, mu_begin, mu_end, nu_begin, nu_end
      complex(dp), intent(in) :: vq(mu_end - mu_begin + 1, nu_end - nu_begin + 1)

      call set_aux_coulomb_k_2d_block(this, ik, mu_begin, mu_end, nu_begin, &
         nu_end, vq, .false.)
   end subroutine set_aux_bare_coulomb_k_2d_block

   subroutine set_aux_cut_coulomb_k_2d_block(this, ik, mu_begin, mu_end, &
         nu_begin, nu_end, vq)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ik, mu_begin, mu_end, nu_begin, nu_end
      complex(dp), intent(in) :: vq(mu_end - mu_begin + 1, nu_end - nu_begin + 1)

      call set_aux_coulomb_k_2d_block(this, ik, mu_begin, mu_end, nu_begin, &
         nu_end, vq, .true.)
   end subroutine set_aux_cut_coulomb_k_2d_block

   subroutine set_dielect_func_imagfreq(this, omegas_imag, dielect_func)
      class(TaescHandler), intent(inout) :: this
      real(dp), intent(in) :: omegas_imag(:)
      real(dp), intent(in) :: dielect_func(:)
      integer :: nfreq
      real(c_double), allocatable :: omegas_c(:), dielect_c(:)

      call require_handler(this, "set_dielect_func_imagfreq")
      call require_matching_sizes(size(omegas_imag), size(dielect_func), &
         "set_dielect_func_imagfreq")
      nfreq = size(omegas_imag)
      allocate(omegas_c(nfreq), dielect_c(nfreq))
      omegas_c = real(omegas_imag, kind=c_double)
      dielect_c = real(dielect_func, kind=c_double)
      call taesc_set_dielect_func_imagfreq_c(this%ptr_c_handle, int(nfreq, c_int), &
         omegas_c, dielect_c)
      deallocate(omegas_c, dielect_c)
   end subroutine set_dielect_func_imagfreq

   subroutine taesc_set_band_kvec(this, kfrac_band)
      class(TaescHandler), intent(inout) :: this
      real(dp), intent(in) :: kfrac_band(:, :)
      integer :: n_kpts_band
      real(c_double), allocatable :: kfrac_band_c(:,:)

      call require_handler(this, "taesc_set_band_kvec")
      if (size(kfrac_band, 1) /= 3) then
         error stop "taesc_set_band_kvec: kfrac_band must have first dimension 3"
      end if
      n_kpts_band = size(kfrac_band, 2)
      allocate(kfrac_band_c(3, n_kpts_band))
      kfrac_band_c = real(kfrac_band, kind=c_double)
      call taesc_set_band_kvec_c(this%ptr_c_handle, int(n_kpts_band, c_int), &
         kfrac_band_c)
      deallocate(kfrac_band_c)
   end subroutine taesc_set_band_kvec

   subroutine taesc_set_band_occ_eigval(this, n_spins, n_kpts_band, n_states, occ, eig)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: n_spins, n_kpts_band, n_states
      real(dp), intent(in) :: occ(n_states, n_kpts_band, n_spins)
      real(dp), intent(in) :: eig(n_states, n_kpts_band, n_spins)
      real(c_double), allocatable :: occ_c(:,:,:), eig_c(:,:,:)

      call require_handler(this, "taesc_set_band_occ_eigval")
      allocate(occ_c(n_states, n_kpts_band, n_spins))
      allocate(eig_c(n_states, n_kpts_band, n_spins))
      occ_c = real(occ, kind=c_double)
      eig_c = real(eig, kind=c_double)
      call taesc_set_band_occ_eigval_c(this%ptr_c_handle, int(n_spins, c_int), &
         int(n_kpts_band, c_int), int(n_states, c_int), occ_c, eig_c)
      deallocate(occ_c, eig_c)
   end subroutine taesc_set_band_occ_eigval

   subroutine taesc_set_wfc_band(this, ispin, ik_band, nstates_local, &
         nbasis_local, wfc)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ispin, ik_band, nstates_local, nbasis_local
      complex(dp), intent(in) :: wfc(nbasis_local, nstates_local)
      real(c_double), allocatable :: wfc_real(:,:), wfc_imag(:,:)

      call require_handler(this, "taesc_set_wfc_band")
      allocate(wfc_real(nbasis_local, nstates_local))
      allocate(wfc_imag(nbasis_local, nstates_local))
      wfc_real = real(wfc, kind=c_double)
      wfc_imag = real(aimag(wfc), kind=c_double)
      call taesc_set_wfc_band_c(this%ptr_c_handle, int(ispin - 1, c_int), &
         int(ik_band - 1, c_int), int(nstates_local, c_int), &
         int(nbasis_local, c_int), wfc_real, wfc_imag)
      deallocate(wfc_real, wfc_imag)
   end subroutine taesc_set_wfc_band

   subroutine taesc_set_wfc_band_real_imag(this, ispin, ik_band, nstates_local, &
         nbasis_local, wfc_real, wfc_imag)
      class(TaescHandler), intent(inout) :: this
      integer, intent(in) :: ispin, ik_band, nstates_local, nbasis_local
      real(dp), intent(in) :: wfc_real(nbasis_local, nstates_local)
      real(dp), intent(in) :: wfc_imag(nbasis_local, nstates_local)
      real(c_double), allocatable :: wfc_real_c(:,:), wfc_imag_c(:,:)

      call require_handler(this, "taesc_set_wfc_band_real_imag")
      allocate(wfc_real_c(nbasis_local, nstates_local))
      allocate(wfc_imag_c(nbasis_local, nstates_local))
      wfc_real_c = real(wfc_real, kind=c_double)
      wfc_imag_c = real(wfc_imag, kind=c_double)
      call taesc_set_wfc_band_c(this%ptr_c_handle, int(ispin - 1, c_int), &
         int(ik_band - 1, c_int), int(nstates_local, c_int), &
         int(nbasis_local, c_int), wfc_real_c, wfc_imag_c)
      deallocate(wfc_real_c, wfc_imag_c)
   end subroutine taesc_set_wfc_band_real_imag

   subroutine taesc_reset_band_data(this)
      class(TaescHandler), intent(inout) :: this

      call require_handler(this, "taesc_reset_band_data")
      call taesc_reset_band_data_c(this%ptr_c_handle)
   end subroutine taesc_reset_band_data

   subroutine get_imaginary_frequency_grids(this, opts, omegas, weights)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts
      real(dp), intent(inout) :: omegas(:), weights(:)
      integer :: nfreq
      real(c_double), allocatable :: omegas_c(:), weights_c(:)

      call require_handler(this, "get_imaginary_frequency_grids")
      call ensure_options(opts)
      call require_matching_sizes(size(omegas), size(weights), &
         "get_imaginary_frequency_grids")
      nfreq = size(omegas)
      allocate(omegas_c(nfreq), weights_c(nfreq))
      call taesc_get_imaginary_frequency_grids_c(this%ptr_c_handle, &
         opts%ptr_c_options, omegas_c, weights_c)
      omegas = real(omegas_c, kind=dp)
      weights = real(weights_c, kind=dp)
      deallocate(omegas_c, weights_c)
   end subroutine get_imaginary_frequency_grids

   real(dp) function get_rpa_correlation_energy(this, opts, contrib_ibzk) result(energy)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts
      complex(dp), intent(inout) :: contrib_ibzk(:)
      integer :: nkpts_ibz
      real(c_double) :: energy_c
      real(c_double), allocatable :: contrib_re(:), contrib_im(:)

      call require_handler(this, "get_rpa_correlation_energy")
      call ensure_options(opts)
      nkpts_ibz = size(contrib_ibzk)
      allocate(contrib_re(nkpts_ibz), contrib_im(nkpts_ibz))
      energy_c = taesc_get_rpa_correlation_energy_c(this%ptr_c_handle, &
         opts%ptr_c_options, int(nkpts_ibz, c_int), contrib_re, contrib_im)
      energy = real(energy_c, kind=dp)
      contrib_ibzk = cmplx(real(contrib_re, kind=dp), real(contrib_im, kind=dp), kind=dp)
      deallocate(contrib_re, contrib_im)
   end function get_rpa_correlation_energy

   subroutine taesc_build_exx(this, opts)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts

      call require_handler(this, "taesc_build_exx")
      call ensure_options(opts)
      call taesc_build_exx_c(this%ptr_c_handle, opts%ptr_c_options)
   end subroutine taesc_build_exx

   subroutine get_exx_pot_kgrid(this, opts, n_spins, iks_this, &
         i_state_low, i_state_high, vexx)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts
      integer, intent(in) :: n_spins, i_state_low, i_state_high
      integer, intent(in) :: iks_this(:)
      real(dp), intent(inout) :: vexx(:, :, :)
      integer :: n_kpts_this, n_states_calc
      integer(c_int), allocatable :: iks_this_c(:)
      real(c_double), allocatable :: vexx_c(:,:,:)

      call require_handler(this, "get_exx_pot_kgrid")
      call ensure_options(opts)
      n_kpts_this = size(iks_this)
      n_states_calc = i_state_high - i_state_low + 1
      call require_result_shape(size(vexx, 1), size(vexx, 2), size(vexx, 3), &
         n_states_calc, n_kpts_this, n_spins, "get_exx_pot_kgrid")
      call make_c_indices(iks_this, iks_this_c)
      allocate(vexx_c(n_states_calc, max(1, n_kpts_this), n_spins))
      call taesc_get_exx_pot_kgrid_c(this%ptr_c_handle, opts%ptr_c_options, &
         int(n_spins, c_int), int(n_kpts_this, c_int), iks_this_c, &
         int(i_state_low - 1, c_int), int(i_state_high, c_int), vexx_c)
      if (n_kpts_this > 0) vexx = real(vexx_c, kind=dp)
      deallocate(iks_this_c, vexx_c)
   end subroutine get_exx_pot_kgrid

   subroutine get_exx_pot_band_k(this, opts, n_spins, iks_band_this, &
         i_state_low, i_state_high, vexx_band)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts
      integer, intent(in) :: n_spins, i_state_low, i_state_high
      integer, intent(in) :: iks_band_this(:)
      real(dp), intent(inout) :: vexx_band(:, :, :)
      integer :: n_kpts_band_this, n_states_calc
      integer(c_int), allocatable :: iks_band_this_c(:)
      real(c_double), allocatable :: vexx_band_c(:,:,:)

      call require_handler(this, "get_exx_pot_band_k")
      call ensure_options(opts)
      n_kpts_band_this = size(iks_band_this)
      n_states_calc = i_state_high - i_state_low + 1
      call require_result_shape(size(vexx_band, 1), size(vexx_band, 2), &
         size(vexx_band, 3), n_states_calc, n_kpts_band_this, n_spins, &
         "get_exx_pot_band_k")
      call make_c_indices(iks_band_this, iks_band_this_c)
      allocate(vexx_band_c(n_states_calc, max(1, n_kpts_band_this), n_spins))
      call taesc_get_exx_pot_band_k_c(this%ptr_c_handle, opts%ptr_c_options, &
         int(n_spins, c_int), int(n_kpts_band_this, c_int), iks_band_this_c, &
         int(i_state_low - 1, c_int), int(i_state_high, c_int), vexx_band_c)
      if (n_kpts_band_this > 0) vexx_band = real(vexx_band_c, kind=dp)
      deallocate(iks_band_this_c, vexx_band_c)
   end subroutine get_exx_pot_band_k

   subroutine taesc_build_g0w0_sigma(this, opts)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts

      call require_handler(this, "taesc_build_g0w0_sigma")
      call ensure_options(opts)
      call taesc_build_g0w0_sigma_c(this%ptr_c_handle, opts%ptr_c_options)
   end subroutine taesc_build_g0w0_sigma

   subroutine get_g0w0_sigc_kgrid(this, opts, n_spins, iks_this, &
         i_state_low, i_state_high, vxc, vexx, sigc)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts
      integer, intent(in) :: n_spins, i_state_low, i_state_high
      integer, intent(in) :: iks_this(:)
      real(dp), intent(in) :: vxc(:, :, :), vexx(:, :, :)
      complex(dp), intent(inout) :: sigc(:, :, :)
      integer :: n_kpts_this, n_states_calc
      integer(c_int), allocatable :: iks_this_c(:)
      real(c_double), allocatable :: vxc_c(:,:,:), vexx_c(:,:,:)
      real(c_double), allocatable :: sigc_re(:,:,:), sigc_im(:,:,:)

      call require_handler(this, "get_g0w0_sigc_kgrid")
      call ensure_options(opts)
      n_kpts_this = size(iks_this)
      n_states_calc = i_state_high - i_state_low + 1
      call require_result_shape(size(sigc, 1), size(sigc, 2), size(sigc, 3), &
         n_states_calc, n_kpts_this, n_spins, "get_g0w0_sigc_kgrid")
      call require_result_shape(size(vxc, 1), size(vxc, 2), size(vxc, 3), &
         n_states_calc, n_kpts_this, n_spins, "get_g0w0_sigc_kgrid")
      call require_result_shape(size(vexx, 1), size(vexx, 2), size(vexx, 3), &
         n_states_calc, n_kpts_this, n_spins, "get_g0w0_sigc_kgrid")
      call make_c_indices(iks_this, iks_this_c)
      allocate(vxc_c(n_states_calc, max(1, n_kpts_this), n_spins))
      allocate(vexx_c(n_states_calc, max(1, n_kpts_this), n_spins))
      allocate(sigc_re(n_states_calc, max(1, n_kpts_this), n_spins))
      allocate(sigc_im(n_states_calc, max(1, n_kpts_this), n_spins))
      if (n_kpts_this > 0) then
         vxc_c = real(vxc, kind=c_double)
         vexx_c = real(vexx, kind=c_double)
      end if
      call taesc_get_g0w0_sigc_kgrid_c(this%ptr_c_handle, opts%ptr_c_options, &
         int(n_spins, c_int), int(n_kpts_this, c_int), iks_this_c, &
         int(i_state_low - 1, c_int), int(i_state_high, c_int), vxc_c, vexx_c, &
         sigc_re, sigc_im)
      if (n_kpts_this > 0) sigc = cmplx(sigc_re, sigc_im, kind=dp)
      deallocate(iks_this_c, vxc_c, vexx_c, sigc_re, sigc_im)
   end subroutine get_g0w0_sigc_kgrid

   subroutine get_g0w0_sigc_band_k(this, opts, n_spins, iks_band_this, &
         i_state_low, i_state_high, vxc_band, vexx_band, sigc_band)
      class(TaescHandler), intent(inout) :: this
      class(TaescOptions), intent(inout) :: opts
      integer, intent(in) :: n_spins, i_state_low, i_state_high
      integer, intent(in) :: iks_band_this(:)
      real(dp), intent(in) :: vxc_band(:, :, :), vexx_band(:, :, :)
      complex(dp), intent(inout) :: sigc_band(:, :, :)
      integer :: n_kpts_band_this, n_states_calc
      integer(c_int), allocatable :: iks_band_this_c(:)
      real(c_double), allocatable :: vxc_band_c(:,:,:), vexx_band_c(:,:,:)
      real(c_double), allocatable :: sigc_band_re(:,:,:), sigc_band_im(:,:,:)

      call require_handler(this, "get_g0w0_sigc_band_k")
      call ensure_options(opts)
      n_kpts_band_this = size(iks_band_this)
      n_states_calc = i_state_high - i_state_low + 1
      call require_result_shape(size(sigc_band, 1), size(sigc_band, 2), &
         size(sigc_band, 3), n_states_calc, n_kpts_band_this, n_spins, &
         "get_g0w0_sigc_band_k")
      call require_result_shape(size(vxc_band, 1), size(vxc_band, 2), &
         size(vxc_band, 3), n_states_calc, n_kpts_band_this, n_spins, &
         "get_g0w0_sigc_band_k")
      call require_result_shape(size(vexx_band, 1), size(vexx_band, 2), &
         size(vexx_band, 3), n_states_calc, n_kpts_band_this, n_spins, &
         "get_g0w0_sigc_band_k")
      call make_c_indices(iks_band_this, iks_band_this_c)
      allocate(vxc_band_c(n_states_calc, max(1, n_kpts_band_this), n_spins))
      allocate(vexx_band_c(n_states_calc, max(1, n_kpts_band_this), n_spins))
      allocate(sigc_band_re(n_states_calc, max(1, n_kpts_band_this), n_spins))
      allocate(sigc_band_im(n_states_calc, max(1, n_kpts_band_this), n_spins))
      if (n_kpts_band_this > 0) then
         vxc_band_c = real(vxc_band, kind=c_double)
         vexx_band_c = real(vexx_band, kind=c_double)
      end if
      call taesc_get_g0w0_sigc_band_k_c(this%ptr_c_handle, opts%ptr_c_options, &
         int(n_spins, c_int), int(n_kpts_band_this, c_int), iks_band_this_c, &
         int(i_state_low - 1, c_int), int(i_state_high, c_int), vxc_band_c, &
         vexx_band_c, sigc_band_re, sigc_band_im)
      if (n_kpts_band_this > 0) then
         sigc_band = cmplx(sigc_band_re, sigc_band_im, kind=dp)
      end if
      deallocate(iks_band_this_c, vxc_band_c, vexx_band_c)
      deallocate(sigc_band_re, sigc_band_im)
   end subroutine get_g0w0_sigc_band_k

   subroutine make_c_indices(f_indices, c_indices)
      integer, intent(in) :: f_indices(:)
      integer(c_int), allocatable, intent(out) :: c_indices(:)
      integer :: n

      n = size(f_indices)
      allocate(c_indices(max(1, n)))
      if (n > 0) c_indices(1:n) = int(f_indices(1:n), kind=c_int) - 1
   end subroutine make_c_indices

   subroutine require_result_shape(actual_1, actual_2, actual_3, expected_1, &
         expected_2, expected_3, routine)
      integer, intent(in) :: actual_1, actual_2, actual_3
      integer, intent(in) :: expected_1, expected_2, expected_3
      character(len=*), intent(in) :: routine

      if (actual_1 /= expected_1 .or. actual_2 /= expected_2) then
         error stop trim(routine) // ": result array has wrong shape"
      end if
      if (actual_3 /= expected_3) then
         error stop trim(routine) // ": result array has wrong shape"
      end if
   end subroutine require_result_shape

end module taesc_f03
