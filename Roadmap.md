# Learning Roadmap: C++26 Reflection (P2996)

*Objective: Master the core APIs of the upcoming C++26 reflection proposal through incremental, code-driven labs.*

---

## Phase 1: Basics and Queries (Day 01 – Day 02)
*Goal: Understand how to obtain meta-information and query simple properties.*

- **[CORE] Day 01: Hello Meta-Info**
    - **Topic**: Setup environment, use the reflection operator `^`, and extract basic `std::meta::info`.
    - **Core concepts**: `^T`, `std::meta::info`, `std::meta::name_of`.

- **[CORE] Day 02: Reflecting Enums**
    - **Topic**: Extract enumerators, convert enum values to strings, and iterate over all values.
    - **Core concepts**: `std::meta::enumerators_of`, iteration over `std::vector<std::meta::info>`.

---

## Phase 2: Types and Members (Day 03 – Day 04)
*Goal: Inspect user-defined types (structs and classes).*

- **[CORE] Day 03: Reflecting Structs/Classes**
    - **Topic**: Extract data members, iterate over fields, and check access specifiers.
    - **Core concepts**: `std::meta::nonstatic_data_members_of`, `std::meta::is_public`.

- **Day 04: Attributes and Functions**
    - **Topic**: Querying C++ attributes (`[[...]]`) and member functions.
    - **Core concepts**: `std::meta::has_attribute`, `std::meta::members_of`.

---

## Phase 3: Synthesis and Splicing (Day 05 – Day 06)
*Goal: Generate code from meta-information (Code Synthesis).*

- **[CORE] Day 05: Type Synthesis (`[: :]`)**
    - **Topic**: Generating types and values dynamically using the splice operator.
    - **Core concepts**: `[: m :]`, splicing meta-info back into the language.

- **Day 06: Tuple Generation**
    - **Topic**: Creating a custom struct or `std::tuple` dynamically from a list of reflected types.
    - **Core concepts**: Parameter pack expansion with reflection, `std::meta::substitute`.

---

## Phase 4: Practical Applications (Day 07 – Day 08)
*Goal: Build real-world utilities using reflection.*

- **[CORE] Day 07: JSON Serializer**
    - **Topic**: Implement a generic `to_json` function that works for any struct.
    - **Core concepts**: Recursive reflection-based serialization.

- **Day 08: Command Line Parser**
    - **Topic**: Automatically map struct fields to CLI arguments.
    - **Core concepts**: Integrating reflection with `std::format` and CLI logic.
