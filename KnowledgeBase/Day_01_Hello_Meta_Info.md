# Day 01: Hello Meta-Info

## Core Question
> How do we obtain and query basic type information in the C++26 Reflection (P2996) proposal?

## Theory & Reflection API
- **The Reflection Operator (`^^`)**: This is the primary entry point to reflection in the Bloomberg fork. It produces an object of type `std::meta::info`.
- **`std::meta::info`**: A scalar type representing a reflected entity.
- **Reflection Queries**:
    - `std::meta::display_string_of(info)`: Returns a `std::string_view` of the entity's name (works for built-in types).
    - `std::meta::identifier_of(info)`: Returns the identifier (works for declarations like members).
    - `std::meta::is_type(info)`: Returns `true` if the reflected entity is a type.
- **Immediate Functions (`consteval`)**: Many reflection functions are `consteval` and return `std::vector<info>`, which is heap-allocated and cannot be stored in long-lived `constexpr` variables in this fork. These results should be processed within `consteval` contexts to avoid leaking metadata into runtime variables.
- **Index-based Extraction**: Since reflection collections are vectors, they are indexed using `[i]`. This is often used with `std::index_sequence` for expansion.

## Core Code Walkthrough
- **Metadata Extraction Helper**:
    ```cpp
    template <typename T, size_t I>
    consteval std::string_view get_member_name() {
        return std::meta::identifier_of(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I]);
    }
    ```
    This helper retrieves the identifier of the member at index `I` entirely at compile-time.
- **Iteration via Index Sequence**:
    The `dump_members` function uses `std::make_index_sequence` to expand a parameter pack of indices, allowing us to call our `consteval` helper for each member without storing a heap-allocated vector.
- `constexpr std::meta::info int_info = ^^int;`: Obtaining meta-info for a built-in type remains a direct operation.

## Expected Output Description
When run, the program should print the names of the reflected types and confirm that all compile-time checks passed:
```
--- Day 01: Hello Meta-Info ---
Member 0: id
Member 1: value
```

## Pitfalls & Retrospective
- **Header Selection**: In the Bloomberg fork, reflection functions are in the `<experimental/meta>` header (following P2996 naming), not the standard C++ `<meta>`.
- **Zero Evaluation**: Remember that `^^T` does not instantiate `T` or perform any runtime actions. It is purely a compile-time construct.

## Connections to Prior Knowledge
- This replaces the old `typeid(T).name()` which was runtime-based, implementation-defined (mangled names), and not `constexpr` friendly.
- It provides a way to do "metaprogramming" without complex template meta-programming (TMP) like `std::is_same_v` or `std::is_integral_v` manually.

## Self-Test Q&A
- **Q: Can `std::meta::info` be used at runtime?**
- **A: No. It is intended for compile-time manipulation. While you can pass it to functions, those functions must generally be `constexpr` for the reflection logic to be useful.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
