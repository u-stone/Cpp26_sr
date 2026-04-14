// Day 03: Reflecting Structs/Classes
// [PLAN]: Use std::meta::nonstatic_data_members_of with index-based extraction.

#include <experimental/meta>
#include <iostream>
#include <string_view>
#include <format>
#include <utility>

struct Person {
    int id;
    std::string_view name;
    double salary;

private:
    [[maybe_unused]] int secret_code;

protected:
    bool is_verified;
};

template <typename T, size_t I>
consteval std::string_view get_field_name() {
    return std::meta::identifier_of(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I]);
}

template <typename T, size_t I>
consteval std::string_view get_field_type_name() {
    return std::meta::display_string_of(std::meta::type_of(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I]));
}

template <typename T, size_t I>
consteval std::string_view get_field_access() {
    constexpr auto f = std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I];
    if (std::meta::is_public(f)) return "public";
    if (std::meta::is_protected(f)) return "protected";
    if (std::meta::is_private(f)) return "private";
    return "unknown";
}

int main() {
    std::cout << "--- Day 03: Reflecting Structs/Classes ---\n";

    constexpr auto field_count = std::meta::nonstatic_data_members_of(^^Person, std::meta::access_context::unchecked()).size();
    
    std::cout << std::format("Fields in 'Person' ({} total):\n", field_count);
    
    [&]<size_t... Is>(std::index_sequence<Is...>) {
        ( (std::cout << std::format("  - [{}]: {} ({})\n", 
                     get_field_access<Person, Is>(), 
                     get_field_name<Person, Is>(), 
                     get_field_type_name<Person, Is>())), ... );
    }(std::make_index_sequence<field_count>{});

    return 0;
}
