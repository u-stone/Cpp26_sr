# Day 02: Reflecting Enums

## Core Question
> How can we iterate over all members of an enum and convert their values back to strings without manual mappings?

## Theory & Reflection API
- **`std::meta::enumerators_of(info)`**: Returns a `std::vector<std::meta::info>` of all enumerators for a given enum type. Since these results are heap-allocated in this fork, they should be processed within `consteval` contexts.
- **`std::meta::extract<T>(info)`**: Extracts the underlying value of an enumerator or constant from its metadata. This is key for mapping runtime values back to their metadata.
- **Index-based Extraction**: We typically access reflection results using indices (e.g., `enumerators_of(^^E)[i]`) within helper functions that can be expanded using `std::index_sequence`.

## Core Code Walkthrough
- **Enum-to-String Helper**:
    - `constexpr auto count = std::meta::enumerators_of(^^E).size();`: Gets the total number of constants in the enum.
    - `return std::meta::identifier_of(std::meta::enumerators_of(^^E)[I]);`: Returns the identifier at index `I`.
    - `if (get_enum_value<E, Is>() == value)`: Compares the runtime value with the value extracted from the metadata at index `Is`.
    - `return get_enum_name<E, Is>();`: Returns the name if a match is found.

## Expected Output Description
Running the code should output the full list of colors and their explicit values, then demonstrate a specific conversion:
```
--- Day 02: Reflecting Enums ---
Enumerators of Color:
  - Name: Red, Value: 0
  - Name: Green, Value: 1
  - Name: Blue, Value: 2
  - Name: Cyan, Value: 10
  - Name: Magenta, Value: 11
  - Name: Yellow, Value: 12

My current color is: Cyan
```

## Pitfalls & Retrospective
- **Value Collisions**: If multiple enumerators have the same value, `enum_to_string` will return the first one it finds.
- **Performance**: While `enum_to_string` in this example iterates through the list, the reflection part happens at compile-time. For large enums, the compiler work increases.
- **Header Selection**: Continued use of `<experimental/meta>`.

## Connections to Prior Knowledge
- Before P2996, this required macros (like `X Macros`) or third-party libraries (like `MagicEnum`), which relied on compiler-specific `__PRETTY_FUNCTION__` hacks.
- This is a formal, standard way to do what `MagicEnum` does.

## Self-Test Q&A
- **Q: Does `std::meta::enumerators_of` work on non-enum types?**
- **A: No. Attempting to call it on a non-enum type meta-info will result in a compile-time error.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
