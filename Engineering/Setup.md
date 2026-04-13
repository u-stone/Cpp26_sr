# 环境配置总结：C++26 反射学习环境 (P2996)

本项目基于 **Deep Mastery Protocol (DMP)** 构建，专门用于探索和实践 C++26 反射提案 (P2996)。由于该特性尚未在主流编译器中完全落地，我们采用了一套自包含的实验环境。

## 1. 核心组件 (The Stack)

- **编译器**: [Bloomberg Clang P2996 Fork](https://github.com/bloomberg/clang-p2996) (支线: `p2996`)
- **构建系统**: CMake 3.25+ & Ninja
- **实验框架**: 基于 DMP 的自动化脚手架与知识库体系

## 2. 环境布局 (Directory Layout)

为了保持系统的整洁，所有的实验工具链均安装在项目内部：

- `.toolchain/clang-p2996/`: 存放专用版本的 Clang 编译器及其头文件。
- `build/`: 存放 CMake 编译产物（已在 `.gitignore` 中忽略）。
- `tools/`: 包含自动化管理脚本。
    - `install_reflection_compiler.sh`: 自动下载、编译并配置项目专用编译器的脚本。
    - `scaffold_lab.sh`: 自动生成每日实验代码、笔记模板并注册到 CMake 的脚本。

## 3. 关键配置细节 (Key Configurations)

### 3.1 编译器路径
编译器安装在项目内的 `.toolchain` 目录下，并已通过 `CMakePresets.json` 自动配置：
- **路径**: `${sourceDir}/.toolchain/clang-p2996/bin/clang++`
- **预设名**: `clang-p2996-debug`

### 3.2 编译标志
在 `CMakeLists.txt` 中已自动注入反射所需的特殊标志：
- `-std=c++2c`: 开启 C++26 标准支持。
- `-freflection`: 激活 P2996 反射核心语法。
- `-Wall -Wextra -Werror`: 遵循 **Zero Warning Policy**，将所有警告视为错误。

## 4. 常用操作流程 (Workflow)

### 4.1 安装/更新编译器
如果您需要重新构建或首次安装编译器：
```bash
./tools/install_reflection_compiler.sh
```

### 4.2 开启新实验
当您准备开始 Roadmap 中的下一天课程时：
```bash
# 例如开启 Day 02 的枚举反射学习
./tools/scaffold_lab.sh 02 Reflecting_Enums
```

### 4.3 编译与运行
使用 CMake 预设进行构建：
```bash
# 配置并构建
cmake --preset clang-p2996-debug
cmake --build --preset clang-p2996-debug

# 运行产物 (以 Day 01 为例)
./build/clang-p2996-debug/01_Hello_Meta_Info
```

## 5. 注意事项
- **实验性特性**: 当前使用的 Clang 版本为研究分支，编译大型代码时可能会有较高的内存消耗。
- **Git 管理**: `.toolchain/` 和 `build/` 目录已被加入 `.gitignore`，请勿手动修改忽略规则，以免提交巨大的二进制文件。
