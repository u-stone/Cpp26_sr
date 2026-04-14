# Day 01: 你好，元信息 (Hello Meta-Info)

## 核心问题
> 如何在 C++26 反射 (P2996) 提案中获取并查询基础类型信息？

## 理论与反射 API
- **反射操作符 (`^^`)**：这是 Bloomberg 分支中进入反射世界的首要入口。它产生一个 `std::meta::info` 类型的对象。
- **`std::meta::info`**：代表反射实体（类型、成员、命名空间等）的标量类型。
- **反射查询接口**：
    - `std::meta::display_string_of(info)`：返回实体的名称（适用于内置类型，如 `int`）。
    - `std::meta::identifier_of(info)`：返回标识符名称（适用于有名称的声明，如成员变量）。
    - `std::meta::is_type(info)`：如果是类型则返回 `true`。
- **即时函数 (`consteval`)**：许多反射函数是 `consteval` 且返回 `std::vector<info>`。由于在该分支中堆分配的向量不能存储在长寿命的 `constexpr` 变量中，这些结果必须在 `consteval` 上下文中立即处理。
- **基于索引的提取**：反射集合通常是向量，通过 `[i]` 进行索引。这通常与 `std::index_sequence` 配合使用来实现展开。

## 核心代码走读
- **元数据提取辅助函数**：
    ```cpp
    template <typename T, size_t I>
    consteval std::string_view get_member_name() {
        return std::meta::identifier_of(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())[I]);
    }
    ```
    该助手完全在编译期检索索引 `I` 处的成员标识符。
- **通过索引序列进行遍历**：
    `dump_members` 函数使用 `std::make_index_sequence` 展开索引参数包，从而允许我们对每个成员调用 `consteval` 助手，而无需存储堆分配的向量。
- `constexpr std::meta::info int_info = ^^int;`：获取内置类型的元信息仍然是一个直接操作。

## 预期输出描述
运行时，程序应打印反射类型的名称并确认所有编译期检查已通过：
```
--- Day 01: Hello Meta-Info ---
Member 0: id
Member 1: value
```

## 陷阱与回顾
- **头文件选择**：在 Bloomberg 分支中，反射函数位于 `<experimental/meta>` 路径下。
- **零求值 (Zero Evaluation)**：记住 `^^T` 不会实例化 `T` 或执行任何运行时操作。它纯粹是编译期构造。

## 与已有知识的联系
- 它取代了旧的 `typeid(T).name()`，后者是运行时的、实现定义的（修饰名），且对 `constexpr` 不友好。
- 它提供了一种无需手动编写复杂模板元编程（如 `std::is_same_v`）即可进行“元编程”的方法。

## 自测问答
- **Q: `std::meta::info` 可以在运行时使用吗？**
- **A: 不行。它专为编译期操作设计。虽然可以传递给函数，但这些函数通常必须是 `constexpr` 才能使反射逻辑生效。**

## 资源来源
- P2996 提案："Reflection for C++26"
