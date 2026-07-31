# TAESC 用户手册

本文档面向 TAESC 0.2.0 原型版本，介绍软件用途、运行环境、安装构建、命令行驱动、API 集成方式、输入输出、异常处理和常见问题。TAESC 当前主要封装并调度项目内置的 LibRPA 能力；若底层 LibRPA 的接口或数据格式发生变化，请以当前源码和构建产物为准。

## 1. 引言

### 1.1 编写目的

TAESC 是先进电子结构计算套件 (**Toolkit for Advanced Electronic Structure Calculations**) 的缩写。
本手册用于帮助用户完成以下工作：

1. 理解 TAESC 的定位、功能范围和适用场景。
2. 在本地或集群环境中构建 TAESC。
3. 使用 `taesc-driver` 从文件数据发起 RPA、EXX、$G^0W^0$ 等后处理计算。
4. 在 C、C++ 或 Fortran 程序中通过 TAESC API 集成高级电子结构计算能力。
5. 处理常见的输入、构建、运行和并行环境问题。

### 1.2 适用对象

本手册适用于以下用户：

- 使用第一性原理软件导出数据并运行后处理计算的计算材料、计算化学或凝聚态物理用户。
- 需要在宿主电子结构程序中调用 RPA、EXX、$G^0W^0$ 能力的开发者。
- 负责安装 TAESC、配置 MPI/BLAS/LAPACK/ScaLAPACK 环境的系统管理员。
- 需要评审 TAESC 原型功能、接口和运行流程的测试人员。

### 1.3 软件概述

TAESC 当前提供两类使用路径：

- **命令行驱动路径**：用户准备 `taesc.in`，TAESC 将其转换为 LibRPA 识别的 `librpa.in`，并调用构建目录中的 `chi0_main.exe` 执行任务。
- **API 集成路径**：宿主程序通过 `include/taesc/` 下的 C/C++ 头文件或 Fortran 2003 模块传入结构、能级、波函数、RI 系数、库仑矩阵等数据，然后调用计算函数获取结果。

当前支持的主要计算任务包括：

- RPA 关联能计算。
- EXX 精确交换势计算。
- $G^0W^0$ 准粒子能量和能带计算。

### 1.4 术语说明

| 术语 | 含义 |
| --- | --- |
| TAESC | 高级电子结构计算工具包，本项目的顶层接口和驱动层。 |
| LibRPA | TAESC 当前封装的底层 RPA/GW 计算库。 |
| LibRI | 用于局域分辨率近似及张量收缩的计算库。 |
| LibComm | LibRI 依赖的并行通信辅助库。 |
| RI/LRI | Resolution of Identity / Localized Resolution of Identity，用于降低计算复杂度的辅助基展开技术。 |
| EXX | Exact Exchange，精确交换。 |
| RPA | Random Phase Approximation，随机相位近似。 |
| $G^0W^0$ | 非自恰 GW 准粒子修正方法。 |
| SCF | Self-Consistent Field，自洽场计算。 |

## 2. 运行环境

### 2.1 硬件环境

TAESC 面向中小规模本地测试和高性能计算集群运行。实际硬件需求随体系大小、k 点数、态数、辅助基大小、频率网格数和并行策略变化。

建议环境如下：

| 场景 | CPU | 内存 | 磁盘 |
| --- | --- | --- | --- |
| 编译和小体系功能测试 | 4 核及以上 | 8 GB 及以上 | 10 GB 及以上 |
| 中等规模计算 | 多核 CPU 或多节点 MPI | 32 GB 及以上 | 视数据集大小预留数十 GB |
| 大规模 GW/RPA 计算 | HPC 集群，多节点 MPI + OpenMP | 按体系规模评估 | 建议使用高速并行文件系统 |

### 2.2 软件环境

构建和运行 TAESC 通常需要：

- 支持 C++17 的 C++ 编译器。
- 支持 Fortran 2003 的 Fortran 编译器。
- MPI 编译和运行环境，例如 OpenMPI、MPICH、Intel MPI。
- BLAS、LAPACK 和 ScaLAPACK。
- CMake 3.13 或更高版本。
- Python 3，用于运行 `driver/taesc-driver.py` 或安装后的 `taesc-driver`。

项目内已包含或通过源码树配置的组件包括：

- `src/LibRPA`：底层 RPA/GW 功能。
- `src/LibRI` 和 `src/LibComm`：LibRI 相关能力。
- `thirdparty/cereal-d81e2f7`：序列化依赖。

### 2.3 运行前检查

在构建前建议确认以下命令可用：

```bash
cmake --version
mpicxx --version
mpifort --version
python3 --version
```

还需要确认 BLAS/LAPACK/ScaLAPACK 的库路径可被 CMake 找到，或在配置阶段显式传入 `BLAS_LIBRARIES`、`LAPACK_LIBRARIES` 和 `SCALAPACK_DIR`。

## 3. 安装与启动

### 3.1 获取源码

进入源码目录：

```bash
cd /path/to/TAESC-prototype
```

如果源码来自 Git 仓库并包含子模块，请按仓库要求初始化子模块。当前原型仓库中主要依赖已经位于源码树内。

### 3.2 CMake 构建选项

TAESC 顶层 CMake 提供以下常用选项：

| 选项 | 默认值 | 说明 |
| --- | --- | --- |
| `TAESC_ENABLE_LIBRPA` | `ON` | 启用内置 LibRPA 后端。当前主要功能依赖该选项。 |
| `TAESC_ENABLE_TEST` | `OFF` | 构建测试。开发或验证时可打开。 |
| `TAESC_ENABLE_FORTRAN_BIND` | `ON` | 构建 Fortran 2003 绑定。 |
| `TAESC_FORTRAN_DP` | `c_double` | Fortran 绑定中双精度 kind 设置。 |
| `BUILD_SHARED_LIBS` | CMake 默认 | 控制生成静态库或动态库。 |
| `CMAKE_INSTALL_PREFIX` | 系统默认 | 安装前缀。 |

### 3.3 基本构建步骤

最小构建流程如下：

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4
```

若系统无法自动找到数学库，可参考以下形式显式指定：

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release \
  -DBLAS_LIBRARIES="-L/path/to/blas -lopenblas" \
  -DLAPACK_LIBRARIES="-L/path/to/lapack -lopenblas" \
  -DSCALAPACK_DIR="/path/to/scalapack" \
  -DCMAKE_CXX_COMPILER=mpicxx \
  -DCMAKE_Fortran_COMPILER=mpifort
cmake --build build -j 4
```

在 macOS、Linux 或集群环境中，编译器包装器和库路径可能不同，需要按本机路径调整命令行变量。

### 3.4 安装

如需安装到指定目录：

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build build -j 4
cmake --install build
```

安装后通常包含：

- `bin/taesc-driver`：命令行驱动脚本。
- `lib/libtaesc.*`：TAESC 库。
- `include/taesc/*.h` 和 `include/taesc/*.hpp`：C/C++ 头文件。
- Fortran 模块和库文件，前提是启用了 `TAESC_ENABLE_FORTRAN_BIND`。

### 3.5 构建产物位置

未安装时，常用产物位于构建目录中：

- TAESC 库：`build/src/libtaesc.a` 或 `build/src/libtaesc.*`。
- Fortran 模块：`build/binding/fortran/taesc_f03.mod`。
- LibRPA 驱动：`build/src/LibRPA/chi0_main.exe`。

`taesc-driver` 运行时会在构建目录下查找 `chi0_main.exe`，因此使用 `-B` 或 `--build-dir` 时应传入包含该文件的 TAESC 构建目录。

### 3.6 启动和退出

命令行模式通过 `taesc-driver` 启动。安装后运行：

```bash
taesc-driver -B /path/to/build --dry-run
```

源码树内也可直接运行：

```bash
python3 /path/to/TAESC-prototype/driver/taesc_driver.py -B /path/to/build --dry-run
```

正常计算完成后程序会自动退出，并返回底层驱动的退出码。若使用 MPI 启动，必要时可通过调度系统或 MPI 工具终止任务。

## 4. 命令行驱动使用

### 4.1 工作目录

命令行驱动默认在当前工作目录读取 `taesc.in`。执行前建议准备一个独立工作目录，例如：

```bash
mkdir run-taesc
cd run-taesc
```

工作目录通常包含：

- `taesc.in`：TAESC 输入文件。
- `librpa.in`：由 TAESC 自动生成，不建议手动编辑。
- 输入数据目录：例如由 FHI-aims 或 ABACUS 导出的 LibRPA 数据集。
- 计算输出文件和日志。

### 4.2 `taesc.in` 格式

`taesc.in` 采用简单的键值格式：

```ini
task = g0w0_band
input_dir = dataset
nfreq = 16
replace_w_head = t
```

格式规则：

- 每行一个 `key = value`。
- 空行会被忽略。
- `#` 或 `!` 后面的内容视为注释。
- 参数名大小写会被归一化为小写。
- 同一参数重复出现时，后出现的值覆盖前面的值。
- 字符串值可以使用单引号或双引号包裹。

### 4.3 必填参数

| 参数 | 说明 | 示例 |
| --- | --- | --- |
| `task` | 计算任务类型。 | `task = rpa` |

当前 `taesc-driver` 支持的任务如下：

| 任务 | 说明 |
| --- | --- |
| `rpa` | RPA 关联能计算。 |
| `g0w0` | k 网格上的一次性 $G^0W^0$ 准粒子能量。 |
| `g0w0_band` | 一次性 $G^0W^0$ 准粒子能带。 |
| `exx` | k 网格上的精确交换计算。 |
| `exx_band` | 能带 k 点上的精确交换计算。 |

### 4.4 常用可选参数

| 参数 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `input_dir` | string | `taesc.in` 所在目录 | 输入数据目录。相对路径按 `taesc.in` 所在目录解析。 |
| `output_dir` | string | `taesc.d` | 结果输出目录。 |
| `restart_from_dir` | string | `output_dir` | 读取重启检查点的目录；空字符串表示使用输出目录。 |
| `output_level` | string | `info` | 全局输出级别：`silent`、`critical`、`warn`、`info` 或 `debug`。 |
| `nfreq` | int | `6` | 虚频积分网格点数。 |
| `tfgrids_type` | string | 底层默认 | 时间-频率网格类型，可选 `GL`、`split-GL`、`GC-I`、`GL-II`、`minimax`、`evenspaced`、`evenspaced_tf`。 |
| `vq_threshold` | double | `0.0` | 库仑矩阵筛选阈值。 |
| `gf_threshold` | double | `0.0` | 格林函数筛选阈值。 |
| `use_scalapack_ecrpa` | bool | `true` | RPA 关联能计算是否使用 ScaLAPACK。 |

命令行驱动会把 `task` 和 `input_dir` 之外的参数继续写入生成的 `librpa.in`。因此，某些仅由底层 LibRPA driver 识别的参数也可以放在 `taesc.in` 中。API 路径的 `taesc_set_option` 支持当前 `LibrpaOptions` 的全部公开字段，以及全局 `output_level`；未知参数仍会报错。

### 4.5 时间频率网格与 GW 参数

| 参数 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `n_params_anacon` | int | `-1` | 解析延拓使用的参数数量，`-1` 表示按底层默认使用频率数据。 |
| `n_params_anacon_resample` | int | `-1` | 重采样频率网格的 Padé 参数数量。 |
| `anacon_tfgrids_type` | string | `unset` | GW 解析延拓可选的独立频率网格类型。 |
| `anacon_nfreq` | int | `-1` | GW 解析延拓频率网格点数。 |
| `option_qpe_solver` | int | `0` | 准粒子方程求解器：`0` 残差混合、`1` 准牛顿、`2` 微扰线性化。 |
| `qpe_solver_thres` | double | `1e-6` | 准粒子方程收敛阈值（Hartree）。 |
| `qpe_solver_n_iter_max` | int | `10000` | 准粒子方程最大迭代次数。 |
| `qpe_solver_damp_factor` | double | `0.1` | 准粒子方程更新阻尼因子。 |
| `use_qpe_adaptive_damp` | bool | `false` | 是否自适应调整准粒子方程阻尼。 |
| `use_hedin_shift` | bool | `false` | 是否在准粒子求解中计算并使用 Hedin shift。 |
| `option_dielect_func` | int | `0` | 虚频轴介电函数处理方式。 |
| `sqrt_coulomb_threshold` | double | `0.0` | 对库仑矩阵做平方根处理时的本征值阈值。 |
| `use_kpara_scf_eigvec` | bool | `false` | 是否使用按 k 点并行分布的 SCF 本征矢。 |
| `use_scalapack_gw_wc` | bool | `true` | GW 中计算屏蔽库仑相互作用时是否使用 ScaLAPACK。 |
| `replace_w_head` | bool | `false` | 是否用宏观介电函数替换介电矩阵 head。 |
| `sf_gf_omega_shift` | double | `0.01` | 谱函数 Green 函数展宽（Hartree）。 |
| `sf_sigc_omega_shift` | double | `0.01` | 谱函数相关自能展宽（Hartree）。 |

### 4.6 LibRI 阈值参数

当使用 `parallel_routing = libri` 或相关低标度算法时，可按需设置以下筛选阈值：

| 参数 | 说明 |
| --- | --- |
| `libri_chi0_threshold_C` | 响应函数计算中 LRI 三中心系数阈值。 |
| `libri_chi0_threshold_G` | 响应函数计算中格林函数阈值。 |
| `libri_exx_threshold_C` | EXX 计算中 LRI 三中心系数阈值。 |
| `libri_exx_threshold_D` | EXX 计算中密度矩阵阈值。 |
| `libri_exx_threshold_V` | EXX 计算中库仑矩阵阈值。 |
| `libri_g0w0_threshold_C` | $G^0W^0$ 自能计算中 LRI 三中心系数阈值。 |
| `libri_g0w0_threshold_G` | $G^0W^0$ 自能计算中格林函数阈值。 |
| `libri_g0w0_threshold_Wc` | $G^0W^0$ 自能计算中屏蔽库仑矩阵阈值。 |

阈值越大，计算和存储成本通常越低，但结果误差可能增大。生产计算前建议对目标体系做收敛性测试。

### 4.7 驱动器运行命令

查看驱动帮助：

```bash
taesc-driver --help
```

本地 dry run，只生成 `librpa.in` 并打印将要执行的命令：

```bash
taesc-driver -B /path/to/build --dry-run
```

使用 4 个 MPI 进程运行：

```bash
taesc-driver -B /path/to/build -n 4
```

指定 MPI 启动器：

```bash
taesc-driver -B /path/to/build --launcher srun -n 16
```

指定工作目录。此时 `librpa.in` 会写入该目录，底层 LibRPA 也会在该目录中运行：

```bash
taesc-driver -B /path/to/build -W /path/to/workdir -n 8
```

如果未安装 `taesc-driver`，可从源码树运行：

```bash
python3 /path/to/TAESC-prototype/driver/taesc-driver.py -B /path/to/build -n 4
```

### 4.8 dry run 输出说明

`--dry-run` 会打印类似信息：

```text
TAESC task       : g0w0_band (One-shot $G^0W^0$ quasi-particle band structure)
LibRPA driver    : /path/to/build/src/LibRPA/chi0_main.exe
Work directory   : /path/to/work
Generated input  : /path/to/work/librpa.in
Command          : mpirun -np 4 /path/to/build/src/LibRPA/chi0_main.exe
```

确认任务类型、驱动路径、工作目录和 MPI 命令无误后，再去掉 `--dry-run` 发起正式计算。

### 4.9 并行和线程设置

TAESC 驱动可通过 `-n` 设置 MPI rank 数。OpenMP 线程数由环境变量控制，例如：

```bash
export OMP_NUM_THREADS=4
taesc-driver -B /path/to/build -n 8
```

这表示使用 8 个 MPI 进程，每个进程 4 个 OpenMP 线程。集群环境中还应遵守作业调度系统对节点数、核心数和内存的分配。

## 5. API 集成使用

### 5.1 API 组成

TAESC 的公共接口位于 `include/taesc/`：

| 文件 | 说明 |
| --- | --- |
| `taesc.h` | C 主入口头文件。 |
| `taesc.hpp` | C++ 封装。 |
| `taesc_global.h` | 全局初始化、结束和版本信息。 |
| `taesc_options.h` | 运行参数对象。 |
| `taesc_handler.h` | 计算句柄对象。 |
| `taesc_input.h` | 输入数据设置函数。 |
| `taesc_compute.h` | 计算函数。 |
| `binding/fortran/taesc_f03.f90` | Fortran 2003 模块源文件。 |

API 使用的两个核心对象是：

- `TaescOptions` / `taesc::Options` / `type(TaescOptions)`：保存运行参数。
- `TaescHandler` / `taesc::Handler` / `type(TaescHandler)`：保存输入数据和计算状态。

### 5.2 API 基本流程

API 集成的通用流程如下：

1. 初始化 MPI。
2. 调用 `taesc_init_global()` 或 `taesc::init_global()` 初始化 TAESC 全局环境。
3. 创建并配置 `TaescOptions`。
4. 创建 `TaescHandler`，并传入 MPI communicator。
5. 设置 SCF 维度、能级、占据数、波函数、晶格、原子、k 点、RI 系数和库仑矩阵等输入。
6. 根据任务调用 RPA、EXX、$G^0W^0$ 或频率网格相关计算函数。
7. 释放 handler 和 options。
8. 调用 `taesc_finalize_global()` 或 `taesc::finalize_global()`。
9. 结束 MPI。

若底层 LibRI 并行功能要求 MPI 线程支持，宿主程序应考虑使用 `MPI_Init_thread` 并请求 `MPI_THREAD_MULTIPLE`。

### 5.3 C++ 调用示意

以下代码仅展示调用骨架，数组内容需要由宿主电子结构程序提供：

```cpp
#include <taesc/taesc.hpp>
#include <mpi.h>
#include <complex>
#include <vector>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    taesc::init_global();

    taesc::Options opts;
    opts.set("nfreq", "16");
    opts.set("parallel_routing", "libri");
    opts.set("output_dir", "taesc.out");

    taesc::Handler h(MPI_COMM_WORLD);

    h.set_scf_dimension(nspins, nkpts, nstates, nbasis);
    h.set_wg_ekb_efermi(nspins, nkpts, nstates, wg, ekb, efermi);
    h.set_latvec_and_G(lat_mat, G_mat);
    h.set_atoms(types, pos_cart);
    h.set_kgrids_kvec(nk1, nk2, nk3, kvecs, kweights);
    h.set_kq_mapping(map_q_ks);
    h.set_ao_basis_wfc(nbs_wfc);
    h.set_ao_basis_aux(nbs_aux);

    for (int ispin = 0; ispin < nspins; ++ispin) {
        for (int ik = 0; ik < nkpts; ++ik) {
            h.set_wfc(ispin, ik, nstates_local, nbasis_local,
                      wfc_real, wfc_imag);
        }
    }

    // 设置 LRI 系数和库仑矩阵，具体循环由宿主程序的数据组织决定。
    h.set_lri_coeff(I, J, nbasis_i, nbasis_j, naux_mu, R, Cs);
    h.set_aux_bare_coulomb_k_atom_pair(ik, I, J, naux_mu, naux_nu,
                                       Vq_real, Vq_imag, vq_threshold);

    std::vector<std::complex<double>> contrib(nkpts_ibz);
    double ec_rpa = h.get_rpa_correlation_energy(opts, contrib);

    h.build_exx(opts);
    std::vector<double> vexx =
        h.get_exx_pot_kgrid(opts, nspins, iks_this, i_state_low, i_state_high);

    h.free();
    taesc::finalize_global();

    MPI_Finalize();
    return 0;
}
```

### 5.4 C 接口对象管理

C 接口使用显式创建和销毁：

```c
TaescOptions *opts = taesc_create_options();
taesc_set_option(opts, "nfreq", "16");

TaescHandler *h = taesc_create_handler(MPI_COMM_WORLD);

/* 设置输入并调用计算函数 */

taesc_destroy_handler(h);
taesc_destroy_options(opts);
```

所有 `taesc_set_*` 和 `taesc_get_*` 函数都要求传入有效的 handler 或 options 指针。不要手动修改结构体中的 `instance_id_` 字段。

### 5.5 Fortran 调用示意

Fortran 绑定以 `taesc_f03` 模块形式提供：

```f90
use mpi
use taesc_f03

implicit none

type(TaescOptions) :: opts
type(TaescHandler) :: h
integer :: ierr

call MPI_Init(ierr)

call taesc_init_global()

call opts%init()
call opts%set("nfreq", 16)
call opts%set("parallel_routing", "libri")

call h%init(MPI_COMM_WORLD)
call h%set_scf_dimension(nspins, nkpts, nstates, nbasis)
call h%set_wg_ekb_efermi(nspins, nkpts, nstates, wg, ekb, efermi)
! 继续设置晶格、原子、k 点、波函数、LRI 系数和库仑矩阵。

call h%free()
call opts%free()

call taesc_finalize_global()
call MPI_Finalize(ierr)
```

### 5.6 输入数据设置函数分类

| 类别 | 代表函数 | 说明 |
| --- | --- | --- |
| SCF 维度与能级 | `taesc_set_scf_dimension`、`taesc_set_wg_ekb_efermi` | 设置自旋数、k 点数、态数、基函数数、自旋分量数、占据数、能级和费米能。 |
| 波函数 | `taesc_set_wfc`、`taesc_set_wfc_packed`、`taesc_set_wfc_spinor` | 设置普通或旋量波函数，支持实部/虚部分开或复数打包输入。 |
| 原子与基组 | `taesc_set_atoms`、`taesc_set_ao_basis_wfc`、`taesc_set_ao_basis_aux`、`taesc_set_ao_basis_aux_shrink` | 设置原子、轨道基、辅助基、压缩辅助基及角动量壳层元数据。 |
| 晶格、对称性与 k 点 | `taesc_set_latvec_and_G`、`taesc_set_symmetry_operations`、`taesc_set_kgrids_kvec`、`taesc_set_kq_mapping` | 设置晶格、实空间对称操作、已载入 SCF k 点及其到库仑 q 点的映射。 |
| RI 与库仑矩阵 | `taesc_set_lri_coeff`、`taesc_set_aux_bare_coulomb_k_atom_pair`、`taesc_set_aux_cut_coulomb_k_atom_pair` | 设置 LRI 三中心系数以及裸/截断库仑矩阵。 |
| 能带数据 | `taesc_set_band_kvec`、`taesc_set_band_occ_eigval`、`taesc_set_wfc_band` | 设置能带路径上的 k 点、占据数、能级和波函数。 |
| 介电函数与速度矩阵 | `taesc_set_dielect_func_imagfreq`、`taesc_set_velocity_matrix` | 设置虚频介电函数及 head/wing 修正所需的速度矩阵。 |

### 5.7 计算函数分类

| 类别 | 函数 | 输出 |
| --- | --- | --- |
| 频率网格 | `taesc_get_imaginary_frequency_grids` | 虚频点和权重。 |
| RPA | `taesc_get_rpa_correlation_energy` | 总 RPA 关联能及不可约 k 点贡献。 |
| EXX | `taesc_build_exx`、`taesc_get_exx_pot_kgrid`、`taesc_get_exx_pot_band_k` | k 网格或能带 k 点上的 EXX 势。 |
| $G^0W^0$ | `taesc_build_g0w0_sigma`、`taesc_get_g0w0_sigc_*`、`taesc_get_g0w0_qpe_*`、`taesc_get_g0w0_spectral_function_*` | 相关自能、准粒子能量和谱函数。 |

## 6. 典型操作流程

### 6.1 命令行基本流程

1. 使用 FHI-aims、ABACUS 或其他接口导出 LibRPA 所需数据。
2. 构建 TAESC，确认 `chi0_main.exe` 已生成。
3. 新建工作目录并写入 `taesc.in`。
4. 使用 `taesc-driver -B /path/to/build --dry-run` 检查路径和命令。
5. 设置 MPI 进程数和 `OMP_NUM_THREADS`。
6. 去掉 `--dry-run` 发起正式计算。
7. 查看标准输出、`librpa.in` 和输出目录中的结果文件。

### 6.2 示例：运行 $G^0W^0$ 能带任务

准备 `taesc.in`：

```ini
task = g0w0_band
input_dir = /path/to/dataset
nfreq = 16
replace_w_head = t
```

检查命令：

```bash
taesc-driver -B /path/to/TAESC/build --dry-run
```

正式运行：

```bash
export OMP_NUM_THREADS=2
taesc-driver -B /path/to/TAESC/build -n 8
```

运行完成后，`taesc-driver` 会保留生成的 `librpa.in`，用于追踪实际传入 LibRPA 的参数。

### 6.3 示例：仅生成 LibRPA 输入文件

如果只是想检查 TAESC 如何转换输入参数：

```bash
taesc-driver -B /path/to/build --dry-run
```

此命令不会启动底层计算，但会写入或更新工作目录中的 `librpa.in`。

## 7. 输入输出说明

### 7.1 输入来源

TAESC 命令行路径需要的数据包括：

- `taesc.in`：任务和运行参数。
- `input_dir` 指向的数据集：由外部电子结构程序导出，包括结构、能级、占据数、波函数、k 点、RI 系数和库仑矩阵等。
- MPI/OpenMP 环境：由命令行参数和环境变量提供。

API 路径不读取 `taesc.in`；宿主程序需要通过 setter 函数传入所有必需数据。

### 7.2 外部数据准备

LibRPA 格式数据可由支持的第一性原理程序导出：

- FHI-aims：在 `control.in` 中启用 `output librpa`。
- ABACUS：在 `INPUT` 中设置 `rpa 1`。

具体导出内容和文件格式依赖外部程序版本。运行 TAESC 前应确认 `input_dir` 中的数据与所选任务匹配。

### 7.3 输出文件

命令行驱动会生成：

- `librpa.in`：由 `taesc.in` 转换得到的底层输入文件。
- 标准输出：任务类型、驱动路径、工作目录、生成输入文件路径、实际执行命令和计算日志。
- 输出目录中的结果文件：由 LibRPA 根据任务和 `output_dir` 写入。

API 路径的输出由宿主程序接收，例如数组、返回值或宿主程序自行写出的文件。

### 7.4 路径解析规则

`taesc-driver` 的路径规则如下：

- `taesc.in` 总是从当前工作目录读取。
- `input_dir` 若为相对路径，则相对于 `taesc.in` 所在目录解析。
- `--work-dir` 若未指定，则默认使用 `taesc.in` 所在目录。
- `librpa.in` 写入工作目录。
- `--build-dir` 应指向 TAESC 构建目录，驱动会在常见位置查找 LibRPA 驱动器程序 `chi0_main.exe`。

## 8. 异常情况与处理

| 现象 | 可能原因 | 处理方法 |
| --- | --- | --- |
| `taesc.in must define a 'task' parameter` | `taesc.in` 缺少 `task`。 | 添加 `task = rpa`、`task = g0w0_band` 等有效任务。 |
| `unknown task` | `task` 拼写错误或当前版本不支持。 | 使用 `rpa`、`g0w0`、`g0w0_band`、`exx`、`exx_band`。 |
| `input file does not exist` | 当前目录没有 `taesc.in`。 | 切换到工作目录，或在当前目录创建 `taesc.in`。 |
| `expected a 'key = value' assignment` | 输入行缺少等号。 | 按 `key = value` 格式修正。 |
| `empty value for parameter` | 某个参数没有值。 | 补充完整值或删除该行。 |
| `build directory does not exist` | `-B` 指向不存在的目录。 | 传入正确的构建目录。 |
| `could not find chi0_main.exe` | TAESC/LibRPA 未成功构建，或构建目录传错。 | 重新构建，并确认存在 `build/src/LibRPA/chi0_main.exe`。 |
| `mpirun` 或 `srun` 找不到 | MPI 环境未加载。 | 加载 MPI module，或使用 `--launcher` 指定正确启动器。 |
| ScaLAPACK 链接失败 | 数学库路径未配置或 ABI 不匹配。 | 检查 `BLAS_LIBRARIES`、`LAPACK_LIBRARIES`、`SCALAPACK_DIR` 和编译器/MPI 组合。 |
| 运行时找不到动态库 | `LD_LIBRARY_PATH`、`DYLD_LIBRARY_PATH` 或 rpath 未配置。 | 将安装目录或依赖库目录加入运行时库路径。 |
| 计算结果不收敛或偏差较大 | 频率网格、阈值或输入数据精度不足。 | 增大 `nfreq`，收紧筛选阈值，并检查外部数据导出设置。 |

## 9. 注意事项

- `librpa.in` 是由 TAESC 生成的中间文件。通过 TAESC 运行时，请优先修改 `taesc.in`。
- 生产计算前应对 `nfreq` 和各类筛选阈值进行体系相关的收敛性测试。
- MPI 进程数和 OpenMP 线程数应结合节点硬件、内存和底层数学库线程策略设置，避免过度超线程。
- API 中数组维度和索引约定必须与头文件声明及宿主程序数据布局一致；维度错误可能导致崩溃或无意义结果。
- 在同一进程中使用 API 时，应保证全局初始化和结束成对出现，handler 和 options 对象及时释放。
- 当前 TAESC 仍是原型接口层，部分高级输出控制参数由底层 LibRPA 管理，实际支持情况应以源码实现为准。

## 10. 常见问题

### Q1：TAESC 和 LibRPA 是什么关系？

TAESC 当前是面向高级电子结构计算的顶层接口和驱动层，主要封装项目内置 LibRPA 的 RPA、EXX、$G^0W^0$ 等能力。命令行模式会把 `taesc.in` 转换为 `librpa.in`，再调用 LibRPA 驱动。

### Q2：为什么 `--dry-run` 也会生成 `librpa.in`？

`--dry-run` 的设计目标是检查输入转换和执行命令，因此它会写出 `librpa.in`，但不会启动底层 `chi0_main.exe`。

### Q3：`input_dir` 可以写相对路径吗？

可以。相对路径按 `taesc.in` 所在目录解析，然后再转换为相对于实际工作目录的路径写入 `librpa.in`。

### Q4：如何选择 `nfreq`？

`nfreq` 控制虚频积分网格数。较大的 `nfreq` 通常提高精度但增加计算量。建议从小体系测试开始，对目标物理量做收敛性检查。

### Q5：API 和命令行能混用吗？

通常二者是两种入口。命令行从文件数据启动底层驱动；API 由宿主程序直接传入内存数据。一次具体计算一般选择其中一种路径。

### Q6：如何确认当前库版本？

C 接口可调用 `taesc_get_major_version()`、`taesc_get_minor_version()`、`taesc_get_patch_version()`；C++ 接口可调用 `taesc::get_major_version()` 等函数。当前源码版本号定义在 `src/version.h`。

## 11. 附录

### 11.1 `taesc-driver` 命令参考

```text
usage: taesc-driver [-h] -B BUILD_DIR [-n NP] [--launcher LAUNCHER]
                    [-W WORK_DIR] [--dry-run]
```

| 参数 | 说明 |
| --- | --- |
| `-h`, `--help` | 显示帮助信息。 |
| `-B`, `--build-dir` | TAESC 构建目录，必须能找到 LibRPA 驱动 `chi0_main.exe`。 |
| `-n`, `--np` | MPI rank 数。若不指定 `--launcher`，默认使用 `mpirun -np NP`。 |
| `--launcher` | MPI 启动命令，例如 `mpirun`、`srun`。 |
| `-W`, `--work-dir` | `librpa.in` 写入目录和底层计算运行目录。 |
| `--dry-run` | 只生成 `librpa.in` 并打印命令，不启动计算。 |

### 11.2 `taesc.in` 完整示例

```ini
# TAESC example input
task = g0w0_band
input_dir = dataset
output_dir = taesc.out

parallel_routing = libri
nfreq = 16
tfgrids_type = minimax

use_scalapack_ecrpa = true
use_scalapack_gw_wc = true
vq_threshold = 0.0
gf_threshold = 0.0

libri_chi0_threshold_C = 0.0
libri_chi0_threshold_G = 0.0
libri_exx_threshold_C = 0.0
libri_exx_threshold_D = 0.0
libri_exx_threshold_V = 0.0
libri_g0w0_threshold_C = 0.0
libri_g0w0_threshold_G = 0.0
libri_g0w0_threshold_Wc = 0.0
```

### 11.3 API 链接示例

若 TAESC 已安装到 `/path/to/install`，C++ 程序可按本地构建系统配置 include 和 link 路径。手动编译形式示意如下：

```bash
mpicxx main.cpp \
  -I/path/to/install/include \
  -L/path/to/install/lib -ltaesc \
  -o main.exe
```

实际链接命令可能还需要加入 Fortran runtime、BLAS、LAPACK、ScaLAPACK、LibRPA 或平台相关库。推荐在宿主项目中使用 CMake 管理依赖。

### 11.4 相关目录

| 路径 | 说明 |
| --- | --- |
| `README.md` | 项目简介。 |
| `CMakeLists.txt` | TAESC 顶层构建配置。 |
| `driver/taesc_driver.py` | 命令行驱动源码。 |
| `driver/taesc.in` | 示例 TAESC 输入文件。 |
| `include/taesc/` | C/C++ 公共头文件。 |
| `binding/fortran/` | Fortran 绑定。 |
| `src/taesc/` | TAESC C/C++ 实现。 |
| `src/LibRPA/docs/` | 底层 LibRPA 文档，可用于了解数据格式和后端参数。 |
