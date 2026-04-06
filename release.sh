#!/bin/bash

# Release build script for Zap compiler
# Uses Release mode with -O2 and prefers static LLVM component libraries.

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_ROOT="$SCRIPT_DIR/build-release"
BUILD_DIR="$BUILD_ROOT/out"
OVERLAY_DIR="$BUILD_ROOT/source-overlay"

echo -e "${YELLOW}Building Zap compiler in release mode...${NC}"

if [ ! -d "$BUILD_ROOT" ]; then
    echo -e "${YELLOW}Creating release build directory...${NC}"
    mkdir -p "$BUILD_ROOT"
fi

mkdir -p "$BUILD_DIR" "$OVERLAY_DIR"

ln -sfn "$SCRIPT_DIR/src" "$OVERLAY_DIR/src"
ln -sfn "$SCRIPT_DIR/std" "$OVERLAY_DIR/std"
ln -sfn "$SCRIPT_DIR/cmake" "$OVERLAY_DIR/cmake"
ln -sfn "$SCRIPT_DIR/Doxygen" "$OVERLAY_DIR/Doxygen"

perl -0pe 's/# Some distros \(e\.g\. Arch Linux\) provide llvm as one shared library rather than multiple ones\.\nif\(LLVM_LINK_LLVM_DYLIB\)\n    # Single\n    set\(llvm_libs LLVM\)\nelse\(\)\n    # Multiple\n    llvm_map_components_to_libnames\(llvm_libs \$\{LLVM_COMPONENTS\}\)\nendif\(\)/# Forced by release.sh: always use LLVM component libraries.\nllvm_map_components_to_libnames(llvm_libs \${LLVM_COMPONENTS})/s' \
    "$SCRIPT_DIR/CMakeLists.txt" > "$OVERLAY_DIR/CMakeLists.txt"

cd "$BUILD_ROOT"

CPU_COUNT=$(getconf _NPROCESSORS_ONLN 2>/dev/null || nproc 2>/dev/null || echo 1)
BUILD_JOBS=$((CPU_COUNT - 1))
if [ "$BUILD_JOBS" -lt 1 ]; then
    BUILD_JOBS=1
fi

echo -e "${YELLOW}Running CMake for Release (-O2, static LLVM preference)...${NC}"
cmake \
    -S "$OVERLAY_DIR" \
    -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
    -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" \
    -DLLVM_LINK_LLVM_DYLIB=OFF

echo -e "${YELLOW}Compiling...${NC}"
cmake --build "$BUILD_DIR" --parallel "$BUILD_JOBS"

if [ -f "$BUILD_DIR/zapc" ]; then
    rm -f "$BUILD_ROOT/zapc" "$BUILD_ROOT/zap-lsp"
    rm -rf "$BUILD_ROOT/std"
    cp "$BUILD_DIR/zapc" "$BUILD_ROOT/zapc"
    cp "$BUILD_DIR/zap-lsp" "$BUILD_ROOT/zap-lsp"
    mkdir -p "$BUILD_ROOT/std"
    cp -a "$SCRIPT_DIR/std/." "$BUILD_ROOT/std/"
    echo -e "${GREEN}Release build successful!${NC}"
    echo -e "${GREEN}Executable: $BUILD_ROOT/zapc${NC}"
else
    echo -e "${RED}Release build failed!${NC}"
    exit 1
fi
