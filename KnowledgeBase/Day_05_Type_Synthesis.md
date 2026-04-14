# Day 05: 类型合成 (Type Synthesis - [: :])

## 核心问题
> 我们如何将元信息 (`std::meta::info`) 转回实际的 C++ 类型和表达式？

## 理论与反射 API
- **拼接操作符 (`[: :]`)**：这是反射操作符 (`^^`) 的逆操作。它在编译期将元数据“拼接”回源代码中。
- **类型拼接**：当 `m` 反射一个类型时，`[: m :]` 可以用在任何允许出现类型名的地方。
- **表达式拼接**：当 `m` 反射一个值或成员时，`[: m :]` 可以用在表达式中。
- **Consteval 上下文**：反射查询（如 `std::vector<info>`）的结果是堆分配的，必须在 `consteval` 上下文中处理，以确保在拼接期间的有效性。

## 核心代码走读
- **创建类型**：
    ```cpp
    constexpr std::meta::info t = ^^int;
    [: t :] x = 5; // 编译器视其为 'int x = 5;'
    ```
- **访问成员**：
    ```cpp
    constexpr std::meta::info field = std::meta::nonstatic_data_members_of(^^S)[0];
    S instance;
    instance.[: field :] = 10; // 编译器视其为 'instance.member_name = 10;'
    ```
    这非常强大，因为它允许我们编写操作成员的代码，而无需在编写时确切知道成员的名称（例如在泛型函数中）。

## 预期输出描述
程序演示了拼接出的类型表现与原始类型完全一致，并且可以使用其元数据访问成员：
```
--- Day 05: Type Synthesis ([: :]) ---
Spliced type variable value: 42
Record id: 1, table: users
Extracted via splicing - id: 1, table: users

Verification: Type and member splicing successful!
```

## 陷阱与回顾
- **上下文敏感性**：拼接操作符是上下文敏感的。如果你在期望类型的地方使用它，它必须反射一个类型。如果你在期望表达式的地方使用它，它必须反射一个值或成员。
- **歧义**：如果 `m` 不是当前上下文有效的元信息（例如尝试将命名空间拼接为类型），编译器将报错。

## 与已有知识的联系
- 这取代了 `decltype` 的许多用途以及复杂的模板元编程。
- 在传统的 C++ 中，要根据某些条件访问成员，你需要复杂的成员指针逻辑或大型 `switch` 语句。拼接使这一切变得直接。

## 自测问答
- **Q: 我可以将 `[: :]` 与运行时计算出的 `std::meta::info` 一起使用吗？**
- **A: 不行。操作数必须是常量表达式 (`constexpr`)，因为拼接发生在编译期间。**

## 资源来源
- P2996 提案："Reflection for C++26"
