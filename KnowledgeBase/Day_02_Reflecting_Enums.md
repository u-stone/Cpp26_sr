# Day 02: 反射枚举 (Reflecting Enums)

## 核心问题
> 如何在不进行手动映射的情况下，遍历枚举的所有成员并将其值转换回字符串？

## 理论与反射 API
- **`std::meta::enumerators_of(info)`**：返回给定枚举类型的所有枚举项的 `std::vector<std::meta::info>`。由于这些结果在该分支中是堆分配的，应在 `consteval` 上下文中处理。
- **`std::meta::extract<T>(info)`**：从元数据中提取枚举项或常量的底层值。这是将运行时值映射回其元数据的关键。
- **基于索引的提取**：我们通常在可以使用 `std::index_sequence` 展开的辅助函数中，使用索引（如 `enumerators_of(^^E)[i]`）来访问反射结果。

## 核心代码走读
- **枚举转字符串助手 (Enum-to-String Helper)**：
    - `constexpr auto count = std::meta::enumerators_of(^^E).size();`：获取枚举中常量的总数。
    - `return std::meta::identifier_of(std::meta::enumerators_of(^^E)[I]);`：返回索引 `I` 处的标识符。
    - `if (get_enum_value<E, Is>() == value)`：将运行时值与从索引 `Is` 处的元数据提取的值进行比较。
    - `return get_enum_name<E, Is>();`：如果匹配，则返回名称。

## 预期输出描述
运行代码应输出完整的颜色列表及其显式值，然后演示特定的转换：
```
--- Day 02: Reflecting Enums ---
Enumerators of Color:
  - Name: Red, Value: 0
  - Name: Green, Value: 1
  - Name: Blue, Value: 2
  - Name: Cyan, Value: 10
  - Name: Magenta, Value: 11
  - Name: Yellow, Value: 12

My current color is: Cyan
```

## 陷阱与回顾
- **值冲突**：如果多个枚举项具有相同的值，`enum_to_string` 将返回它找到的第一个。
- **性能**：虽然此示例中的 `enum_to_string` 遍历列表，但反射部分发生在编译期。对于大型枚举，编译器的负载会增加。
- **头文件选择**：继续使用 `<experimental/meta>`。

## 与已有知识的联系
- 在 P2996 之前，这需要宏（如 `X Macros`）或第三方库（如 `MagicEnum`），这些库依赖于编译器特定的 `__PRETTY_FUNCTION__` 黑科技。
- 这是实现 `MagicEnum` 所做工作的正式、标准的方式。

## 自测问答
- **Q: `std::meta::enumerators_of` 对非枚举类型有效吗？**
- **A: 不行。对非枚举类型的元信息调用它会导致编译期错误。**

## 资源来源
- P2996 提案："Reflection for C++26"
