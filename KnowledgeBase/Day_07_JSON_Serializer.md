# Day 07: JSON Serializer

## Core Question
> How can we combine reflection queries, member splicing, and recursion to create a universal serializer for any C++ struct?

## Theory & Reflection API
- **The "Killer Feature" of Reflection**: This is the culmination of all previous lessons. A generic serializer demonstrates why P2996 is transformative for C++.
- **Accessing field values dynamically**: We use the member splice operator `obj.[: field_info :]` within a loop to access each field's value of an instance without knowing the field names at compile-time.
- **Type-aware Formatting**: By reflecting on each field's type, we can decide how to format its value (e.g., adding quotes for strings, formatting bools as "true"/"false").

## Core Code Walkthrough
- **Generic `to_json<T>`**:
    - `constexpr auto fields = std::meta::nonstatic_data_members_of(^T);` gets all the metadata for fields in struct `T`.
    - `for (size_t i = 0; i < fields.size(); ++i)` iterates through the metadata.
    - `obj.[: fields[i] :]` is the magic: it tells the compiler to access the field on the `obj` instance that corresponds to the metadata `fields[i]`.
- **Handling recursion**:
    - By combining reflection with `if constexpr` and template recursion, we can automatically handle nested structs (like `Address` inside `User`).

## Expected Output Description
The program outputs a properly formatted JSON string for a complex nested struct:
```json
--- Day 07: JSON Serializer ---
Generated JSON for User:
{
  "id": 101,
  "name": "John Doe",
  "is_active": true,
  "home_address": {
    "city": "Neo-Tokyo",
    "zip_code": 12345
  }
}
```

## Pitfalls & Retrospective
- **Access Control**: This simple version only works for `public` members. To handle private members, you'd need the struct to be a friend of the serializer or use specialized access hooks.
- **Circular Dependencies**: Recursive serializers need protection against circular references (not an issue for simple data structs, but common in graph-like objects).
- **Arrays and Containers**: A full-featured serializer would need additional logic to handle `std::vector` and arrays by reflecting on whether a type is a container.

## Connections to Prior Knowledge
- Before P2996, you'd need massive libraries like `nlohmann/json` and would have to use `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MyType, field1, field2, ...)` for *every* type you want to serialize.
- With P2996, serialization is "zero-boilerplate"—it just works for any type.

## Self-Test Q&A
- **Q: Does this work for classes with private members?**
- **A: `nonstatic_data_members_of` will return private members, but the splicing `obj.[: f :]` will fail to compile if the member is private from the perspective of the `to_json` function.**

## Asset Sources
- P2996 Proposal: "Reflection for C++26"
