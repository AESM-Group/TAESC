#!/usr/bin/env bash
# Download LibRPA third-party source archives for offline builds.
# Run on a machine with network access, then copy thirdparty/archives/ to the
# build host and configure with -DLIBRPA_THIRDPARTY_DOWNLOAD=OFF.
# Usage: scripts/prepare_thirdparty.sh [COMPONENTS]   (default: "greenX;ELPA")
# Add LibDDLA to COMPONENTS for HIP/CUDA builds.
set -euo pipefail
root=$(cd "$(dirname "$0")/.." && pwd)
components=${1:-"greenX;ELPA"}
archive_dir=${TAESC_THIRDPARTY_ARCHIVE_DIR:-"${root}/thirdparty/archives"}
mkdir -p "${archive_dir}"
cmake -DCOMPONENTS="${components}" \
      -DLIBRPA_THIRDPARTY_ARCHIVE_DIR="${archive_dir}" \
      -P "${root}/src/LibRPA/cmake/LibRPADownloadThirdparty.cmake"
echo "Archives in ${archive_dir}"
