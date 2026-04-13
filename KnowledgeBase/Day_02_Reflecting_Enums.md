# Day 02: Reflecting Enums

## Core Question
> How can we iterate over all members of an enum and convert their values back to strings without manual mappings?

## Theory & Reflection API
- **`std::meta::enumerators_of(info)`**: Returns a sequence (usually a `std::vector<std::meta::info>`) of all enumerators for a given enum type.
- **`std::meta::value_of<T>(info)`**: Extracts the underlying value of an enumerator. It returns the value as type `T`. This is key for mapping runtime values back to their metadata.
- **Iteration over Metadata**: Since `std::meta::info` is a compile-time value, we can use standard C++ iteration (like range-based `for`) to process metadata collections.

## Core Code Walkthrough
- **Enum-to-String Helper**:
    - `constexpr auto enumerators = std::meta::enumerators_of(^E);`: Gets the metadata for all constants in the enum.
    - `for (auto e : enumerators)`: Loops through the metadata.
    - `if (std::meta::value_of<E>(e) == value)`: Compares the runtime value with the value stored in the metadata.
    - `return std::meta::name_of(e);`: Returns the name if a match is found.

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
Verification: All enumerators matched correctly!
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
