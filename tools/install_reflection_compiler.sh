#!/usr/bin/env bash
# Script to install Bloomberg Clang P2996 and configure the project
# Usage: ./tools/install_reflection_compiler.sh [INSTALL_DIR]

set -e

# 🛠️ Configuration
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
INSTALL_PREFIX="${1:-$PROJECT_ROOT/.toolchain/clang-p2996}"
BUILD_DIR="/tmp/clang-p2996-build"
REPO_URL="https://github.com/bloomberg/clang-p2996.git"

echo "🚀 Starting installation of Clang P2996..."
echo "📍 Install Prefix: $INSTALL_PREFIX"
echo "🛠️  Build Directory: $BUILD_DIR"

# 1. Clone the repository
if [ ! -d "$BUILD_DIR" ]; then
    echo "📥 Cloning Bloomberg Clang P2996..."
    git clone --depth 1 --branch p2996 "$REPO_URL" "$BUILD_DIR"
else
    echo "📂 Build directory already exists, skipping clone."
fi

# 2. Build and Install
mkdir -p "$BUILD_DIR/build"
cd "$BUILD_DIR/build"

echo "⚙️  Configuring LLVM/Clang + runtimes (Release mode)..."
# We must include libcxx, libcxxabi, and libunwind to get the reflection headers and runtime support
cmake -G Ninja \
      -DLLVM_ENABLE_PROJECTS="clang" \
      -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" \
      ../llvm

echo "🔨 Building Clang and runtimes (this will take a while)..."
# Build the compiler and the runtimes
ninja clang cxx cxxabi unwind

echo "📦 Installing to $INSTALL_PREFIX..."
# Install the compiler and the runtimes
ninja install-clang install-cxx install-cxxabi install-unwind

# 3. Update CMakePresets.json in the project root
PRESETS_FILE="$PROJECT_ROOT/CMakePresets.json"
# We point to the local toolchain using ${sourceDir} for portability
CLANG_PATH="\${sourceDir}/.toolchain/clang-p2996/bin/clang++"

if [ -f "$PRESETS_FILE" ]; then
    echo "📝 Updating CMakePresets.json with new compiler path..."
    # Note: On macOS, sed -i requires an empty string for the extension if you don't want a backup
    if [[ "$OSTYPE" == "darwin"* ]]; then
        sed -i '' "s|\"CMAKE_CXX_COMPILER\": \".*\"|\"CMAKE_CXX_COMPILER\": \"$CLANG_PATH\"|g" "$PRESETS_FILE"
    else
        sed -i "s|\"CMAKE_CXX_COMPILER\": \".*\"|\"CMAKE_CXX_COMPILER\": \"$CLANG_PATH\"|g" "$PRESETS_FILE"
    fi
    echo "✅ CMakePresets.json updated."
else
    echo "⚠️  Warning: CMakePresets.json not found in project root."
fi

echo "✨ Installation and configuration complete!"
echo "👉 You can now run: cmake --preset clang-p2996-debug"
