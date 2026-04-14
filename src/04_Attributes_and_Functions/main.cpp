// Day 04: Attributes and Functions
// [PLAN]: Use index-based extraction and display_string_of to handle special members.

#include <experimental/meta>
#include <iostream>
#include <string_view>
#include <format>
#include <utility>

struct [[nodiscard]] MyService {
    void start() {}
    [[deprecated]] void stop() {}
    static int get_version() { return 1; }
    int data;
};

template <typename T, size_t I>
consteval std::meta::info get_member() {
    return std::meta::members_of(^^T, std::meta::access_context::unchecked())[I];
}

template <typename T, size_t I>
consteval bool has_deprecated() {
    return std::meta::has_attribute(get_member<T, I>(), ^^[[deprecated]]);
}

int main() {
    std::cout << "--- Day 04: Attributes and Functions ---\n";

    constexpr std::meta::info service_info = ^^MyService;

    if constexpr (std::meta::has_attribute(service_info, ^^[[nodiscard]])) {
        std::cout << "MyService is marked as [[nodiscard]]\n";
    }

    constexpr auto member_count = std::meta::members_of(service_info, std::meta::access_context::unchecked()).size();
    std::cout << std::format("\nMembers of 'MyService' ({} total):\n", member_count);

    [&]<size_t... Is>(std::index_sequence<Is...>) {
        ( ( [&]{
            constexpr auto m = get_member<MyService, Is>();
            
            // display_string_of works for ALL members including constructors/destructors
            constexpr std::string_view name = std::meta::display_string_of(m);
            constexpr bool is_func = std::meta::is_function(m);
            constexpr bool is_stat = std::meta::is_static_member(m);
            constexpr bool is_dep = has_deprecated<MyService, Is>();
            constexpr bool is_field = std::meta::is_nonstatic_data_member(m);

            if constexpr (is_func) {
                std::cout << std::format("  - [Function]: {}{} {}\n", 
                             is_stat ? "static " : "",
                             name,
                             is_dep ? "(DEPRECATED)" : "");
            } else if constexpr (is_field) {
                std::cout << std::format("  - [Field]: {}\n", name);
            } else {
                std::cout << std::format("  - [Other]: {}\n", name);
            }
        }() ), ... );
    }(std::make_index_sequence<member_count>{});

    return 0;
}
