// Day 04: Attributes and Functions
// [PLAN]: Use std::meta::members_of to inspect member functions and attributes.
// We'll filter members for functions and check for built-in attributes.

#include <experimental/meta>
#include <print>
#include <string_view>
#include <vector>

// A struct with member functions and attributes
struct [[nodiscard]] MyService {
    void start() {}
    
    [[deprecated("Old API")]] 
    void stop() {}
    
    static int get_version() { return 1; }
    
    int data;
};

// Helper to check if a member is a function
consteval bool is_function(std::meta::info m) {
    // In P2996, members_of returns everything. We need to distinguish.
    // Note: The specific API might vary slightly in experimental forks.
    return std::meta::is_function(m);
}

int main() {
    std::println("--- Day 04: Attributes and Functions ---");

    constexpr std::meta::info service_info = ^MyService;

    // 1. Check class-level attributes
    // P2996 allows checking for attributes by their meta-info
    if constexpr (std::meta::has_attribute(service_info, ^nodiscard)) {
        std::println("MyService is marked as [[nodiscard]]");
    }

    // 2. Inspect all members
    constexpr auto all_members = std::meta::members_of(service_info);
    std::println("\nMembers of 'MyService' ({} total):", all_members.size());

    for (auto m : all_members) {
        std::string_view name = std::meta::name_of(m);
        
        if (std::meta::is_function(m)) {
            bool is_static = std::meta::is_static(m);
            bool is_deprecated = std::meta::has_attribute(m, ^deprecated);
            
            std::println("  - [Function]: {}{} {}", 
                         is_static ? "static " : "",
                         name,
                         is_deprecated ? "(DEPRECATED)" : "");
        } else if (std::meta::is_nonstatic_data_member(m)) {
            std::println("  - [Field]: {}", name);
        }
    }

    // 3. Static verification
    static_assert(std::meta::has_attribute(^MyService, ^nodiscard));
    
    // Find 'stop' and check deprecated
    constexpr auto stop_member = []() {
        for (auto m : std::meta::members_of(^MyService)) {
            if (std::meta::name_of(m) == "stop") return m;
        }
        return std::meta::info{};
    }();
    static_assert(std::meta::has_attribute(stop_member, ^deprecated));

    std::println("\nVerification: Functions and attributes successfully queried!");

    return 0;
}
