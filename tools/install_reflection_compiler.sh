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

echo "⚙️  Configuring LLVM/Clang (Release mode, Clang only)..."
cmake -G Ninja \
      -DLLVM_ENABLE_PROJECTS=clang \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" \
      ../llvm

echo "🔨 Building Clang (this may take a long time)..."
ninja clang
echo "📦 Installing to $INSTALL_PREFIX..."
ninja install

# 3. Update CMakePresets.json in the project root
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PRESETS_FILE="$PROJECT_ROOT/CMakePresets.json"
CLANG_PATH="$INSTALL_PREFIX/bin/clang++"

if [ -f "$PRESETS_FILE" ]; then
    echo "📝 Updating CMakePresets.json with new compiler path..."
    # Use sed to replace the CMAKE_CXX_COMPILER value. 
    # Note: This is a simple regex that assumes the structure created in bootstrap.
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
