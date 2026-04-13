# Day 03: Reflecting Structs/Classes

## Core Question
> How can we programmatically inspect the members, types, and access levels of a class or struct at compile-time?

## Theory & Reflection API
- **`std::meta::nonstatic_data_members_of(info)`**: Returns a collection of meta-info objects representing all non-static fields (data members) of a class or struct. This includes members from all access specifiers (public, protected, private).
- **`std::meta::type_of(info)`**: When called on a member's meta-info, it returns the meta-info representing that member's type.
- **Access Specifiers**:
    - `std::meta::is_public(info)`
    - `std::meta::is_protected(info)`
    - `std::meta::is_private(info)`
    These functions return a `bool` and can be used to filter members or generate access-aware reports.

## Core Code Walkthrough
- **Reflecting on Class Members**:
    - `constexpr auto fields = std::meta::nonstatic_data_members_of(^Person);` retrieves all data members.
    - `constexpr std::meta::info type_info = std::meta::type_of(f);` is nested reflection — we reflect on the member to get its type's reflection.
- **Access Level Querying**:
    - `std::meta::is_private(fields[3])` is a powerful tool. Previously, checking member access required complex compiler-specific traits or was impossible.

## Expected Output Description
Running the code should show the layout of the `Person` struct:
```
--- Day 03: Reflecting Structs/Classes ---
Fields in 'Person' (5 total):
  - [public]: id (int)
  - [public]: name (std::string_view)
  - [public]: salary (double)
  - [private]: secret_code (int)
  - [protected]: is_verified (bool)

Verification: Struct layout successfully inspected!
```

## Pitfalls & Retrospective
- **Static Members**: `nonstatic_data_members_of` does NOT return static data members. To get those, use `members_of` and filter for `is_static`.
- **Inherited Members**: This query usually only returns members declared *directly* in the class. To handle inheritance, one would need to reflect on `bases_of(info)` and recursively query their members.

## Connections to Prior Knowledge
- Before P2996, this type of inspection was only possible using external code generators or extremely complex, non-standard library hacks (e.g., "Precise-Flat-Reflect").
- This is the fundamental building block for **serialization** and **Object-Relational Mapping (ORM)**.

## Self-Test Q&A
- **Q: Can reflection access the actual runtime values of these fields?**
- **A: Reflection gives you the metadata (name, type, etc.). To access the values of an instance, you need "splicing" or specialized access functions, which we will explore in Phase 3.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
