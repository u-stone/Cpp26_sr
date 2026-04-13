// Day 02: Reflecting Enums
// [PLAN]: Use std::meta::enumerators_of to inspect an enum.
// We'll iterate over all enumerators and print their names.

#include <experimental/meta>
#include <print>
#include <string_view>
#include <vector>
#include <algorithm>

enum class Color {
    Red,
    Green,
    Blue,
    Cyan = 10,
    Magenta,
    Yellow
};

// A helper function to find the name of an enum value via reflection
template <typename E>
constexpr std::string_view enum_to_string(E value) {
    // 1. Reflect on the enum type
    constexpr std::meta::info enum_info = ^E;
    
    // 2. Get all enumerators (returns a std::vector<std::meta::info> in P2996)
    // Note: In some versions it returns a collection that can be iterated.
    constexpr auto enumerators = std::meta::enumerators_of(enum_info);
    
    // 3. Search for the enumerator whose value matches the input
    for (auto e : enumerators) {
        if (std::meta::value_of<E>(e) == value) {
            return std::meta::name_of(e);
        }
    }
    
    return "Unknown";
}

int main() {
    std::println("--- Day 02: Reflecting Enums ---");

    // 1. Iterate over all enumerators of 'Color'
    constexpr auto color_enumerators = std::meta::enumerators_of(^Color);
    
    std::println("Enumerators of Color:");
    for (auto e : color_enumerators) {
        // e is of type std::meta::info
        std::println("  - Name: {}, Value: {}", 
                     std::meta::name_of(e), 
                     (int)std::meta::value_of<Color>(e));
    }

    // 2. Demonstrate enum-to-string conversion
    Color my_color = Color::Cyan;
    std::println("\nMy current color is: {}", enum_to_string(my_color));

    // 3. Static verification
    static_assert(std::meta::enumerators_of(^Color).size() == 6);
    static_assert(enum_to_string(Color::Red) == "Red");
    static_assert(enum_to_string(Color::Yellow) == "Yellow");

    std::println("\nVerification: All enumerators matched correctly!");

    return 0;
}
