# Engineering Standards

## 🛠️ Toolchain
- **Compiler**: Clang (Bloomberg P2996 Reflection Fork)
- **Standard**: C++26 (`-std=c++2c`)
- **Flags**: `-freflection`, `-Wall`, `-Wextra`, `-Werror`, `-pedantic`
- **Build System**: CMake 3.25+ (Ninja generator)

## 🎨 Coding Style
- **Naming**: 
  - Classes: `PascalCase`
  - Functions: `camelCase`
  - Variables: `snake_case`
  - Constants/Macros: `SCREAMING_SNAKE_CASE`
- **Formatting**: LLVM-based (see `.clang-format`)
- **Headers**: Use `#pragma once`. Include order: Standard Library, Third-party, Project headers.

## 🧪 Testing & Verification
- **Per-Lesson**: Each lesson must have a `main.cpp` that demonstrates the core concept.
- **Verification**: Use `static_assert` where possible for reflection results. Use `std::print` (C++23) for runtime output.

## 🛡️ Safety
- **Zero-Placeholder**: No `TODO` or `...` in final commits.
- **Warnings**: All warnings treated as errors.
