# Day 07: JSON 序列化器 (JSON Serializer)

## 核心问题
> 我们如何结合反射查询、成员拼接和递归，为任何 C++ 结构体创建一个通用的序列化器？

## 理论与反射 API
- **通用序列化器**：一个通用的序列化器展示了为什么 P2996 对 C++ 具有变革性。
- **成员拼接循环**：我们在循环中使用 `obj.[: fields[i] :]` 来访问每个字段的值。
- **Consteval 处理**：由于来自 `nonstatic_data_members_of` 的 `std::vector<info>` 集合是堆分配的，序列化逻辑必须在 `consteval` 上下文中处理它们以提取元数据。

## 核心代码走读
- **泛型 `to_json<T>`**：
    - `constexpr auto fields = std::meta::nonstatic_data_members_of(^^T);` 获取结构体 `T` 中字段的所有元数据。
    - `for (size_t i = 0; i < fields.size(); ++i)` 遍历元数据向量。
    - `obj.[: fields[i] :]` 使用其元信息访问 `obj` 实例上的字段。
- **处理递归**：
    - 通过将反射与 `if constexpr` 和模板递归相结合，我们可以自动处理嵌套结构（如 `User` 内部的 `Address`）。

## 预期输出描述
程序为复杂的嵌套结构体输出格式良好的 JSON 字符串：
```json
--- Day 07: JSON Serializer ---
Generated JSON for User:
{
  "id": 101,
  "name": "John Doe",
  "is_active": true,
  "home_address": {
    "city": "Neo-Tokyo",
    "zip_code": 12345
  }
}
```

## 陷阱与回顾
- **访问控制**：这个简单版本仅适用于 `public` 成员。要处理私有成员，你需要结构体作为序列化器的友元，或者使用专门的访问钩子。
- **循环依赖**：递归序列化器需要防止循环引用（对于简单的数据结构不是问题，但在图状对象中很常见）。
- **数组和容器**：功能齐全的序列化器需要额外的逻辑，通过反射判断类型是否为容器来处理 `std::vector` 和数组。

## 与已有知识的联系
- 在 P2996 之前，你需要像 `nlohmann/json` 这样庞大的库，并且必须为你想要序列化的 *每个* 类型使用 `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MyType, field1, field2, ...)`。
- 有了 P2996，序列化是“零样板”的——它适用于任何类型。

## 自测问答
- **Q: 这适用于具有私有成员的类吗？**
- **A: `nonstatic_data_members_of` 将返回私有成员，但如果成员从 `to_json` 函数的角度来看是私有的，则拼接 `obj.[: f :]` 将无法编译。**

## 资源来源
- P2996 提案："Reflection for C++26"
