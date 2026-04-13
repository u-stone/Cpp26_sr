# Day 05: Type Synthesis ([: :])

## Core Question
> How can we turn meta-information (`std::meta::info`) back into actual C++ types and expressions?

## Theory & Reflection API
- **The Splice Operator (`[: :]`)**: This is the inverse of the reflection operator (`^`). It "splices" metadata back into the source code at compile-time.
- **Type Splicing**: When `m` reflects a type, `[: m :]` can be used anywhere a type-name is allowed (e.g., variable declarations, template arguments).
- **Expression Splicing**: When `m` reflects a value or a member, `[: m :]` can be used in an expression.
    - **Member Access**: `instance.[: m :]` allows accessing a data member or calling a member function reflected by `m`.
- **Compile-time Guarantee**: The content inside `[: :]` must be a constant expression of type `std::meta::info`.

## Core Code Walkthrough
- **Creating types**:
    ```cpp
    constexpr std::meta::info t = ^int;
    [: t :] x = 5; // Compiler sees 'int x = 5;'
    ```
- **Accessing members**:
    ```cpp
    constexpr std::meta::info field = std::meta::nonstatic_data_members_of(^S)[0];
    S instance;
    instance.[: field :] = 10; // Compiler sees 'instance.member_name = 10;'
    ```
    This is extremely powerful because it allows us to write code that operates on members whose names we don't know at the time of writing (e.g., in a generic function).

## Expected Output Description
The program demonstrates that spliced types behave exactly like the original types and that members can be accessed using their metadata:
```
--- Day 05: Type Synthesis ([: :]) ---
Spliced type variable value: 42
Record id: 1, table: users
Extracted via splicing - id: 1, table: users

Verification: Type and member splicing successful!
```

## Pitfalls & Retrospective
- **Context Sensitivity**: The splice operator is context-sensitive. If you use it where a type is expected, it must reflect a type. If you use it where an expression is expected, it must reflect a value or member.
- **Ambiguity**: If `m` is not a valid meta-info for the context (e.g., trying to splice a namespace as a type), the compiler will emit an error.

## Connections to Prior Knowledge
- This replaces many uses of `decltype` and complex template metaprogramming.
- In traditional C++, to access a member based on some condition, you would need complex pointer-to-member logic or large `switch` statements. Splicing makes this direct.

## Self-Test Q&A
- **Q: Can I use `[: :]` with a runtime-calculated `std::meta::info`?**
- **A: No. The operand must be a constant expression (`constexpr`) because splicing happens during compilation.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
