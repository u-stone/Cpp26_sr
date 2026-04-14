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
We use a project-local toolchain to ensure consistency. Use our automation script to install the custom Clang fork and its runtime into the `.toolchain/` directory:

```bash
# Install Clang P2996 into .toolchain/ and auto-configure CMakePresets.json
./tools/install_reflection_compiler.sh
```

For a detailed breakdown of the environment, see the **[Setup Summary](Engineering/Setup.md)**.

### 3. Build & Run
You can build all experiments or run a specific one using our helper scripts:

```bash
# Build all 8 labs
./tools/build_all.sh

# Build and run a specific lab (e.g., Day 07)
./tools/run_lab.sh 07
```

## 🧠 Learning Methodology (DMP)
This project follows the **Deep Mastery Protocol**, which enforces a rigorous loop of coding, verification, and distillation.

- **[Roadmap.md](Roadmap.md)**: Your 8-day syllabus and progress tracker.
- **[KnowledgeBase/](KnowledgeBase/)**: Feynman-style deep-dive notes for each lesson.
- **[SyncState.md](SyncState.md)**: Current session focus and handover dashboard.
- **[Retrospective](Engineering/Retrospective.md)**: **CRITICAL** - A record of all build issues, API changes, and lessons learned during environment setup.

### Start a New Lesson
Use the scaffolding tool to create the next lab:
```bash
./tools/scaffold_lab.sh XX Topic_Name
```

## 📁 Directory Structure
- `src/`: Practice code for each lesson.
- `KnowledgeBase/`: Detailed analysis of reflection primitives.
- `Engineering/`:
    - `Setup.md`: Environment configuration guide.
    - `Standards.md`: C++26 coding and project standards.
    - `Retrospective.md`: Log of mistakes and their solutions.
- `Protocol/`: The DMP specification.
- `tools/`: Automation scripts (`install`, `scaffold`, `build_all`, `run_lab`).

## ⚖️ License
MIT
