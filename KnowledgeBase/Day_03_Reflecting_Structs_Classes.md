# Day 03: 反射结构体/类 (Reflecting Structs/Classes)

## 核心问题
> 我们如何在编译期通过程序检查类或结构体的成员、类型和访问级别？

## 理论与反射 API
- **`std::meta::nonstatic_data_members_of(info)`**：返回代表类中所有非静态字段的 `std::vector<std::meta::info>`。这些集合在该分支中是堆分配的，必须在 `consteval` 上下文中处理。
- **`std::meta::type_of(info)`**：当对成员的元信息调用时，它返回代表该成员类型的元信息。
- **访问修饰符查询**：
    - `std::meta::is_public(info)`
    - `std::meta::is_protected(info)`
    - `std::meta::is_private(info)`
    这些函数返回 `bool` 值，可用于过滤成员。

## 核心代码走读
- **反射类成员**：
    - `constexpr auto fields = std::meta::nonstatic_data_members_of(^^Person);` 以向量形式检索所有数据成员。
    - `constexpr std::meta::info type_info = std::meta::type_of(fields[I]);` 演示了基于索引的元数据提取。
- **访问级别查询**：
    - `std::meta::is_private(fields[3])` 允许在编译期通过编程方式确定可见性。

## 预期输出描述
运行代码应显示 `Person` 结构体的布局：
```
--- Day 03: Reflecting Structs/Classes ---
Fields in 'Person' (5 total):
  - [public]: id (int)
  - [public]: name (basic_string_view<char, char_traits<char>>)
  - [public]: salary (double)
  - [private]: secret_code (int)
  - [protected]: is_verified (bool)

Verification: Struct layout successfully inspected!
```

## 陷阱与回顾
- **静态成员**：`nonstatic_data_members_of` *不*返回静态数据成员。要获取这些成员，请使用 `members_of` 并过滤 `is_static`。
- **继承成员**：此查询通常仅返回 *直接* 在类中声明的成员。要处理继承，需要反射 `bases_of(info)` 并递归查询它们的成员。

## 与已有知识的联系
- 在 P2996 之前，这种类型的检查只能使用外部代码生成器或极其复杂的非标准库黑科技（例如 "Precise-Flat-Reflect"）。
- 这是实现 **序列化** 和 **对象关系映射 (ORM)** 的基本构建块。

## 自测问答
- **Q: 反射可以访问这些字段的实际运行时值吗？**
- **A: 反射为你提供元数据（名称、类型等）。要访问实例的值，你需要“拼接”或专门的访问函数，我们将在 Phase 3 中探索这些内容。**

## 资源来源
- P2996 提案："Reflection for C++26"
