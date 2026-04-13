#!/usr/bin/env bash
# DMP Lab Scaffolder for C++26 Reflection
# Usage: ./tools/scaffold_lab.sh <day_number> <topic_name>
set -e

DAY="$1"
TOPIC="$2"
DIR="src/${DAY}_${TOPIC}"

[[ -z "$DAY" || -z "$TOPIC" ]] && echo "Usage: $0 <day> <topic>" && exit 1
[[ -d "$DIR" ]] && echo "Directory $DIR already exists!" && exit 1

mkdir -p "$DIR"

# Generate main.cpp
cat > "$DIR/main.cpp" << EOF
// Day ${DAY}: ${TOPIC}
// [PLAN]: TODO — fill in this lesson's implementation goal

#include <print>
#include <string_view>

int main() {
    std::println("Day ${DAY}: ${TOPIC} initialized.");
    return 0;
}
EOF

# Generate KnowledgeBase template
cat > "KnowledgeBase/Day_${DAY}_${TOPIC}.md" << EOF
# Day ${DAY}: ${TOPIC}

## Core Question
> TODO: What problem does this lesson solve?

## Theory & Reflection API
<!-- P2996 primitives used: ^, [: :], std::meta::... -->

## Core Code Walkthrough
<!-- Analysis of the reflection logic implemented -->

## Expected Output Description
<!-- Descriptive summary of runtime or compile-time results -->

## Pitfalls & Retrospective
<!-- Issues with experimental Clang or P2996 syntax -->

## Connections to Prior Knowledge
<!-- Relations to templates or constexpr metaprogramming -->

## Self-Test Q&A
<!-- Q: ... A: ... -->
EOF

# Add to root CMakeLists.txt (if it exists)
if [[ -f "CMakeLists.txt" ]]; then
    # Very simple append; in a real project we'd use a more robust way
    echo "" >> CMakeLists.txt
    echo "add_executable(${DAY}_${TOPIC} src/${DAY}_${TOPIC}/main.cpp)" >> CMakeLists.txt
fi

chmod +x "tools/scaffold_lab.sh"
echo "✅ Scaffolded: $DIR"
echo "📝 KB template: KnowledgeBase/Day_${DAY}_${TOPIC}.md"
