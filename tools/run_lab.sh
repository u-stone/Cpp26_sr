#!/usr/bin/env bash
# Script to build and run a specific C++26 Reflection lab
# Usage: ./tools/run_lab.sh <day_number> [preset_name]
# Example: ./tools/run_lab.sh 01

set -e

DAY_INPUT="$1"
PRESET="${2:-clang-p2996-debug}"
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"

if [[ -z "$DAY_INPUT" ]]; then
    echo "❌ Usage: $0 <day_number> [preset_name]"
    echo "Example: $0 01"
    exit 1
fi

# 1. Resolve the full lab name from the src directory
# We search for directories starting with the day number (e.g., 01_...)
LAB_DIR=$(find "$PROJECT_ROOT/src" -maxdepth 1 -type d -name "${DAY_INPUT}_*" -print -quit)

if [[ -z "$LAB_DIR" ]]; then
    echo "❌ Error: Could not find lab for day '$DAY_INPUT' in src/."
    exit 1
fi

LAB_NAME=$(basename "$LAB_DIR")
EXECUTABLE="$PROJECT_ROOT/build/$PRESET/$LAB_NAME"

echo "🚀 Day $DAY_INPUT: $LAB_NAME"
echo "🛠️  Configuring and Building with preset '$PRESET'..."

# 2. Configure (if needed) and Build the specific target
# We use --target to only build the specific lab executable to save time
cmake --preset "$PRESET"
cmake --build --preset "$PRESET" --target "$LAB_NAME"

# 3. Run the executable
if [[ -f "$EXECUTABLE" ]]; then
    echo -e "\n🏃 Running $LAB_NAME...\n"
    echo "--------------------------------------------------"
    "$EXECUTABLE"
    echo -e "--------------------------------------------------\n"
    echo "✅ Lab execution finished successfully."
else
    echo "❌ Error: Executable not found at $EXECUTABLE"
    exit 1
fi
