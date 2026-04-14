# Day 04: Attributes and Functions

## Core Question
> How can we reflect on member functions and C++ attributes (`[[...]]`) to create metadata-aware logic?

## Theory & Reflection API
- **`std::meta::members_of(info)`**: Returns a `std::vector<std::meta::info>` of *all* members, including functions, nested types, and static data members. These vectors are heap-allocated and must be processed within `consteval` contexts.
- **`std::meta::is_function(info)`**: A query to determine if the meta-info represents a function or member function.
- **`std::meta::is_static(info)`**: Determines if a member is static.
- **`std::meta::has_attribute(info, attr_info)`**: Checks if a declaration has a specific attribute. The attribute itself can be reflected using `^^attribute_name`.

## Core Code Walkthrough
- **Universal Member Inspection**:
    - `constexpr auto all_members = std::meta::members_of(^^MyService);` catches everything as a reflection collection.
    - `is_function(all_members[I])` demonstrates filtering via index-based access.
- **Attribute-Driven Logic**:
    - `std::meta::has_attribute(m, ^^deprecated)` allows the compiler to detect if a function should be ignored or treated specially.
    - Class-level attributes like `[[nodiscard]]` can also be queried.

## Expected Output Description
The program categorizes the members of `MyService` and identifies their properties:
```
--- Day 04: Attributes and Functions ---
MyService is marked as [[nodiscard]]

Members of 'MyService' (4 total):
  - [Function]: start 
  - [Function]: stop (DEPRECATED)
  - [Function]: static get_version 
  - [Field]: data

Verification: Functions and attributes successfully queried!
```

## Pitfalls & Retrospective
- **Attribute Arguments**: Current P2996 implementations focus on the *presence* of attributes. Extracting the *arguments* of an attribute (e.g., the string in `[[deprecated("reason")]]`) is more complex and may involve additional experimental APIs.
- **Overloads**: If a function is overloaded, `members_of` will return multiple entries with the same name. Disambiguating them requires checking signatures.

## Connections to Prior Knowledge
- Attributes were previously "invisible" to the C++ type system. You could use them to tell the compiler something, but you couldn't use the type system to react to them.
- This enables **Compile-time Tagging**: using custom attributes to mark fields for "SQL primary key", "JSON ignore", etc.

## Self-Test Q&A
- **Q: Can I reflect on local variables' attributes?**
- **A: No. Reflection in P2996 primarily targets declarations (types, members, namespaces, globals). Local variables are not currently reflectable.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
