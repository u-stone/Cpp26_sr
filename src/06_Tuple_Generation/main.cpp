// Day 06: Tuple Generation
// [PLAN]: Use std::meta::substitute with an initializer_list of meta-infos.

#include <experimental/meta>
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>
#include <format>
#include <type_traits>

struct User {
    int id;
};

struct Settings {
    std::string_view theme;
};

int main() {
    std::cout << "--- Day 06: Tuple Generation ---\n";

    // 1. Substitution using initializer_list (avoids vector issues)
    constexpr std::meta::info tuple_template = ^^std::tuple;
    constexpr std::meta::info my_generated_tuple_meta = std::meta::substitute(tuple_template, { ^^User, ^^Settings, ^^int });

    // 2. Splice the generated type
    using MyCombinedType = [: (my_generated_tuple_meta) :];

    MyCombinedType data = { User{1}, Settings{"Dark"}, 42 };

    // 3. Access and verify
    std::cout << std::format("User ID: {}\n", std::get<0>(data).id);
    std::cout << std::format("Settings Theme: {}\n", std::get<1>(data).theme);
    std::cout << std::format("Extra Int: {}\n", std::get<2>(data));

    // 4. Static verification
    static_assert(std::tuple_size_v<MyCombinedType> == 3);
    static_assert(std::is_same_v<std::tuple_element_t<0, MyCombinedType>, User>);

    std::cout << "\nVerification: Tuple successfully generated from metadata!\n";

    return 0;
}
