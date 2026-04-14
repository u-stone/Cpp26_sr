// Day 02: Reflecting Enums
// [PLAN]: Use std::meta::enumerators_of with index-based extraction.

#include <experimental/meta>
#include <iostream>
#include <string_view>
#include <format>
#include <utility>

enum class Color {
    Red,
    Green,
    Blue,
    Cyan = 10,
    Magenta,
    Yellow
};

template <typename E, size_t I>
consteval std::string_view get_enum_name() {
    return std::meta::identifier_of(std::meta::enumerators_of(^^E)[I]);
}

template <typename E, size_t I>
consteval E get_enum_value() {
    return std::meta::extract<E>(std::meta::constant_of(std::meta::enumerators_of(^^E)[I]));
}

template <typename E>
constexpr std::string_view enum_to_string(E value) {
    constexpr auto count = std::meta::enumerators_of(^^E).size();
    std::string_view result = "Unknown";
    
    [&]<size_t... Is>(std::index_sequence<Is...>) {
        ( (get_enum_value<E, Is>() == value ? (void)(result = get_enum_name<E, Is>()) : (void)0), ... );
    }(std::make_index_sequence<count>{});
    
    return result;
}

int main() {
    std::cout << "--- Day 02: Reflecting Enums ---\n";

    constexpr auto color_count = std::meta::enumerators_of(^^Color).size();
    
    std::cout << "Enumerators of Color:\n";
    [&]<size_t... Is>(std::index_sequence<Is...>) {
        ( (std::cout << std::format("  - Name: {}, Value: {}\n", 
                     get_enum_name<Color, Is>(), 
                     (int)get_enum_value<Color, Is>())), ... );
    }(std::make_index_sequence<color_count>{});

    Color my_color = Color::Cyan;
    std::cout << std::format("\nMy current color is: {}\n", enum_to_string(my_color));

    return 0;
}
