#!/usr/bin/env bash
set -euo pipefail

# The preset records the verified WSL OpenFHE package location. Run this script
# from any directory; the repository root is resolved from the script location.
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${PROJECT_DIR}"

cmake --preset wsl-release
cmake --build --preset wsl-release
ctest --preset wsl-release
"${PROJECT_DIR}/build/ckks_starter"

