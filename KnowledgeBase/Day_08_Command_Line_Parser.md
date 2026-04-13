# Day 08: Command Line Parser

## Core Question
> How can reflection be used to eliminate the boilerplate of command-line argument parsing?

## Theory & Reflection API
- **Dynamic Mapping**: By reflecting on a struct's members, we can create a dynamic map between strings (CLI flags) and actual variables.
- **`std::meta::name_of` in runtime loops**: While reflection is compile-time, we can use the results (like field names) in runtime logic (like comparing with `argv`).
- **Splicing for Assignment**: `obj.[: f :] = value` allows us to write to any field of any struct dynamically, as long as we have its meta-info.

## Core Code Walkthrough
- **The Generic Parser Loop**:
    1. Reflect on the configuration struct `T`.
    2. Get all fields using `nonstatic_data_members_of`.
    3. Iterate through `argv`.
    4. For each `--flag`, loop through reflected fields.
    5. If names match:
        - Use `std::meta::type_of(f)` to determine the field's type.
        - Use `if constexpr` to apply the correct parsing logic (`std::from_chars` for ints, etc.).
        - Use `config.[: f :] = parsed_value` to update the instance.

## Expected Output Description
The program parses simulated arguments and updates the configuration struct:
```
--- Day 08: Command Line Parser ---
Simulating arguments: --port 8080 --host example.com --verbose
Warning: Unknown flag '--unknown-flag'

Parsed Configuration:
  - Port: 8080
  - Host: example.com
  - Verbose: true
  - Timeout: 5000ms
```

## Pitfalls & Retrospective
- **Ambiguity**: If two fields have the same name (not possible in a single struct, but possible with nested structs), more advanced logic would be needed.
- **Error Handling**: A production-ready parser would need robust error handling for malformed input (e.g., passing a string to an `int` port).
- **Aliases**: Supporting shorthand flags (e.g., `-p` for `--port`) would require custom attributes, which we explored in Day 04.

## Connections to Prior Knowledge
- This is the reflection-based alternative to libraries like `gflags` or `getopt`, which require manual registration of every flag.
- This is "declarative configuration"—you define your data structure, and the system automatically knows how to populate it from external input.

## Self-Test Q&A
- **Q: Can this handle nested configurations?**
- **A: Yes, with recursion (similar to the JSON serializer in Day 07).**
- **Q: How would you implement a `-h` help flag?**
- **A: Reflect on all members and print their names and types as part of a "Usage" message.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
