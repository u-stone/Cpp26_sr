# C++26 Reflection Mastery Project (P2996)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A structured learning project to explore and master the C++26 Reflection APIs (P2996) using the **Deep Mastery Protocol (DMP)**.

## 🚀 Getting Started

### 1. Prerequisites
Since C++26 Reflection is not yet in mainline compilers, you MUST use a specialized fork:
- **Clang Fork**: [Bloomberg Clang P2996](https://github.com/bloomberg/clang-p2996)
- **CMake**: 3.25+
- **Ninja**: Recommended generator

### 2. Environment Setup
Configure your environment to point to the custom Clang binary. You can do this via `CMakePresets.json` or by setting `CMAKE_CXX_COMPILER`.

```bash
# Example using CMake Presets
cmake --preset clang-p2996-debug
cmake --build --preset clang-p2996-debug
```

### 3. Start a New Lesson
Use the scaffolding tool to create the next lab:
```bash
./tools/scaffold_lab.sh 01 Hello_Meta_Info
```

## 🧠 Learning Methodology (DMP)
This project follows the **Deep Mastery Protocol**, which enforces a rigorous loop of coding, verification, and distillation.
- **`Roadmap.md`**: Your syllabus and progress tracker.
- **`KnowledgeBase/`**: Deep-dive notes for each lesson.
- **`SyncState.md`**: Current session focus and handover dashboard.

## 📁 Directory Structure
- `src/`: Practice code for each lesson.
- `KnowledgeBase/`: Feynman-style notes for each lesson.
- `Engineering/`: Coding standards and environment setup.
- `Protocol/`: The DMP specification.

## ⚖️ License
MIT
