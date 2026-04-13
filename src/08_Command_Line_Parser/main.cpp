// Day 08: Command Line Parser
// [PLAN]: Create a generic CLI parser that maps arguments to struct fields.
// Example: --port 8080 --host localhost -> Config{ .port = 8080, .host = "localhost" }

#include <experimental/meta>
#include <print>
#include <string_view>
#include <vector>
#include <charconv>
#include <string>

// A sample configuration struct
struct AppConfig {
    int port = 3000;
    std::string_view host = "127.0.0.1";
    bool verbose = false;
    int timeout_ms = 5000;
};

// Generic parser function
template <typename T>
T parse_args(int argc, char* argv[]) {
    T config;
    std::vector<std::string_view> args(argv + 1, argv + argc);

    constexpr std::meta::info type_meta = ^T;
    constexpr auto fields = std::meta::nonstatic_data_members_of(type_meta);

    for (size_t i = 0; i < args.size(); ++i) {
        std::string_view arg = args[i];
        if (!arg.starts_with("--")) continue;

        std::string_view flag = arg.substr(2);

        // Find a matching field using reflection
        bool found = false;
        for (auto f : fields) {
            if (std::meta::name_of(f) == flag) {
                found = true;
                using FieldType = [: std::meta::type_of(f) :];

                // Check if it's a boolean flag (no value follows)
                if constexpr (std::is_same_v<FieldType, bool>) {
                    config.[: f :] = true;
                } else {
                    // Expect a value in the next argument
                    if (i + 1 < args.size()) {
                        std::string_view val_str = args[++i];

                        if constexpr (std::is_same_v<FieldType, int>) {
                            std::from_chars(val_str.data(), val_str.data() + val_str.size(), config.[: f :]);
                        } else if constexpr (std::is_same_v<FieldType, std::string_view>) {
                            config.[: f :] = val_str;
                        }
                    }
                }
                break;
            }
        }

        if (!found) {
            std::println("Warning: Unknown flag '--{}'", flag);
        }
    }

    return config;
}

int main() {
    std::println("--- Day 08: Command Line Parser ---");

    // Simulate command line arguments
    const char* simulated_argv[] = { 
        "myapp", 
        "--port", "8080", 
        "--host", "example.com", 
        "--verbose", 
        "--unknown-flag", "value" 
    };
    int simulated_argc = sizeof(simulated_argv) / sizeof(simulated_argv[0]);

    std::println("Simulating arguments: --port 8080 --host example.com --verbose");

    AppConfig config = parse_args<AppConfig>(simulated_argc, (char**)simulated_argv);

    std::println("\nParsed Configuration:");
    std::println("  - Port: {}", config.port);
    std::println("  - Host: {}", config.host);
    std::println("  - Verbose: {}", config.verbose);
    std::println("  - Timeout: {}ms", config.timeout_ms);

    // Static check for one field to ensure basic reflection works
    static_assert(std::meta::name_of(std::meta::nonstatic_data_members_of(^AppConfig)[0]) == "port");

    return 0;
}
