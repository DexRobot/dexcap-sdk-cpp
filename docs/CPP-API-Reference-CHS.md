# DexCap SDK V2.1（对应DexCap设备版本V4）C++ API Reference

## 1 常用数据类型

###  __1.11 `DEXCAP_SUIT_HANDLE`__
__声明：__`#define DEXCAP_SUIT_HANDLE void *`

__文件：__`typedef.h`

C接口用户使用，代表访问DexCap套设备的实例句柄。用户在连接和访问DexCap套设备的管理功能和数据接口时，均需要提供该句柄
<br><br>


###  __1.12  `DEX_RETURN`__
__声明：__`typedef enum { ... } DEX_RETURN;`

__文件：__`typedef.h`

C/C++接口通用，枚举类型。DexCap API中非数据访问接口的返回码，如管理类接口，该返回值代表命令执行的结果，通常接口执行成功后会返回`DEX_SUCCESS`，失败的返回`DEX_ERROR`。
如果是带有混合语义或逻辑的接口，如执行结果部分成功，则返回`DEX_SUCCESS_WITH_INFO`，代表接口执行完成了，但有部分失败或出错。用户可通过anyError()，getErrorCode()，
以及getErrorMessage()等接口获取是否发生错误，以及具体的错误码和错误信息。
<br><br>


###  __1.13 `ErrorCode`__
__声明：__`typedef enum { ... } ErrorCode;`

__文件：__`typedef.h`

C/C++接口通用，枚举类型，DexCap SDK开放的所有错误码的定义。
<br><br>


###  __1.14 `ADAPTER_TYPE`__
__声明：__`typedef enum { ... } ADAPTER_TYPE;`

__文件：__`typedef.h`

C/C++接口通用，枚举类型，表示DexCap设备所支持的所有连接适配器的类型，如WIREDUSB，WLAN_TCP，分别代表USB和WLAN连接。在创建DexCap实例句柄或对象时使用。
<br><br>WLABN


###  __1.15 `DEXCAP_DEVICE_TYPE`__
__声明：__`typedef enum { ... } DEXCAP_DEVICE_TYPE;`

__文件：__`typedef.h`

C/C++接口通用，枚举类型，表示DexCap设备的类型，如UpBody/JSBody(包含左右手手柄)代表上肢外骨骼，LGlove和RGlove分别代表左右手手套，以及IMUnit代表腰部IMU。对于V4设备，SDK与设备成功连接后，会自动识别设备的真实类型，并将设备的类型参数进行赋值。
<br><br>


###  __1.16 `ExoSkeletonJointID`__
__声明：__`typedef enum { ... } ExoSkeletonJointID;`

__文件：__`typedef.h`

C/C++接口通用，枚举类型，表示DexCap上肢外骨骼所有自由度的关节ID，该类型映射关节，分别以腰部-左臂-右臂，且以近端-远端的顺序原则，对ID进行分配。
<br><br>


###  __1.17 `ProductVersion`__
__声明：__`typedef enum { ... } ProductVersion;`

__文件：__`typedef.h`

C/C++接口通用，枚举类型，表示DexCap的产品版本，目前V3代表V3.5版设备，V4即V4版设备，V4C1代表V4 CASBOT定制版1代。
<br><br>


###  __1.18 `Joystick`__
__声明 ：__
```
typedef struct Joystick_t
{
    int16_t  RockerX  : 16;
    int16_t  RockerY  : 16;
    uint16_t TgrDistA : 16;
    uint16_t TgrDistB : 16;
    uint8_t  ButtonA  : 1;
    uint8_t  ButtonB  : 1;
    uint8_t  RockerZ  : 1;
    uint8_t  TriggerA : 1;
    uint8_t  TriggerB : 1;
    int16_t  Reserved : 11;
} Joystick;
```

__文件：__`typedef.h`

C/C++接口通用，结构体。遥控手柄所有按键数据。在数采过程中可通过对应的数据访问接口实时获取当前时刻手柄上各个按键的状态，摇杆以及板机的行程等数据。
摇杆的行程为有符号short整型数据，扳机行程为无符号short整型。其中摇杆X轴值为负数时，表明摇杆被拨向左侧，为正数时表明摇杆被拨向右侧。摇杆Y轴值为负时，
表明摇杆被拨向下方，为正时则是被拨向上方。摇杆按键及其他按键均为无符号单字节数据，读数为1时，表明按键被按下，读数为0时，则按键未触发。
<br><br>


###  __1.19 `SkeletonArmsData`__
__声明 ：__
```
typedef struct SkeletonArmsData_t
{
    uint16_t LArm1;
    uint16_t LArm2;
    uint16_t LArm3;
    uint16_t LArm4;
    uint16_t LArm5;
    uint16_t LArm6;
    uint16_t LArm7;
    uint16_t RArm1;
    uint16_t RArm2;
    uint16_t RArm3;
    uint16_t RArm4;
    uint16_t RArm5;
    uint16_t RArm6;
    uint16_t RArm7;
    Joystick LJoyS;  // Left Joystick
    Joystick RJoyS;  // Right Joystick
    uint64_t timestamp;
} SkeletonArmsData;
```

__文件：__`typedef.h`

C/C++接口通用结构体。外骨骼及手柄所有关节的和按键的原始数据的模型，在数采过程中可通过对应的数据访问接口获取当前时刻SDK采集到的上肢外骨骼设备所有关节的角度数据。
其中关节角度数据均为uint16_t类型，取值为实际角度值*100。LJoyS代表左手手柄，RJoyS代表右手手柄。时间戳即为SDK采样获取该帧数据时刻的系统时间。
<br><br>


###  __1.20 `MainBatteryState`__
__声明 ：__
```
typedef struct MainBatteryState_t
{
    int16_t  Currency; // Positive for charging, negative for discharging, in mA
    uint16_t Voltage;  // As in mV
    uint16_t RemainPower;  // Percentage of remaining power
    uint16_t Temperature;  // As in centigrade value divided by 10
    uint16_t StatusBitmap;
    uint16_t Reserved;
} MainBatteryState;
```
<br><br>


###  __1.21 `SystemStatus`__
__声明：__
```
typedef struct SystemStatus_t
{
    BOOL  Enabled    : 1;  // Whether sensors are enabled
    UCHAR Reserved1  : 2;
    BOOL  WifiState  : 1;
    BOOL  BootState  : 1;
    BOOL  NeedCharge : 1;
    BOOL  Reserved2  : 1;
    BOOL  LJoyConn   : 1;
    BOOL  RJoyConn   : 1;
    BOOL  Reserved2  : 7;
} SystemStatus;
```
__文件：__`typedef.h`

C/C++接口通用结构体。DexCap数采设备的主控系统状态图。其中：

    - Enabled： 表示设备传感器是否全部使能。
    - Reserved1： 系统保留字段，暂无意义。
    - WifiState： 表示设备Wifi连接状态，1为已连接，0为空闲。
    - BootState ： 设备固件的bootloader状态，为1时表明设备固件已进入bootloader，等待升级。
    - NeedCharge： 该字段为1时，表明设备主控的电池电量很低，需要充电才能维持设备正常运行
    - Reserved2 ： 系统保留字段，暂无意义。
    - LJoyConn  ： 左手手柄是否已连接
    - RJoyConn  ： 右手手柄是否已连接
    - Reserved2 ： 系统保留字段，暂无意义。


###  __1.22 `SuitStatusData`__
__声明：__
```
typedef struct SuitStatusData_t
{
    SkeletonArmsData jointData;
    MainBatteryStatus mainBatteryState;
    SystemStatus systemStatus;
} SuitStatusData
```

__文件：__`typedef.h`

C/C++接口通用结构体。DexCap套设备所有模块的关节角度数据，手柄按键状态及行程数据，主控板电池状态数据，主控板系统状态数据。
关于各字段数据语义的详细说明，分别参考SkeletonArmsData，MainBatteryStatus，以及SystemStatus的说明。
<br><br>


###  __1.23 `DexCapSuitDataProc`__
__声明 ：__
```typedef void (* DexCapSuitDataProc)(const SuitStatusData *);```

__文件：__`typedef.h`

C/C++通用接口，函数指针声明，该函数接受一个SuitStatusData的指针作为入参。
用户可按照该声明以及自己的数据处理逻辑，实现自己的DexCap套设备数据处理函数，并将该函数通过register_joint_data_callback()接口注册给DexCap SDK作为数据处理的回调函数。
SDK内部逻辑会将实时采集到的数据同步时间戳之后装载到一个SuitStatusData临时对象中，并将该对象的指针作为输入参数传递给该回调函数。
<br><br>


##  __2 C++接口类__

###  __2.1 `DexCapSuit`__
__声明 ：__
```
class DexCapSuit;
```
__文件：__`DexCap.hpp`

C++接口核心类，DexCapSuit代表一套完整的DexCap套设备，即包括上肢外骨骼和左右手手柄。
DexCapSuit类提供DexCap套设备向用户开放的所有管理接口和数据访问接口。包括与设备的连接管理，状态管理和监测，实时数据访问。
DexCapSuit类不强制要求上述所有设备在线才能使用该类的对象，用户在单独使用上肢外骨骼，左右手手套这些设备中的任一设备时，都可以使用该类的实例对象对单个设备进行操作。
同时DexCapSuit类不支持在同一实例中使用多个同类型设备，所有自设备数必须为唯一，即使用DexCapSuit对象时，仅支持单一的上肢外骨骼，以及单一的左/右手手套。
其中任一设备的连接数量超过1时，SDK将会使用后连接的设备信息覆盖前面已连接的同类设备，前面的设备在当前程序实例中将不再可用。
<br><br>


###  __2.2 `DexCapSuit构造函数`__
__声明 ：__
```
DexCapSuit() = delete;
explicit DexCapSuit(AdapterType connectionType, ProductVersion version=V4C1);
```
__文件：__`DexCap.hpp`

DexCapSuit类不支持无参构造，用户在构造实例时必须提供产品版本号(目前仅完成V4设备兼容)信息。

__参数说明：__

| 参数名    | 参数类型                | 含义                                                     | 补充说明                                             |
|--------|---------------------|--------------------------------------------------------|--------------------------------------------------|
| connectionType | AdapterType  | 连接DexCap设备的方式，如WIREDUSB表示使用USB有线连接，WLAN_TCP则表示使用Wifi连接 |  |
| version | ProductVersion | DexCap设备的硬件版本                                          | V4用户请忽略该参数                                       |
<br><br>


###  __2.3 `DexCapSuit::ConnectDevice`__
__声明 ：__
```
virtual ExoApparatus ConnectDevice(const std::string & deviceAddress);
```
__文件：__`DexCap.hpp`

通过给定的设备地址，连接DexCap设备。如果是通过串口连接，设备地址即PC系统通过DexCap设备的连接适配器所识别出的设备名称。
例如当DexCap设备通过USB串口连接至电脑时，适配器设备名即为系统为该串口分配的枚举名。通常在Windows系统上，该名称为COM*。
在Linux上该名称通常为/dev/ttyUSB*或者/dev/ttyACM*。如果设备使用Wifi连接，设备地址即设备连接到无线局域网后的IP地址。

__参数说明：__

| 参数名    | 参数类型                | 含义                      | 补充说明                                                  |
|--------|---------------------|-------------------------|-------------------------------------------------------|
| deviceAddress | const std::string & | 设备的串口路径(枚举)名，或无线局域网IP地址 | 对于串口，Windows上为COM*，Linux上通常为/dev/ttyUSB*或/dev/ttyACM* |

__返回值说明：__

返回类型ExoApparatus表示DexCap设备的类型，如上肢外骨骼，手套或其他。函数调用时，SDK与设备的连接适配器发起连接。
连接成功建立后，SDK会自动读取设备的固件信息以获得设备的类型，ID等基本信息。
设备基本信息成功加载后，函数调用返回该设备的真实类型。如未能成功建立连接或未能成功从固件加载设备信息，则返回UnDef，表示未知的设备。
如果返回UnDef，并不总是表示该设备不是DexCap设备。如备故障，不稳定的连接(如USB供电不稳定，或WLAN网络不稳定，环境干扰强等)，或是设备
在前一次采集任务结束后未被置为待机状态(即传感器使能和采样未关闭)，均有可能导致设备固件未能在预计时间将设备信息上报给SDK，从而使得
SDK加载设备信息失败而返回UnDef。遇到这种情况时，如用户确信所使用的设备是DexCap设备，可尝试将设备下电再重新上电后，重新发起连接。
如多次重连均失败后，则考虑设备已经故障。
<br><br>


###  __2.4 `DexCapSuit::DisconnectDevice`__
__声明 ：__
```
virtual bool DisconnectDevice(const std::string & deviceAddress);
virtual bool DisconnectDevice(ExoApparatus device);
```
__文件：__ `DexCap.hpp`

该函数有两个重载形式。一个通过给定的连接适配器设备名与设备断开连接，第二个使用设备的类型。
SDK与设备断开时，会主动将设备的传感器使能关闭，并在固件层面关闭采样和数据传输。

__参数说明：__

| 参数名    | 参数类型                | 含义                                              | 补充说明 |
|--------|---------------------|-------------------------------------------------|------|
| adapterName | const std::string & | 系统分配给设备适配器的枚举名                                  |      |
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__

连接成功断开则返回true，失败则返回false。
在连接断开过程中，若设备固件未能成功将传感器下使能，或未能成功关闭数据传输，将导致该函数返回false。
此时建议用户将设备下电，以保证设备进入省电状态，并在下次使用时重新上电处于正确的状态。
<br><br>

###  __2.5 `DexCapSuit::GetAdapterName`__
__声明 ：__
```
std::string GetAdapterName(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

用于获取已连接的DexCap设备的连接适配器名。例如已通过USB串口连接的上肢外骨骼，可通过该函数获取其串口枚举名，如在Linux上应为/dev/ttyACM*，在Windows上则为COM*。


__参数说明：__

| 参数名    | 参数类型                | 含义                                              | 补充说明 |
|--------|---------------------|-------------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__

连接成功断开则返回系统分配给该设备的连接适配器名称，失败（通常为指定设备未连接的情况下）则返回空字符串。
<br><br>

###  __2.6 `DexCapSuit::productVersion`__
__声明 ：__
```
const ProductVersion & productVersion() const;
```
__文件：__ `DexCap.hpp`

用于获取当前DexCapSuit实例对应的产品版本。

__返回值说明：__
V4即对应DexCap V4设备，V3则对应DexCap V3.5版的设备。
<br><br>


###  __2.7 `DexCapSuit::GetDeviceID`__
__声明 ：__
```
uint8_t GetDeviceID(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

用于获取指定的DexCap设备的设备ID，在产品出厂时写入固件，作为单个DexCap设备的标识，通常默认为1。
目前DexCap SDK并不使用该ID来参与设备的区分和操作。该ID仅作为未来DexCap设备被用于非套装使用场景(即多个同类型设备在同一应用程序中同时在线使用)的预留。

__参数说明：__

| 参数名    | 参数类型                | 含义                                             | 补充说明 |
|--------|---------------------|------------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
指定设备的ID。如指定的设备未连接，则返回0;
<br><br>


###  __2.8 `DexCapSuit::getSerialNumber`__
__声明 ：__
```
std::string getSerialNumber(ExoApparatus device);
```
__文件：__ `DexCap.hpp`

用于获取DexCap设备的出厂序列号。序列号的格式和规则详见用户说明文档。

__参数说明：__

| 参数名    | 参数类型                | 含义                                             | 补充说明 |
|--------|---------------------|------------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
以字符串的形式返回指定设备出厂序列号。如指定的设备未连接，则返回空字符串;
<br><br>


###  __2.9 `DexCapSuit::Start`__
__声明 ：__
```
bool Start(ExoApparatus device) const;
DEX_RETURN Start();
```
__文件：__ `DexCap.hpp`

该函数有两个重载形式。调用该函数使设备传感器使能，并开启固件的实时采样和数据传输。
重载形式1：带ExoApparatus参数的重载，对指定的DexCap设备开启实时数据采样。
重载形式2：不带参数的重载，对当前已连接的所有DexCap设备开启实时数据采样。
开启成功后，SDK会将设备固件实时采集到的数据缓存用于后续处理，并传给通过用户注册的处理数据的回调函数。

__参数说明：__

| 参数名    | 参数类型                | 含义                                              | 补充说明 |
|--------|---------------------|-------------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
重载形式1：成功返回true, 失败则返回false。
重载形式2：若全部设备开启成功，则返回DEX_SUCCESS。 
         若全部失败则返回DEX_ERROR。
         若部分设备开启成功，部分设备失败，则返回DEX_SUCCESS_WITH_INFO，此时用户可通过调用getErrorMessage()来获知具体错误信息；
<br><br>

###  __2.10 `DexCapSuit::IsConnected`__
__声明 ：__
```
bool IsConnected(ExoApparatus device);
```
__文件：__ `DexCap.hpp`

获知指定的设备是否已经连接。

__参数说明：__

| 参数名    | 参数类型                | 含义                                                       | 补充说明 |
|--------|---------------------|----------------------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
成功返回true, 失败则返回false。
<br><br>


###  __2.11 `DexCapSuit::IsRunning`__
__声明 ：__
```
bool IsRunning() const;
bool IsRunning(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

该函数具有两个重载形式。
重载1：无参的重载。获知是否所有设备均已启动采样。
重载2：带有ExoApparatus参数的重载。获知指定的设备是否已启动采样。

__参数说明：__

| 参数名    | 参数类型                | 含义                                       | 补充说明 |
|--------|---------------------|------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
成功返回true, 失败则返回false。
<br><br>


###  __2.12 `DexCapSuit::Pause`__
__声明 ：__
```
DEX_RETURN Pause();
bool Pause(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

该函数具有两个重载形式。
重载1：无参的重载。使当前已连接的所有设备停止采样。
重载2：带有ExoApparatus参数的重载。使指定的设备停止采样。

__参数说明：__

| 参数名    | 参数类型                | 含义                                       | 补充说明 |
|--------|---------------------|------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
成功返回true, 失败则返回false。
<br><br>


###  __2.13 `DexCapSuit::Resume`__
__声明 ：__
```
DEX_RETURN Resume();
bool Resume(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

与Pause()接口对应，恢复数据采样。该函数具有两个重载形式。
重载1：无参的重载。使当前已连接的所有设备恢复采样。
重载2：带有ExoApparatus参数的重载。使指定的设备恢复采样。

__参数说明：__

| 参数名    | 参数类型                | 含义                                       | 补充说明 |
|--------|---------------------|------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
成功返回true, 失败则返回false。
<br><br>


###  __2.14 `DexCapSuit::Close`__
__声明 ：__
```
DEX_RETURN Close();
bool Close(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

停止设备采样并与设备断开连接。该函数具有两个重载形式。
重载1：无参的重载。使当前已连接的所有设备均停止采样，并断开连接。
重载2：带有ExoApparatus参数的重载。使指定的设备停止采样，并断开连接。

__参数说明：__

| 参数名    | 参数类型                | 含义                                       | 补充说明 |
|--------|---------------------|------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
成功返回DEX_SUCCESS。全部失败则返回DEX_ERROR。
当函数返回DEX_SUCCESS_WITH_INFO时，通常有以下几种情况：
1. 部分设备未能停止固件的实时采样和数据传输。此时需要将设备下电确保下次使用时，设备上电处于正常状态
2. 部分设备未能成功将传感器下使能，此时无需对设备强制下电，不影响下次SDK与设备的正常连接和工作。但传感器始终在工作，会耗费电池电量。如需使设备节能，则可以将设备下电。
3. 所有设备均出现以上两种情况中的任意一种。
<br><br>


###  __2.15 `DexCapSuit::GetAdapterType`__
__声明 ：__
```
AdapterType GetAdapterType() const;
```
__文件：__ `DexCap.hpp`

获知当前SDK与DexCap套设备的连接类型。

__返回值说明：__
DexCap设备的连接适配器类型。
如使用USB串口连接，则返回WIREDUSB。
如使用Wifi连接，则返回WLAN_TCP。
若无任何设备连接，则返回INVALID。
<br><br>


###  __2.16 `DexCapSuit::GetDeviceType`__
__声明 ：__
```
ExoApparatus GetDeviceType(const std::string & adapterAddress) const;
```
__文件：__ `DexCap.hpp`

获知指定的连接适配器设备名的DexCap设备类型。

__参数说明：__

| 参数名    | 参数类型                | 含义                                       | 补充说明 |
|--------|---------------------|------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody/JSBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
指定的适配器设备地址对应的DexCap设备类型。对于V4C1设备，该函数返回值永远为UpBody或JSBody。
<br><br>


###  __2.17 `DexCapSuit::GetSuitJointState`__
__声明 ：__
```
const SuitStatusData & GetSuitJointState() const;
```
__文件：__ `DexCap.hpp`

获取DexCap套设备所有关节当前时刻的角度数据，以及系统当前时刻的状态数据。
系统状态数据包括主控电池的电量，实时电流等状态数据，已经SystemStatus中列举的数据。

__返回值说明：__
SuitStatusData的const引用。关于SuitStatusData对象中数据的详细说明请参见2.1.16的说明。应用程序运行过程中，SDK会
将从设备采集到的数据实时更新到内部缓存中，因而SuitStatusData中的数据会不断的被更新。建议使用该接口获取设备关节数据
的用户，在取得该对象引用时即将其中数据拷贝至自己应用程序的缓存中，以保持数据的有效性。
<br><br>


###  __2.18 `DexCapSuit::GetArmsJointsState`__
__声明 ：__
```
const SkeletonArmsData & GetArmsJointsState() const;
```
__文件：__ `DexCap.hpp`

获取DexCap上肢外骨骼设备中所有关节当前时刻的角度数据，即双臂和腰部自由度的角度数据。

__返回值说明：__
SkeletonArmsData的const引用。关于SkeletonArmsData对象中数据的详细说明请参见1.19的说明。应用程序运行过程中，SDK会
将从设备采集到的数据实时更新到内部缓存中，因而SkeletonArmsData中的数据会不断的被更新。建议使用该接口获取设备关节数据
的用户，在取得该对象引用时即将其中数据拷贝至自己应用程序的缓存中，以保持数据的有效性。
<br><br>


###  __2.19 `DexCapSuit::GetLJoyStickState`__
__声明 ：__
```
const Joystick & GetLJoyStickState() const;
```
__文件：__ `DexCap.hpp`

获取左手手柄的状态数据。

__返回值说明：__
Joystick的const引用。关于Joystick对象中数据的详细说明请参见1.18的说明。应用程序运行过程中，SDK会
将从设备采集到的数据实时更新到内部缓存中，因而Joystick中的数据会不断的被更新。建议使用该接口获取设备关节数据
的用户，在取得该对象引用时即将其中数据拷贝至自己应用程序的缓存中，以保持数据的有效性。
<br><br>


###  __2.20 `DexCapSuit::GetRJoyStickState`__
__声明 ：__
```
const Joystick & GetRJoyStickState() const;
```
__文件：__ `DexCap.hpp`

获取右手手柄的状态数据。

__返回值说明：__
Joystick的const引用。关于Joystick对象中数据的详细说明请参见1.18的说明。应用程序运行过程中，SDK会
将从设备采集到的数据实时更新到内部缓存中，因而Joystick中的数据会不断的被更新。建议使用该接口获取设备关节数据
的用户，在取得该对象引用时即将其中数据拷贝至自己应用程序的缓存中，以保持数据的有效性。
<br><br>


###  __2.21 `DexCapSuit::IsChargeNeeded`__
__声明 ：__
```
bool IsChargeNeeded(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

获知当前指定设备的电池是否需要充电。如果用户使用WLAN进行通信，且设备未插入USB供电进行充电时，设备的角度传感器采集的数据准确性
取决于其供电状况，当电压低于一定的阈值时，传感器采集的数据会失真，这个时候需要对设备的电池进行充电。DexCap系统会根据电池当前
的状态以及设定的阈值来判断当前电池是否需要充电。

__返回值说明：__
如果设备需要充电，返回true，如果设备电池仍能保持传感器正常工作，则返回false。
<br><br>


###  __2.22 `DexCapSuit::GetBatteryLevel`__
__声明 ：__
```
uint16_t GetBatteryLevel(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

获取当前时刻指定设备电池当前的电压值，uint16_t类型，取值为实际电压值*100。

__参数说明：__

| 参数名    | 参数类型                | 含义                                       | 补充说明 |
|--------|---------------------|------------------------------------------|------|
| device | ExoApparatus         | 设备类型，如上肢外骨骼(UpBody),或手套(LGlove/RGlove)，等 |      |

__返回值说明：__
指定设备的电池当前时刻的电压值，取值为实际电压值*100。
<br><br>


###  __2.23 `DexCapSuit::GetMainBatteryStatus`__
__声明 ：__
```
const MainBatteryStatus *GetMainBatteryStatus() const;
```
__文件：__ `DexCap.hpp`

获取当前时刻上肢外骨骼主电池的状态数据。

__返回值说明：__
指向SDK内部缓存的MainBatteryStatus对象的常量指针。关于MainBatteryStatus内部数据结构的说明，请参考1.22。
<br><br>


###  __2.24 `DexCapSuit::GetFirmwareVersion`__
__声明 ：__
```
std::string GetFirmwareVersion(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

获取指定设备中的固件版本号。固件版本号的格式通常为由“.”分为4段，最高位为主版本号(产品或设计变更)，第二段为副版本号(功能更新，改进等)，第三段为更新号(少量功能改进，bug修复为主)，
最后一段则为build号

__参数说明：__

| 参数名    | 参数类型                | 含义                                   | 补充说明 |
|--------|---------------------|--------------------------------------|------|
| device | ExoApparatus         | 设备类型，上肢外骨骼UpBody，或手套(LGlove/RGlove)  |      |
<br><br>

__返回值说明：__

设备的固件版本号
<br><br>


###  __2.25 `DexCapSuit::anyError`__
__声明 ：__
```
bool anyError(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

获知指定的设备当前是否发生错误。通常在向所有设备或部分指定设备发送管理类指令后，如Start(), Resume(), Close(), VibeMotors()等指令，当接口返回结果为false或非DEX_SUCCESS
时，又或者返回值为void的接口被调用完后，用户在对设备进行后续操作未能获得预期的行为或数据反馈时，则可以使用改接口判断设备是否处于错误状态。

__参数说明：__

| 参数名    | 参数类型                | 含义                                   | 补充说明 |
|--------|---------------------|--------------------------------------|------|
| device | ExoApparatus         | 设备类型，上肢外骨骼UpBody，或手套(LGlove/RGlove)  |      |

__返回值说明：__

如果设备处于错误状态，返回true，如系统未检测到或记录指定设备存在的错误，则返回false。
<br><br>


###  __2.26 `DexCapSuit::getErrorCode`__
__声明 ：__
```
ErrorCode getErrorCode() const;
ErrorCode getErrorCode(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

该函数有两个重载版本。
重载1：不带参数。获取DexCapSuit套设备实例当前的错误码，如当前所有设备状态正常，即无错误，则返回ERROR_NONE。
重载2：带ExoApparatus参数。获取指定设备当前的错误码，如设备当前状态正常，则返回ERROR_NONE。

__参数说明：__

| 参数名    | 参数类型                | 含义                                   | 补充说明 |
|--------|---------------------|--------------------------------------|------|
| device | ExoApparatus         | 设备类型，上肢外骨骼UpBody，或手套(LGlove/RGlove)  |      |

__返回值说明：__

如果设备当前存在错误，则返回错误码，若设备状态正常则返回ERROR_NONE。
<br><br>


###  __2.27 `DexCapSuit::getErrorMessage`__
__声明 ：__
```
const std::string & getErrorMessage();
std::string getErrorMessage(ExoApparatus device) const;
```
__文件：__ `DexCap.hpp`

该函数有两个重载版本。
重载1：不带参数。获取DexCapSuit套设备实例当前错误的具体文本信息，如当前所有设备状态正常，则返回空字符串。
重载2：带ExoApparatus参数。获取指定设备当前错误的具体文本信息，如当前设备状态正常，则返回空字符串。

__参数说明：__

| 参数名    | 参数类型                | 含义                                   | 补充说明 |
|--------|---------------------|--------------------------------------|------|
| device | ExoApparatus         | 设备类型，上肢外骨骼UpBody，或手套(LGlove/RGlove)  |      |

__返回值说明：__

如果设备当前存在错误，则返回错误码，若设备状态正常则返回ERROR_NONE。
<br><br>


###  __2.28 `DexCapSuit::registerStatusDataProc`__
__声明 ：__
```
void registerStatusDataProc(const DexCapSuitDataProc & callback);
```
__文件：__ `DexCap.hpp`

注册一个形式如DexCapSuitDataProc的声明的函数，作为接收和处理DexCapSuit套设备所有关节原始实时数据的回调函数。
该函数接收一个类型为const SuitStatusData *的指针作为参数。
关于用户实现该函数的详细说明，请参考1.23章节。

__参数说明：__

| 参数名    | 参数类型                | 含义                       | 补充说明 |
|--------|---------------------|--------------------------|------|
| callback | DexCapSuitDataProc   | 用户希望注册给DexCapSuit实例的回调函数 |      |
<br><br>
