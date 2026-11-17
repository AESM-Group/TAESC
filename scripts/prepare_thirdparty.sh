#!/usr/bin/env bash
# Download third-party source archives (cereal, greenX, ELPA) for offline builds.
# Run on a machine with network access, then copy thirdparty/archives/ to the
# build host and configure with -DTAESC_THIRDPARTY_DOWNLOAD=OFF.
# Usage: scripts/prepare_thirdparty.sh [COMPONENTS]   (default: "cereal;greenX;ELPA")
# Add LibDDLA to COMPONENTS for HIP/CUDA builds.
set -euo pipefail
root=$(cd "$(dirname "$0")/.." && pwd)
components=${1:-"cereal;greenX;ELPA"}
archive_dir=${TAESC_THIRDPARTY_ARCHIVE_DIR:-"${root}/thirdparty/archives"}
mkdir -p "${archive_dir}"
cmake -DCOMPONENTS="${components}" \
      -DLIBRPA_THIRDPARTY_ARCHIVE_DIR="${archive_dir}" \
      -P "${root}/src/LibRPA/cmake/LibRPADownloadThirdparty.cmake"
echo "Archives in ${archive_dir}"
