// Day 05: Type Synthesis ([: :])
// [PLAN]: Use the splice operator '[: :]' with constant-evaluated metadata.

#include <experimental/meta>
#include <iostream>
#include <string_view>
#include <format>
#include <type_traits>

struct DatabaseRecord {
    int id;
    std::string_view table_name;
};

template <typename T, size_t I>
consteval std::meta::info get_field() {
    return std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I];
}

int main() {
    std::cout << "--- Day 05: Type Synthesis ([: :]) ---\n";

    // 1. Type Splicing
    constexpr std::meta::info type_meta = ^^int;
    [: type_meta :] my_int = 42; 
    std::cout << std::format("Spliced type variable value: {}\n", my_int);

    // 2. Creating an instance of a reflected struct
    constexpr std::meta::info record_meta = ^^DatabaseRecord;
    constexpr [: record_meta :] record = {1, "users"};
    std::cout << std::format("Record id: {}, table: {}\n", record.id, record.table_name);

    // 3. Member Splicing
    constexpr auto f0 = get_field<DatabaseRecord, 0>();
    constexpr auto f1 = get_field<DatabaseRecord, 1>();
    int extracted_id = record.[: f0 :];
    std::string_view extracted_table = record.[: f1 :];

    std::cout << std::format("Extracted via splicing - id: {}, table: {}\n", extracted_id, extracted_table);

    // 4. Static Verification
    // Use an alias to avoid unparenthesized splice errors in template arguments
    using SplicedInt = [: type_meta :];
    static_assert(std::is_same_v<SplicedInt, int>);
    
    using SplicedRecord = [: record_meta :];
    static_assert(std::is_same_v<SplicedRecord, DatabaseRecord>);

    // record is now constexpr
    static_assert(record.[: f0 :] == 1);

    std::cout << "\nVerification: Type and member splicing successful!\n";

    return 0;
}
