// Day 07: JSON Serializer
// [PLAN]: Use index-sequence expansion with separated extraction and printing.

#include <experimental/meta>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <format>
#include <utility>

struct Address {
    std::string_view city;
    int zip_code;
};

struct User {
    int id;
    std::string_view name;
    bool is_active;
    Address home_address;
};

template <typename T>
void to_json(const T& obj, int indent = 2);

template <typename T, size_t I>
consteval std::meta::info get_field_info() {
    return std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I];
}

template <typename T>
void print_json_value(const T& val) {
    if constexpr (std::is_same_v<T, std::string_view> || std::is_same_v<T, std::string>) {
        std::cout << std::format("\"{}\"", val);
    } else if constexpr (std::is_same_v<T, bool>) {
        std::cout << (val ? "true" : "false");
    } else if constexpr (std::is_arithmetic_v<T>) {
        std::cout << val;
    } else {
        to_json(val, 4);
    }
}

template <typename T>
void to_json(const T& obj, int indent) {
    std::cout << "{\n";
    constexpr auto count = std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()).size();
    
    [&]<size_t... Is>(std::index_sequence<Is...>) {
        ( ( [&]{
            constexpr auto f = get_field_info<T, Is>();
            constexpr std::string_view field_name = std::meta::identifier_of(f);
            
            std::cout << std::format("{:>{}}\"{}\": ", "", indent, field_name);
            print_json_value(obj.[: f :]);

            if constexpr (Is < count - 1) {
                std::cout << ",";
            }
            std::cout << "\n";
        }() ), ... );
    }(std::make_index_sequence<count>{});

    std::cout << std::format("{:>{}}}}", "", indent - 2);
}

int main() {
    std::cout << "--- Day 07: JSON Serializer ---\n";

    User user = {
        .id = 101,
        .name = "John Doe",
        .is_active = true,
        .home_address = { "Neo-Tokyo", 12345 }
    };

    std::cout << "Generated JSON for User:\n";
    to_json(user);
    std::cout << "\n";

    return 0;
}
