# DexCap C++ SDK

**版本**  V2.1.0

### 1.1 概述
DexCap SDK C++ API V2.1.0 对应DexCap产品的硬件设备版本为V4。该SDK提供了针对DexCap V4设备所有功能的管理，控制，以及数据访问的基础接口，
同时提供了对应功能的C风格接口。

## **2. DexCap SDK目录结构**

### 2.1 目录结构布局

```
dexcap-sdk-cpp/
├── include/                   # 头文件目录，DexCap SDK开放的所有类型和接口声明的头文件均在该目录
│   ├── cpp                    # 提供C++类和接口声明的头文件的目录。C++开发者使用该目录下头文件提供的API
│       ├── DexCap.hpp         # 管理和控制DexCap设备套装的接口类DexCapSuit的声明
│       ├── TypeDef.hpp        # 使用DexCap设备套装所需的基本类型，常量，以及可访问的数据类型的声明
│       ├── Utils.hpp          # SDK提供的若干简单工具类，如常用的字符串和日期时间戳处理
│   ├── configuration.h        # 以C形式提供的基础类型，常量，以及系统接口配置等信息和功能接口。
│                              # 包含兼容V3.5的设置工具类(未完成兼容性测试，V3.5用户暂且请勿使用，V3.5兼容性测试完成后会经过官方途径公告)
│   ├── dexcap.h               # 以C形式提供的DexCap设备套装的管理和数据获取接口，对应DexCap.hpp所提供的功能接口
│   ├── typedef.h              # 以C形式提供的描述基础数据模型的结构体，基本类型等，TypeDef.hpp中的声明依赖该头文件
├── libs/                      # 动态库文件所在目录
│   ├── linux/                 # Linux平台动态库文件的目录
│   |   ├── libDexCap.so       # DexCap SDK的Linux(目前仅支持Ubuntu)平台动态库文件
│   |   ├── 其他                # 依赖的第三方库
│   ├── windows/               # Windows平台动态库文件的目录
│   |   ├── DexCap.dll         # DexCap SDK的.dll文件
│   |   ├── DexCap.lib         # DexCap SDK的.lib文件
│   |   ├── 其他                # 依赖的第三方库
├── conf/                      # V3.5所使用的配置文件及脚本，V4用户请忽略
│   |   ├── config.yaml        # DexCap手套及外骨骼设备的SDK配置文件
│   |   ├── env_setup.sh       # 环境配置文件，自动搜索系统中的串口设备并授予权限
├── examples/                  # 动态库文件所在目录
│   ├── example.cpp            # C++示例代码
│   ├── c_example.c            # C示例代码
│   ├── example_defs.h         # 示例程序头文件
│   ├── CMakeLists.txt         # 构建示例程序的cmake工程文件
```
<br><br>

### 1.2 关键文件职责说明

| 文件                         | 作用                                                               | 依赖                     |
|----------------------------|------------------------------------------------------------------|------------------------|
| `cpp/DexCap.hpp`           | DexCap SDK的核心C++ API头文件， 提供DexCap套设备向用户开放的所有管理接口和数据访问接口          |                        |
| `cpp/TypeDef.hpp`          | DexCap SDK的基础C++数据模型和常量声明，依赖typedef.h提供的C风格声明                    |                        |
| `cpp/Utils.hpp`            | 字符串，日期，时间戳相关工具函数接口                                               |                        |
| `typedef.h`                | DexCap SDK的C API用的基础数据类型，以及数据模型结构体声明，cpp/TypeDef.hpp中的部分内容依赖该头文件 | 
| `dexcap.h`                 | DexCap SDK的C API核心头文件，提供DexCap套设备向用户开放的所有管理接口和数据访问的C语言接口         |
| `configuration.h`          | 用于获取设备配置信息的接口声明，V3.5用户使用，V4用户请忽略                                 | 依赖yaml库                |   |
| `libDexCap.so`             | DexCap SDK的Linux平台的动态库文件                                         | 依赖libserial等三方库        |
| `DexCap.dll`, `DexCap.lib` | DexCap SDK的Windows平台的动态库文件                                       | 依赖Windows SDK          |
| `config.yaml`              | 数采设备配置信息，设备的串口名，波特率，数采串口适配器类型(USB有线或2.4G无线)，等。V3.5用户使用，V4用户请忽略   |                        |
| `env_setup.sh`             | Linux平台上的环境设置脚本，用于扫描系统中的USB串口设备并授予当前用户权限。V3.5用户使用，V4用户请忽略        | 需要sudo权限               |
| `c_example.cpp`            | C示例代码，展示DexCap SDK C API的基本用法                                    | include目录下的头文件和动态库     |
| `example.cpp`              | C++示例代码，展示DexCap SDK C++ API的基本用法                                | include/cpp目录下的头文件和动态库 |


## **2. API Reference**

### [2.1. CPP API Reference](./docs/CPP-API-Reference.md)

