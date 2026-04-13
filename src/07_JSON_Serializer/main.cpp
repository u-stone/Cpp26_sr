// Day 07: JSON Serializer
// [PLAN]: Combine query (members_of), synthesis ([: :]), and type-aware logic
// to create a generic 'to_json' function that works for any struct.

#include <experimental/meta>
#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>

// Simple structs to serialize
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

// --- The Generic JSON Serializer ---

// Forward declaration for recursion
template <typename T>
void to_json(const T& obj, int indent = 2);

// Helper for basic types
template <typename T>
void print_json_value(const T& val) {
    if constexpr (std::is_same_v<T, std::string_view> || std::is_same_v<T, std::string>) {
        std::print("\"{}\"", val);
    } else if constexpr (std::is_same_v<T, bool>) {
        std::print("{}", val ? "true" : "false");
    } else if constexpr (std::is_arithmetic_v<T>) {
        std::print("{}", val);
    } else {
        // Assume it's a struct/class and recurse
        to_json(val, 4);
    }
}

template <typename T>
void to_json(const T& obj, int indent) {
    constexpr std::meta::info type_meta = ^T;
    constexpr auto fields = std::meta::nonstatic_data_members_of(type_meta);

    std::print("{{\n");
    
    for (size_t i = 0; i < fields.size(); ++i) {
        constexpr auto f = fields[i];
        std::string_view field_name = std::meta::name_of(f);
        
        // Print indentation and key
        std::print("{:>{}}\"{}\": ", "", indent, field_name);
        
        // Access and print the value using the splice operator '[: :]'
        print_json_value(obj.[: f :]);

        // Print comma if not the last field
        if (i < fields.size() - 1) {
            std::print(",");
        }
        std::print("\n");
    }

    std::print("{:>{}}}}", "", indent - 2);
}

int main() {
    std::println("--- Day 07: JSON Serializer ---");

    User user = {
        .id = 101,
        .name = "John Doe",
        .is_active = true,
        .home_address = { "Neo-Tokyo", 12345 }
    };

    std::print("Generated JSON for User:\n");
    to_json(user);
    std::println("");

    // Verification: ensure we can serialize another type without extra code
    Address addr = { "Metropolis", 54321 };
    std::print("\nGenerated JSON for Address:\n");
    to_json(addr);
    std::println("");

    return 0;
}
