// Day 08: Command Line Parser
// [PLAN]: Use index-sequence expansion with separated extraction and mapping logic.

#include <experimental/meta>
#include <iostream>
#include <string_view>
#include <vector>
#include <charconv>
#include <string>
#include <format>
#include <utility>

struct AppConfig {
    int port = 3000;
    std::string_view host = "127.0.0.1";
    bool verbose = false;
    int timeout_ms = 5000;
};

template <typename T, size_t I>
consteval std::meta::info get_field_info() {
    return std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I];
}

template <typename T>
T parse_args(int argc, char* argv[]) {
    T config;
    std::vector<std::string_view> args(argv + 1, argv + argc);

    for (size_t i = 0; i < args.size(); ++i) {
        std::string_view arg = args[i];
        if (!arg.starts_with("--")) continue;

        std::string_view flag = arg.substr(2);
        bool found = false;
        
        constexpr auto count = std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()).size();

        [&]<size_t... Is>(std::index_sequence<Is...>) {
            ( ( [&]{
                if (found) return;
                constexpr auto f = get_field_info<T, Is>();
                constexpr std::string_view field_name = std::meta::identifier_of(f);
                
                if (field_name == flag) {
                    found = true;
                    using FieldType = [: (std::meta::type_of(f)) :];

                    if constexpr (std::is_same_v<FieldType, bool>) {
                        config.[: f :] = true;
                    } else {
                        if (i + 1 < args.size()) {
                            std::string_view val_str = args[++i];
                            if constexpr (std::is_same_v<FieldType, int>) {
                                std::from_chars(val_str.data(), val_str.data() + val_str.size(), config.[: f :]);
                            } else if constexpr (std::is_same_v<FieldType, std::string_view>) {
                                config.[: f :] = val_str;
                            }
                        }
                    }
                }
            }() ), ... );
        }(std::make_index_sequence<count>{});

        if (!found) {
            std::cout << std::format("Warning: Unknown flag '--{}'\n", flag);
        }
    }

    return config;
}

int main() {
    std::cout << "--- Day 08: Command Line Parser ---\n";

    const char* simulated_argv[] = { 
        "myapp", 
        "--port", "8080", 
        "--host", "example.com", 
        "--verbose", 
        "--unknown-flag", "value" 
    };
    int simulated_argc = sizeof(simulated_argv) / sizeof(simulated_argv[0]);

    AppConfig config = parse_args<AppConfig>(simulated_argc, (char**)simulated_argv);

    std::cout << "\nParsed Configuration:\n";
    std::cout << std::format("  - Port: {}\n", config.port);
    std::cout << std::format("  - Host: {}\n", config.host);
    std::cout << std::format("  - Verbose: {}\n", config.verbose);
    std::cout << std::format("  - Timeout: {}ms\n", config.timeout_ms);

    return 0;
}
