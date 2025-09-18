# DexCap API Reference

## **1. DexCap SDK目录结构**

### 1.1 目录结构布局

```JSON
DexCapSDK/
├── include/                   # 对外头文件（按设备模块划分）
│   ├── configuration.h        # 数采手套和外骨骼设备的配置读取
│   ├── exoskeleton.h          # 数采手套和外骨骼集成控制接口
│   ├── typedef.h              # DexCap SDK中所有数据类型的声明
├── libs/                      # 动态库文件所在目录
│   ├── linux/                 # Linux平台动态库文件的目录
│   |   ├── libDexCap.so       # DexCap SDK的动态库文件
│   |   ├── libserial.so       # 依赖库libserial的动态库文件
│   |   ├── libusb-1.0.so      # 依赖库libusb的动态库文件
│   |   ├── libyaml.so         # 依赖库libyaml的动态库文件
│   ├── windows/                # Windows平台动态库文件的目录
│   |   ├── DexCap.dll         # DexCap SDK的.dll文件
│   |   ├── DexCap.lib         # DexCap SDK的.lib文件
├── conf/                      # 配置文件及脚本
│   |   ├── config.yaml        # DexCap手套及外骨骼设备的SDK配置文件
│   |   ├── env_setup.sh       # 环境配置文件，自动搜索系统中的串口设备并授予权限
├── examples/                  # 动态库文件所在目录
│   ├── example.cpp            # 示例代码
│   ├── CMakeLists.txt         # 构建示例程序的cmake工程文件
```

### 1.2 关键文件职责说明

| 文件                     | 作用                                                              | 依赖            |
| ----------------------- | ------------------------------------------------------------------| -------------- |
| `configuration.h`       | 用于获取设备配置信息的接口声明                                         | 依赖yaml库      |
| `exoskeleton.h`         | DexCap SDK的核心API头文件， 数采手套和外骨骼设备的数据读取和控制接口均在该头文件中声明 |        |
| `libDexCap.so`              | DexCap SDK的Linux平台的动态库文件                               | 依赖libserial等三方库 |
| `DexCap.dll`, `DexCap.lib`  | DexCap SDK的Windows平台的动态库文件                             | 依赖Windows SDK |
| `config.yaml`               | 数采设备配置信息，设备的串口名，波特率，数采串口适配器类型(USB有线或2.4G无线)，等 |         |
| `env_setup.sh`              | Linux平台上的环境设置脚本，用于扫描系统中的USB串口设备并授予当前用户权限 | 需要sudo权限     |
| `example.cpp`               | 示例代码，展示DexCap SDK的基本用法                                | DexCapSDK头文件和动态库 |

## **2. API Reference**

### 2.1 DexCapConfig类

声明位于头文件configuration.h中，用于读取config.yaml文件中数采手套和外骨骼的配置信息，以及DexCap遥操系统服务端的网络配置信息。该类不支持公开显示构造，只能通过静态成员函数Load()来构造该类对象的shared_ptr.

#### 2.1.1 静态成员函数

##### static DexCapConfig::PTR Load(const std::string & yamlFile)

传入一个yaml配置文件的完整路径，用于构造一个该类对象，并返回该对象的智能指针std::shared_ptr.

#### 2.1.2 公开成员函数

##### std::string GetServerAddress() const

获取yaml配置文件中的DexCap遥操服务器的IP地址.

##### uint16_t GetServerPortNum() const

获取yaml配置文件中的DexCap遥操服务器的端口号.

##### NetProtocolType GetServerProtoType() const

获取yaml配置文件中的DexCap遥操服务器的通讯协议类型，返回值为NetProtocolType::TCP或NetProtocolType::UDP.

##### std::string GetLeftHandSPortName() const

获取yaml配置文件中配置的左手数采手套设备的USB串口名称，该串口名称应由操作系统自动分配。在Linux系统上，通常为/dev/ttyUSB*. 在Windows系统上，通常为COM*.

##### std::string GetRightHandSPortName() const

获取yaml配置文件中配置的右手数采手套设备的USB串口名称.

##### std::string GetBodySPortName() const

获取yaml配置文件中配置的上半身外骨骼设备的USB串口名称. 在Linux系统上，通常为/dev/ttyACM*. 在Windows系统上，通常为COM*.

##### AdapterType GetLeftHandAdapterType() const

获取yaml配置文件中配置的连接左手数采手套设备的USB串口适配器类型，返回值可能为AdapterType::WIREDUSB或者AdapterType::WIRELESS，分别代表USB直连和2.4G无线接受器.

##### AdapterType GetRightHandAdapterType() const

获取yaml配置文件中配置的连接右手数采手套设备的USB串口适配器类型.

##### AdapterType GetBodyAdapterType() const

获取yaml配置文件中配置的连接上身外骨骼设备的USB串口适配器类型.

##### uint8_t GetLeftHandDeviceId() const

获取yaml配置文件中配置的左手数采手套的设备ID，该ID出厂默认为1，使用该SDK在单套DexCap数采外骨骼套装上执行数采任务时，可不区分左右手手套及外骨骼的设备ID.

##### uint8_t GetRightHandDeviceId() const

获取yaml配置文件中配置的右手数采手套的设备ID，该ID出厂默认为1.

##### uint8_t GetBodyDeviceId() const

获取yaml配置文件中配置的上身外骨骼的设备ID，该ID出厂默认为1.

##### uint8_t IsDebugOnLeftHand() const

获取yaml配置文件中关于是否在左手数采手套上开启调试的设置. 当调试开启时，数采手套上各关节的角度数据将打印在标准输出上.

##### uint8_t IsDebugOnRightHand() const

获取yaml配置文件中关于是否在右手数采手套上开启调试的设置. 当调试开启时，数采手套上各关节的角度数据将打印在标准输出上.

##### uint8_t IsDebugOnBody() const

获取yaml配置文件中关于是否在上身外骨骼设备上开启调试的设置. 当调试开启时，外骨骼上各关节的角度数据将打印在标准输出上.

##### bool IsNetworkEnabled() const

获取yaml配置文件中关于是否连接DexCap遥操系统网络服务器的设置，一旦启用该设置，使用DexCap SDK启动的应用程序将自动使用配置文件中的配置信息启动与遥操系统服务器端的网络连接.

##### bool IsBodyEnabled() const

获取yaml配置文件中关于是启用上身外骨骼数采设备的设置，使用该SDK的应用程序可以根据该设置项主动选择是否连接外骨骼数采设备.

##### bool IsLeftHandEnabled() const

获取yaml配置文件中关于是启用左手数采手套设备的设置，使用该SDK的应用程序可以根据该设置项主动选择是否连接左手数采手套设备.

##### bool IsRightHandEnabled() const

获取yaml配置文件中关于是启用右手数采手套设备的设置，使用该SDK的应用程序可以根据该设置项主动选择是否连接右手数采手套设备.

##### BaudRate GetBodyBaudRate() const

获取yaml配置文件中上身外骨骼数采设备的波特率设置，使用该SDK的应用程序使用该波特率连接上身外骨骼数采设备并与之通讯.

##### LibSerial::BaudRate GetLeftHandBaudRate() const

获取yaml配置文件中左手数采手套设备的波特率设置，使用该SDK的应用程序使用该波特率连接左手数采手套设备并与之通讯.

##### LibSerial::BaudRate GetRightHandBaudRate() const

获取yaml配置文件中右手数采手套设备的波特率设置，使用该SDK的应用程序使用该波特率连接右手数采手套设备并与之通讯.

##### uint16_t GetBodySampleRate() const

获取yaml配置文件中上身外骨骼数采设备的数据采样率，单位Hz. 使用该采样率连接并初始化上身外骨骼数采设备后，设备将以该采样率向应用程序反馈实时数据.

##### uint16_t GetLeftHandSampleRate() const

获取yaml配置文件中左手数采手套设备的数据采样率设置，单位Hz.

##### uint16_t GetRightHandSampleRate() const

获取yaml配置文件中右手数采手套设备的数据采样率，单位Hz.

### 2.2 ExoSkeleton类

声明位于头文件exoskeleton.h中，提供了DexCap数采系统的所有核心功能，如连接和初始化手套或外骨骼数采设备，对数采设备进行初时位姿标定，连接DexCap遥操系统网络服务器，读取或更改设备ID，启动实时数采任务，向DexCap遥操系统发送实时关节位姿数据等.

#### 2.2.1 构造函数

该类不支持默认构造函数，构造该类的对象时，必须说明使用的何种网络通讯方式（TCP或UDP）与DexCap遥操系统的网络服务器进行通讯，以及与设备进行通讯的默认波特率.

##### ExoSkeleton(NetProtocolType protoType, BaudRate=BaudRate::BAUD_921600)

构造一个ExoSkeleton类的实例，默认使用921600的波特率与手套和外骨骼设备进行连接并初始化设备. 参数protoType指明使用哪种网络通讯方式与DexCap遥操系统的网络服务器进行通讯. 设备连接并初始化完成后，可以通过SwitchBaudRate接口对单个设备的波特率进行更改.

#### 2.2.1 成员函数

##### bool InitNetwork(const std::string & srvrAddr, int srvrPort)

初始化网络设置并与发起与DexCap遥操系统服务器的连接. 成功返回true, 失败返回false.

##### bool InitBody(AdapterType adapterType, const char * portName, uint8_t deviceId, uint32_t timeout=3000)

使用给定的adapterType(值为WIREDUSB或WIRELESS)，串口号portName, 以及deviceId, 与上半身外骨骼数采设备发起连接，并将其初始化. 若给定的deviceId与设备的真实ID不匹配，将会导致连接失败. 成功返回true, 失败返回false. 若超过timeout指定的超时时间，即便设备已成功连接，但初始化未完成，同样会返回false.

##### bool InitLeftHand(AdapterType adapterType, const char * portName, uint8_t deviceId, uint32_t timeout=3000)

使用给定的adapterType(值为WIREDUSB或WIRELESS)，串口号portName, 以及deviceId, 与左手数采手套设备发起连接，并将其初始化. 若给定的deviceId与设备的真实ID不匹配，将会导致连接失败. 成功返回true, 失败返回false. 若超过timeout指定的超时时间，即便设备已成功连接，但初始化未完成，同样会返回false.

##### bool InitRightHand(AdapterType adapterType, const char * portName, uint8_t deviceId, uint32_t timeout=3000)

使用给定的adapterType(值为WIREDUSB或WIRELESS)，串口号portName, 以及deviceId, 与右手数采手套设备发起连接，并将其初始化. 若给定的deviceId与设备的真实ID不匹配，将会导致连接失败. 成功返回true, 失败返回false. 若超过timeout指定的超时时间，即便设备已成功连接，但初始化未完成，同样会返回false.

##### uint8_t GetLeftHandID() const

获取当前ExoSkeleton实例中已成功连接的左手数采手套的设备ID.

##### uint8_t GetRightHandID() const

获取当前ExoSkeleton实例中已成功连接的右手数采手套的设备ID.

##### uint8_t GetBodyDeviceID() const

获取当前ExoSkeleton实例中已成功连接的上半身外骨骼的设备ID.

##### void UpdateDeviceID(ExoApparatus apptus, uint8_t newDeviceId)

为当前ExoSkeleton实例中指定的单体设备设置新的设备ID，指定的设备由参数apptus说明，该参数取值可为ExoApparatus::LGlove, ExoApparatus::RGlove,以及ExoApparatus::UpBody，分别代表左手手套，右手手套，以及上身外骨骼. 参数newDeviceId为需要写入的新设备ID. 若指定的设备未连接，则该调用将直接返回，设置不生效.

##### void SetLeftHandLogLevel(LOG_LEVEL level)

为当前ExoSkeleton实例中的左手数采手套设置日志级别. 当前仅支持DEBUG和非DEBUG两个级别。当级别设置为DEBUG时,设备上的关节位姿角度数据将打印在标准输出上.

##### void SetRightHandLogLevel(LOG_LEVEL level)

为当前ExoSkeleton实例中的右手数采手套设置日志级别. 当前仅支持DEBUG和非DEBUG两个级别。当级别设置为DEBUG时,设备上的关节位姿角度数据将打印在标准输出上.

##### void SetBodyLogLevel(LOG_LEVEL level)

为当前ExoSkeleton实例中的上身外骨骼设置日志级别. 当前仅支持DEBUG和非DEBUG两个级别。当级别设置为DEBUG时,设备上的关节位姿角度数据将打印在标准输出上.

##### DEX_RETURN SetAutoFeedback(uint16_t interval, bool enable=true)

为当前ExoSkeleton实例中所有设备设置是否开启自动数据反馈，以及设置采样间隔(单位毫秒). 参数enable指明是否开启. 若某个单体设备未连接，则设置不生效.

##### void SetAutoFeedback(ExoApparatus apptus, uint16_t interval, bool enable=true)

为当前ExoSkeleton实例中指定的单体设备设置是否开启自动数据反馈，以及设置采样间隔(单位毫秒). 参数enable指明是否开启. 若指定的设备未连接，则设置不生效.

##### void Start(bool debug=false)

在当前ExoSkeleton实例中的所有设备上启动数采任务. 任务启动后，开启了自动数据反馈的设备将按照设置的采样间隔，不间断的上报各关节的角度数据. 如果与DexCap遥操系统的网络服务器已建立连接，则上报的角度数据将会实时发送给服务器.

##### void Pause()

暂停当前ExoSkeleton实例中的所有设备的数采任务.

##### void Pause(ExoApparatus apptus)

暂停当前ExoSkeleton实例中指定的单体设备的数采任务.

##### void Resume()

恢复当前ExoSkeleton实例中的所有设备的数采任务.

##### void Resume(ExoApparatus apptus)

恢复当前ExoSkeleton实例中指定的单体设备的数采任务.

##### void Close()

停止当前ExoSkeleton实例中的所有设备的数采任务，并关闭设备连接.

##### void Close(ExoApparatus apptus)

停止当前ExoSkeleton实例中指定的单体的数采任务，并关闭该设备的连接.

##### bool ReOpen(ExoApparatus apptus)

重新连接当前ExoSkeleton实例被关闭的单体设备.

##### void Reset(ExoApparatus apptus)

将当前ExoSkeleton实例中的指定的单体设备恢复出厂设置.

##### void ReadCurrentState(ExoApparatus apptus)

对当前ExoSkeleton实例中的指定的单体设备执行单次采集，收到指令的设备将反馈当前时刻所有关节的角度传感器读数.

##### uint16_t GetBatteryLevel(ExoApparatus apptus) const

获取当前ExoSkeleton实例中的指定的单体设备的电池剩余电量（百分比）. 仅数采手套支持该指令，对外骨骼执行该指令始终返回0.

##### void VibeMotors(ExoApparatus apptus, const std::vector<uint8_t> & vibeData)

对当前ExoSkeleton实例中的指定的数采手套设备发送震动指令，收到指令的手套设备按照vibeData中对应位的数值触发指尖的震动电机进行震动，vibeData长度要求为5,分别代表5根手指.
