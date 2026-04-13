// Day 06: Tuple Generation
// [PLAN]: Use std::meta::substitute to dynamically create a std::tuple from a list of types.
// We'll reflect on two types and generate a tuple containing both.

#include <experimental/meta>
#include <print>
#include <string_view>
#include <tuple>
#include <vector>

struct User {
    int id;
};

struct Settings {
    std::string_view theme;
};

int main() {
    std::println("--- Day 06: Tuple Generation ---");

    // 1. Collect meta-info for types we want in our tuple
    constexpr std::vector<std::meta::info> types_to_combine = { ^User, ^Settings, ^int };

    // 2. Dynamically substitute the template arguments of std::tuple
    // std::meta::substitute(template_meta, arguments_meta_list)
    // returns a meta-info representing the instantiated template.
    constexpr std::meta::info tuple_template = ^std::tuple;
    constexpr std::meta::info my_generated_tuple_meta = std::meta::substitute(tuple_template, types_to_combine);

    // 3. Splice the generated type back into the language
    using MyCombinedType = [: my_generated_tuple_meta :];

    MyCombinedType data = { User{1}, Settings{"Dark"}, 42 };

    // 4. Access elements to verify
    std::println("User ID: {}", std::get<0>(data).id);
    std::println("Settings Theme: {}", std::get<1>(data).theme);
    std::println("Extra Int: {}", std::get<2>(data));

    // 5. Static verification
    static_assert(std::tuple_size_v<MyCombinedType> == 3);
    static_assert(std::is_same_v<std::tuple_element_t<0, MyCombinedType>, User>);
    static_assert(std::is_same_v<std::tuple_element_t<1, MyCombinedType>, Settings>);
    static_assert(std::is_same_v<std::tuple_element_t<2, MyCombinedType>, int>);

    std::println("\nVerification: Tuple successfully generated from metadata!");

    return 0;
}
