// Day 03: Reflecting Structs/Classes
// [PLAN]: Use std::meta::nonstatic_data_members_of to inspect a struct.
// We'll iterate over all fields and print their names, types, and access specifiers.

#include <experimental/meta>
#include <print>
#include <string_view>
#include <vector>

// A user-defined type with various field types and access levels
struct Person {
    int id;
    std::string_view name;
    double salary;

private:
    int secret_code;

protected:
    bool is_verified;
};

// Helper function to get access level as string
consteval std::string_view access_to_string(std::meta::info m) {
    if (std::meta::is_public(m)) return "public";
    if (std::meta::is_protected(m)) return "protected";
    if (std::meta::is_private(m)) return "private";
    return "unknown";
}

int main() {
    std::println("--- Day 03: Reflecting Structs/Classes ---");

    // 1. Reflect on the 'Person' struct
    constexpr std::meta::info person_info = ^Person;
    
    // 2. Extract non-static data members
    // This returns a collection (std::vector<std::meta::info>) of fields
    constexpr auto fields = std::meta::nonstatic_data_members_of(person_info);

    std::println("Fields in 'Person' ({} total):", fields.size());
    
    for (auto f : fields) {
        // Get the name of the field
        constexpr std::string_view field_name = std::meta::name_of(f);
        
        // Get the type of the field as another meta-info
        constexpr std::meta::info field_type_info = std::meta::type_of(f);
        constexpr std::string_view type_name = std::meta::name_of(field_type_info);
        
        // Get the access level
        std::string_view access = access_to_string(f);

        std::println("  - [{}]: {} ({})", access, field_name, type_name);
    }

    // 3. Static verification
    static_assert(fields.size() == 5);
    static_assert(std::meta::name_of(fields[0]) == "id");
    static_assert(std::meta::name_of(std::meta::type_of(fields[0])) == "int");
    static_assert(std::meta::is_private(fields[3])); // secret_code is private

    std::println("\nVerification: Struct layout successfully inspected!");

    return 0;
}
