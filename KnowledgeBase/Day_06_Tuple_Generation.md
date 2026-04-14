# Day 06: Tuple Generation

## Core Question
> How can we use reflection to programmatically generate complex template instantiations (like `std::tuple`) from a collection of types?

## Theory & Reflection API
- **`std::meta::substitute(template_meta, args_meta_list)`**: Synthesis tool that takes a class template's meta-info and a `std::vector<std::meta::info>` of arguments. It returns the meta-info representing the resulting instantiation.
- **Heap Allocation**: Because argument lists like `std::vector<info>` are heap-allocated in this fork, they must be processed within `consteval` contexts.
- **Pack Expansion on Splicing**: In some implementations, `[: v :]...` can be used to expand a vector of info into template arguments.

## Core Code Walkthrough
- **Defining the Type List**:
    ```cpp
    constexpr std::vector<std::meta::info> types = { ^^T1, ^^T2, ^^T3 };
    ```
- **Substituting into `std::tuple`**:
    ```cpp
    constexpr auto t = std::meta::substitute(^^std::tuple, types);
    ```
    `t` now reflects `std::tuple<T1, T2, T3>`.
- **Splicing to create the alias**:
    ```cpp
    using MyTuple = [: t :];
    ```
    The compiler resolves this at compile-time to the actual instantiated `std::tuple`.

## Expected Output Description
The program generates a 3-element tuple and verifies its contents:
```
--- Day 06: Tuple Generation ---
User ID: 1
Settings Theme: Dark
Extra Int: 42

Verification: Tuple successfully generated from metadata!
```

## Pitfalls & Retrospective
- **Template Compatibility**: The arguments passed to `substitute` must match the template's expected parameters (e.g., number of arguments, type vs. non-type). If they don't, `substitute` returns an invalid meta-info or the compiler throws an error during splicing.
- **Recursive Substitution**: You can pass the result of one `substitute` as an argument to another, allowing for complex nested structures.

## Connections to Prior Knowledge
- Before P2996, creating a tuple from a type list required complex recursive templates and parameter pack manipulation (e.g., `std::tuple_cat` logic or custom type-list libraries).
- `substitute` makes template instantiation as easy as manipulating a vector.

## Self-Test Q&A
- **Q: Can `std::meta::substitute` be used for my custom templates?**
- **A: Yes. Any class template or alias template that can be reflected can be used with `substitute`.**
- **Q: What if the argument list is empty?**
- **A: If the template allows zero arguments (like a variadic template), it will succeed. Otherwise, it will fail at compile-time.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
