// Day 01: Hello Meta-Info
// [PLAN]: Demonstrate the basic reflection operator '^' and std::meta::info type.
// We will reflect on built-in types, custom structs, and use compile-time queries.

#include <experimental/meta> // Header for P2996 Reflection
#include <print>
#include <string_view>

// A simple user-defined type for reflection
struct MyAwesomeStruct {
    int id;
    float value;
};

int main() {
    // 1. Obtain meta-information using the reflection operator '^'
    // The result is of type std::meta::info, which is a scalar type suitable for constexpr.
    constexpr std::meta::info int_info = ^int;
    constexpr std::meta::info struct_info = ^MyAwesomeStruct;

    // 2. Query properties of the meta-information at compile-time
    // std::meta::name_of returns a std::string_view of the entity's name.
    constexpr std::string_view int_name = std::meta::name_of(int_info);
    constexpr std::string_view struct_name = std::meta::name_of(struct_info);

    // 3. Runtime demonstration
    std::println("--- Day 01: Hello Meta-Info ---");
    std::println("Reflecting on built-in type: {}", int_name);
    std::println("Reflecting on custom struct: {}", struct_name);

    // 4. Compile-time verification using static_assert
    // Reflection is inherently a constant expression.
    static_assert(std::meta::name_of(^int) == "int");
    static_assert(std::meta::name_of(^MyAwesomeStruct) == "MyAwesomeStruct");

    // Check if it's a type
    static_assert(std::meta::is_type(int_info));
    static_assert(std::meta::is_type(struct_info));

    std::println("Verification: All static_asserts passed!");

    return 0;
}
