# Day 04: 属性与函数 (Attributes and Functions)

## 核心问题
> 我们如何反射成员函数和 C++ 属性 (`[[...]]`)，以创建具有元数据感知能力的逻辑？

## 理论与反射 API
- **`std::meta::members_of(info)`**：返回包含类中 *所有* 成员（包括函数、嵌套类型和静态数据成员）的 `std::vector<std::meta::info>`。这些向量是堆分配的，必须在 `consteval` 上下文中处理。
- **`std::meta::is_function(info)`**：查询元信息是否代表函数或成员函数。
- **`std::meta::is_static_member(info)`**：判断成员是否为静态成员。
- **`std::meta::has_attribute(info, attr_info)`**：检查声明是否具有特定属性。属性本身可以使用 `^^attribute_name` 进行反射。

## 核心代码走读
- **通用成员检查**：
    - `constexpr auto all_members = std::meta::members_of(^^MyService);` 获取作为一个反射集合的所有成员。
    - `is_function(all_members[I])` 演示了通过基于索引的访问进行过滤。
- **属性驱动的逻辑**：
    - `std::meta::has_attribute(m, ^^deprecated)` 允许编译器检测函数是否应被忽略或特殊处理。
    - 也可以查询类级别的属性，如 `[[nodiscard]]`。

## 预期输出描述
程序对 `MyService` 的成员进行分类并识别其属性：
```
--- Day 04: Attributes and Functions ---
MyService is marked as [[nodiscard]]

Members of 'MyService' (10 total):
  - [Function]: start 
  - [Function]: stop (DEPRECATED)
  - [Function]: static get_version 
  - [Field]: data
  - [Function]: MyService 
  - [Function]: MyService 
  - [Function]: operator= 
  - [Function]: MyService 
  - [Function]: operator= 
  - [Function]: ~MyService 
```

## 陷阱与回顾
- **属性参数**：当前的 P2996 实现侧重于属性的 *存在*。提取属性的 *参数*（例如 `[[deprecated("reason")]]` 中的字符串）更为复杂，可能涉及额外的实验性 API。
- **重载**：如果一个函数被重载，`members_of` 将返回具有相同名称的多个条目。消除歧义需要检查签名。

## 与已有知识的联系
- 属性以前对 C++ 类型系统是“不可见”的。你可以使用它们告诉编译器某些信息，但不能使用类型系统对其做出反应。
- 这开启了 **编译期打标 (Compile-time Tagging)**：使用自定义属性为字段标记“SQL 主键”、“JSON 忽略”等。

## 自测问答
- **Q: 我可以反射局部变量的属性吗？**
- **A: 不行。P2996 中的反射主要针对声明（类型、成员、命名空间、全局变量）。局部变量目前不可反射。**

## 资源来源
- P2996 提案："Reflection for C++26"
