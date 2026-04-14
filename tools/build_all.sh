#!/usr/bin/env bash
# Script to build all C++26 Reflection labs in the project
# Usage: ./tools/build_all.sh [preset_name]

set -e

PRESET="${1:-clang-p2996-debug}"
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "🚀 Building ALL C++26 Reflection labs..."
echo "🛠️  Using preset: $PRESET"

# 1. Configure the project
echo "⚙️  Configuring..."
cmake --preset "$PRESET"

# 2. Build all targets
echo "🔨 Compiling all targets..."
cmake --build --preset "$PRESET" --parallel

# 3. List built executables
echo -e "\n✅ Build complete! Executables available in build/$PRESET/:"
# macOS find compatibility: use -perm +111 or similar, or just filter by type f
find "$PROJECT_ROOT/build/$PRESET" -maxdepth 1 -type f -perm +111 -not -name "*.so" -not -name "*.dylib" -not -name "*.dll" | sort
