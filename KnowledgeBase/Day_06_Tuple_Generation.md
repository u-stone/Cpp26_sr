# Day 06: Tuple 生成 (Tuple Generation)

## 核心问题
> 我们如何使用反射从类型集合中以编程方式生成复杂的模板实例化（如 `std::tuple`）？

## 理论与反射 API
- **`std::meta::substitute(template_meta, args_meta_list)`**：合成工具，接受类模板的元信息和参数的 `std::vector<std::meta::info>`。它返回代表生成实例的元信息。
- **堆分配**：由于在此分支中，类似 `std::vector<info>` 的参数列表是堆分配的，它们必须在 `consteval` 上下文中处理。
- **拼接上的包展开**：在某些实现中，`[: v :]...` 可用于将 info 向量展开为模板参数。

## 核心代码走读
- **定义类型列表**：
    ```cpp
    constexpr std::vector<std::meta::info> types = { ^^T1, ^^T2, ^^T3 };
    ```
- **代入 `std::tuple`**：
    ```cpp
    constexpr auto t = std::meta::substitute(^^std::tuple, types);
    ```
    `t` 现在反射了 `std::tuple<T1, T2, T3>`。
- **通过拼接创建别名**：
    ```cpp
    using MyTuple = [: t :];
    ```
    编译器在编译期将其解析为实际实例化的 `std::tuple`。

## 预期输出描述
程序生成一个包含 3 个元素的 tuple 并验证其内容：
```
--- Day 06: Tuple Generation ---
User ID: 1
Settings Theme: Dark
Extra Int: 42

Verification: Tuple successfully generated from metadata!
```

## 陷阱与回顾
- **模板兼容性**：传递给 `substitute` 的参数必须匹配模板预期的参数（例如参数数量、类型 vs. 非类型）。如果不匹配，`substitute` 返回无效的元信息，或者编译器在拼接期间抛出错误。
- **递归代入**：你可以将一个 `substitute` 的结果作为另一个的参数传递，从而实现复杂的嵌套结构。

## 与已有知识的联系
- 在 P2996 之前，从类型列表创建 tuple 需要复杂的递归模板和参数包操作（例如 `std::tuple_cat` 逻辑或自定义类型列表库）。
- `substitute` 使得模板实例化变得像操作向量一样简单。

## 自测问答
- **Q: `std::meta::substitute` 可以用于我的自定义模板吗？**
- **A: 可以。任何可以被反射的类模板或别名模板都可以与 `substitute` 一起使用。**
- **Q: 如果参数列表为空会怎样？**
- **A: 如果模板允许零参数（如变长参数模板），它将成功。否则，它将在编译期失败。**

## 资源来源
- P2996 提案："Reflection for C++26"
