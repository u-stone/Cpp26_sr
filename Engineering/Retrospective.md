# 项目回顾：C++26 反射实验环境修坑记录

> "所有犯过的错误，都值得被记录。"

本项目在搭建 C++26 反射 (P2996) 学习环境的过程中，经历了从“编译器报错”到“运行时崩溃”的多次迭代。以下是详细的错误记录、原因分析及解决方案。

---

## 1. 编译环境类错误

### 1.1 找不到 `<experimental/meta>` 头文件
- **错误现象**：编译时提示 `fatal error: 'experimental/meta' file not found`。
- **根本原因**：最初的安装脚本只编译了 Clang 编译器本体 (`ninja clang`)，而反射相关的标准库支持位于 `libc++` 中。
- **解决方案**：修改 `tools/install_reflection_compiler.sh`，在 `LLVM_ENABLE_RUNTIMES` 中启用 `libcxx;libcxxabi;libunwind`，并编译安装 `cxx` 目标。

### 1.2 Ninja 目标名称错误
- **错误现象**：`ninja: error: unknown target 'libcxx'`。
- **根本原因**：在 LLVM 的运行时构建体系中，`libc++` 对应的 Ninja 目标名为 `cxx`，安装目标为 `install-cxx`。
- **解决方案**：将脚本中的目标名称统一修正为官方定义的 `cxx` 和 `cxxabi`。

### 1.3 macOS 运行时链接失败 (Library not loaded)
- **错误现象**：程序编译通过，但运行时报错 `dyld[...]: Library not loaded: @rpath/libc++.1.dylib`。
- **根本原因**：程序默认链接到了系统路径下的旧版 `libc++`，或者无法找到项目内 `.toolchain/` 下的新版动态库。
- **解决方案**：在 `CMakeLists.txt` 中增加 `rpath` 配置：`-Wl,-rpath,${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/clang-p2996/lib`。

---

## 2. P2996 语法与分支差异

### 2.1 反射操作符不匹配
- **错误现象**：使用 `^int` 报错。
- **根本原因**：标准提案可能使用 `^`，但 Bloomberg 的 P2996 实现分支目前使用双脱字符 `^^` 作为反射操作符。
- **解决方案**：将所有代码中的 `^` 更新为 `^^`。

### 2.2 核心查询函数更名
- **错误现象**：找不到 `std::meta::name_of`。
- **根本原因**：该实现分支使用了更细分的命名：`identifier_of` (获取标识符) 和 `display_string_of` (获取显示字符串)。
- **解决方案**：根据查询对象选择合适的函数，内置类型通常需使用 `display_string_of`。

---

## 3. 内存与编译期限制

### 3.1 `constexpr` 向量存储限制
- **错误现象**：`pointer to subobject of heap-allocated object is not a constant expression`。
- **根本原因**：`std::meta::nonstatic_data_members_of` 返回的是 `std::vector<info>`。虽然它能在 `consteval` 内部运行，但在该分支的 `libc++` 实现中，这种堆分配的向量无法被赋值给跨越多个上下文的 `constexpr` 变量。
- **解决方案**：引入 **“索引式常量提取 (Index-based Consteval Extraction)”** 策略。使用 `std::index_sequence` 配合 `consteval` 辅助函数，通过索引实时获取每一个元信息，避免存储整个向量。

### 3.2 拼接操作符 (`[: :]`) 的解析歧义
- **错误现象**：在模板参数中使用拼接操作符时报错。
- **根本原因**：编译器解析器对 `[: :]` 在某些上下文（如 `std::is_same_v`）中的解析存在困难。
- **解决方案**：
    1. 使用 `using` 别名预先转换。
    2. 在拼接表达式外手动增加圆括号：`([: m :])`。

---

## 4. 编译器标志缺失

### 4.1 无法使用 `template for`
- **错误现象**：`error: 'template for' statements are not enabled; use '-fexpansion-statements'`。
- **解决方案**：在 `CMakeLists.txt` 中全局开启 `-fexpansion-statements`。

### 4.2 无法使用 `has_attribute`
- **错误现象**：`no member named 'has_attribute' in namespace 'std::meta'`。
- **解决方案**：需同时开启 `-fattribute-reflection` 编译器标志。

---

## 5. 特殊成员的边缘情况

### 5.1 匿名类型与特殊函数
- **错误现象**：`identifier_of` 在处理构造函数或析构函数时报错。
- **根本原因**：这些成员没有标准的 C++ 标识符。
- **解决方案**：
    1. 使用 `std::meta::has_identifier(m)` 进行前置判断。
    2. 使用 `display_string_of(m)` 获取其文本表示。

---

## 总结

探索前沿技术（如 C++26）如同在丛林中开路。每一个报错都是一次对工具链深层逻辑的窥探。通过本次实验，我们不仅学会了反射的语法，更深刻理解了编译器在处理 `constexpr` 堆分配、符号链接以及实验性语法扩展时的行为边界。
