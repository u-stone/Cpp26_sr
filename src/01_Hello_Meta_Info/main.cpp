// Day 01: Hello Meta-Info
// [PLAN]: Use separate consteval helper for metadata extraction to allow printing.

#include <experimental/meta> 
#include <iostream>
#include <string_view>
#include <format>
#include <utility>

struct MyAwesomeStruct {
    int id;
    float value;
};

template <typename T, size_t I>
consteval std::string_view get_member_name() {
    return std::meta::identifier_of(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I]);
}

template <typename T>
void dump_members() {
    constexpr auto count = std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()).size();
    
    [&]<size_t... Is>(std::index_sequence<Is...>) {
        ( (std::cout << std::format("Member {}: {}\n", Is, get_member_name<T, Is>())), ... );
    }(std::make_index_sequence<count>{});
}

int main() {
    std::cout << "--- Day 01: Hello Meta-Info ---\n";
    dump_members<MyAwesomeStruct>();
    return 0;
}
