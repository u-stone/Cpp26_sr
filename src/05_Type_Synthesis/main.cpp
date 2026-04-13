// Day 05: Type Synthesis ([: :])
// [PLAN]: Use the splice operator '[: :]' to convert meta-info back into C++ code.
// We will demonstrate type splicing and value/member access splicing.

#include <experimental/meta>
#include <print>
#include <string_view>

struct DatabaseRecord {
    int id;
    std::string_view table_name;
};

int main() {
    std::println("--- Day 05: Type Synthesis ([: :]) ---");

    // 1. Type Splicing
    // '[: m :]' acts as a type if 'm' reflects a type.
    constexpr std::meta::info type_meta = ^int;
    [: type_meta :] my_int = 42; // This is equivalent to 'int my_int = 42;'
    
    std::println("Spliced type variable value: {}", my_int);

    // 2. Creating an instance of a reflected struct
    constexpr std::meta::info record_meta = ^DatabaseRecord;
    [: record_meta :] record = {1, "users"};
    
    std::println("Record id: {}, table: {}", record.id, record.table_name);

    // 3. Member Splicing (Expression Splicing)
    // We can reflect on a member and then use it to access that member on an instance.
    constexpr auto fields = std::meta::nonstatic_data_members_of(^DatabaseRecord);
    constexpr std::meta::info id_field_meta = fields[0];
    constexpr std::meta::info table_field_meta = fields[1];

    // Accessing fields via splice operator
    // On an object 'obj', 'obj.[: m :]' accesses the member reflected by 'm'.
    int extracted_id = record.[: id_field_meta :];
    std::string_view extracted_table = record.[: table_field_meta :];

    std::println("Extracted via splicing - id: {}, table: {}", extracted_id, extracted_table);

    // 4. Static Verification
    static_assert(std::is_same_v<[: ^int :], int>);
    static_assert(std::is_same_v<[: record_meta :], DatabaseRecord>);

    // Verify that the spliced expression works in constexpr
    constexpr [: record_meta :] const_record = {99, "logs"};
    static_assert(const_record.[: id_field_meta :] == 99);

    std::println("\nVerification: Type and member splicing successful!");

    return 0;
}
