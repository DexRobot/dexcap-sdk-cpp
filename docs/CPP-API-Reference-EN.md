# DexCap SDK C\+\+ API Reference

# **Version** V2\.1\.0

### 1\.1 Overview

DexCap SDK C\+\+ API V2\.1\.0 corresponds to the hardware device version V4 of the DexCap product\. This SDK provides basic interfaces for managing, controlling, and accessing data for all functions of the DexCap V4 device, while also providing C\-style interfaces for the corresponding functions\.

## **2\. DexCap SDK**** Directory structure**

### 2\.1 Directory structure layout



```YAML
dexcap-sdk-cpp/
├── include/                   # Header File Directory, all Header Files declaring types and interfaces exposed by DexCap SDK are located in this directory
│   ├── cpp                    # Directory for Header Files providing declarations of C++ classes and interfaces. C++ developers use the APIs provided by the Header Files in this directory
│       ├── DexCap.hpp         # Declaration of the interface class DexCapSuit for managing and controlling the DexCap device set
│       ├── TypeDef.hpp        # Declaration of basic types, constants, and accessible data types required for using the DexCap device set
│       ├── Utils.hpp          # Several simple utility classes provided by the SDK, such as common string and date-time stamp processing
│   ├── configuration.h        # Information and functional interfaces such as basic types, constants, and system interface configurations provided in C form.
│                              # Includes a settings utility class compatible with V3.5 (compatibility testing is not yet complete, V3.5 users please do not use for the time being, official announcements will be made via official channels after V3.5 compatibility testing is completed)
│   ├── dexcap.h               # Management and data acquisition interfaces for the DexCap device set provided in C form, corresponding to the functional interfaces provided by DexCap.hpp
│   ├── typedef.h              # Structures, basic types, etc. describing the basic data model provided in C form, the declarations in TypeDef.hpp depend on this Header File
├── libs/                      # Directory where dynamic library files are located
│   ├── linux/                 # Directory for dynamic library files on the Linux platform
│   |   ├── libDexCap.so       # Dynamic library file for the Linux (currently only supports Ubuntu) platform of DexCap SDK
│   |   ├── Others             # Dependent third-party libraries
│   ├── windows/               # Directory for dynamic library files on the Windows platform
│   |   ├── DexCap.dll         #.dll file for DexCap SDK
│   |   ├── DexCap.lib         #.lib file for DexCap SDK
│   |   ├── Others             # Dependent third-party libraries
├── conf/                      # Configuration files and scripts used by V3.5, V4 users please ignore
│   |   ├── config.yaml        # SDK configuration file for DexCap gloves and exoskeleton devices
│   |   ├── env_setup.sh       # Environment configuration file, automatically searches for serial devices in the system and grants permissions
├── examples/                  # Directory where dynamic library files are located
│   ├── example.cpp            # C++ example code
│   ├── c_example.c            # C example code
│   ├── example_defs.h         # Header File for the example program
│   ├── CMakeLists.txt         # CMake project file for building the example program
```



### 1\.2 Responsibility Description for Key Documents

|Document|Function|Dependency|
|---|---|---|
|`cpp/DexCap.hpp`|The core C\+\+ API Header File of DexCap SDK, which provides all management interfaces and data access interfaces open to users by DexCap devices\.||
|`cpp/TypeDef.hpp`|The basic C\+\+ data model and constant declarations of DexCap SDK rely on the C\-style declarations provided by typedef\.h\.||
|`cpp/Utils.hpp`|Function interfaces for string, date, and timestamp related utilities\.||
|`typedef.h`|The basic data types used in the C API of DexCap SDK, as well as the declarations of data model structures, and some content in cpp/TypeDef\.hpp depend on this Header File\.||
|`dexcap.h`|The core C API Header File of DexCap SDK, which provides all management interfaces and C language interfaces for data access exposed to users by DexCap devices\.||
|`configuration.h`|Interface declaration for retrieving device configuration information, intended for V3\.5 users; V4 users please ignore\.|Depends on the yaml library<br>|
|`libDexCap.so`|Dynamic library files for the Linux platform of DexCap SDK|Depends on third\-party libraries such as libserial|
|`DexCap.dll`, `DexCap.lib`|Dynamic library files for the Windows platform of DexCap SDK|Depends on Windows SDKSDK|
|`config.yaml`|Data acquisition device configuration information, including the device's serial port name, baud rate, data acquisition serial port adapter type \(USB wired or 2\.4G wireless\), etc\. For V3\.5 users only; V4 users please ignore\.||
|`env_setup.sh`|Environment setup script on Linux platform, used to scan USB serial devices in the system and grant permissions to the current user\. For V3\.5 users only, V4 users please ignore\.|Requires sudo privileges|
|`c_example.cpp`|C sample code demonstrating the basic usage of DexCap SDK C API|Header Files and Dynamic Libraries under the include Directory|
|`example.cpp`|C\+\+ sample code demonstrating the basic usage of the DexCap SDK C\+\+ API|Header files and dynamic libraries under the include/cpp directory|





## **2\. API Reference**

### 2\.1 Common Data Types

#### 2\.1\.1 `DEXCAP_SUIT_HANDLE`

**Statement****：**`#define DEXCAP_SUIT_HANDLE void *`

**Document****：**`typedef.h`

Used by C interface users, it represents the instance handle for accessing DexCap devices\. Users need to provide this handle when connecting to and accessing the management functions and data interfaces of DexCap devices\.



#### 2\.1\.2 `DEX_RETURN`

**Statement****：**`typedef enum { ... } DEX_RETURN;`

**Document****：**`typedef.h`

C/C\+\+ interfaces are generic, with an enumeration type\. Return codes for non\-data access interfaces in the DexCap API, such as management interfaces, represent the result of command execution\. Usually, after successful execution, an interface returns DEX\_SUCCESS, and in case of failure, it returns DEX\_ERROR\.

If an interface has mixed semantics or logic, such as partial success in execution results, it returns DEX\_SUCCESS\_WITH\_INFO, indicating that the interface execution is completed but with partial failure or error\. Users can use interfaces such as anyError\(\), getErrorCode\(\), and getErrorMessage\(\) to obtain whether an error has occurred, as well as the specific error code and error message\.



#### 2\.1\.3 `ErrorCode`

**Statement****：**`typedef enum { ... } ErrorCode;`

**Document****：**`typedef.h`

C/C\+\+ interface is universal, an enumeration type, defining all error codes exposed by the DexCap SDK\.



#### 2\.1\.4 `ADAPTER_TYPE`

**Statement****：**`typedef enum { ... } ADAPTER_TYPE;`

**Document****：**`typedef.h`

C/C\+\+ interface is universal, an enumeration type representing all types of connection adapters supported by DexCap devices, such as WIREDUSB and BLUETOOTH, which respectively represent USB and Bluetooth connections\. It is used when creating a DexCap instance handle or object\.



#### 2\.1\.5 `DEXCAP_DEVICE_TYPE`

**Statement****：**`typedef enum { ... } DEXCAP_DEVICE_TYPE;`

**Document****：**`typedef.h`

C/C\+\+ interface is universal, an enumeration type representing the type of DexCap devices\. For example, UpBody represents the upper limb exoskeleton, LGlove and RGlove represent the left and right hand gloves respectively, and IMUnit represents the lumbar IMU\. For V4 devices, after the SDK successfully connects to the device, it will automatically recognize the real type of the device and assign values to the device's type parameters\.



#### 2\.1\.6 `DEXCAP_DEVICE_TYPE`

**Statement****：**`typedef enum { ... } DEXCAP_DEVICE_TYPE;`

**Document：**`typedef.h`

C/C\+\+ interface is universal, an enumeration type representing the type of DexCap device\. For example, UpBody represents the upper limb exoskeleton, and LGlove and RGlove represent the left and right hand gloves respectively\. For V4 devices, after the SDK successfully connects to the device, it will automatically recognize the real type of the device and return this value\.



#### 2\.1\.7 `ExoSkeletonJointID`

**Statement****：**`typedef enum { ... } ExoSkeletonJointID;`

**Document：**`typedef.h`

C/C\+\+ interface is universal\. An enumeration type representing the joint IDs of all degrees of freedom of the DexCap upper limb exoskeleton\. This type maps joints, and assigns IDs in the order of waist \- left arm \- right arm, following the principle of proximal \- distal\.



#### 2\.1\.8 `ProductVersion`

**Statement****：**`typedef enum { ... } ProductVersion;`

**Document：**`typedef.h`

The C/C\+\+ interface is universal\. The enumeration type represents the product version of DexCap\. Currently, V3 represents the V3\.5 device, and V4 represents the V4 device\.



#### **2\.1\.9 ****`GloveJointAngles`**

**Statement**** ：**

```Plaintext
typedef struct GloveJointAngles_t
{
    uint16_t ThumbDIP;
    uint16_t ThumbPIP;
    uint16_t ThumbMCP;
    uint16_t ThumbSWP;
    uint16_t ThumbROP;
    uint16_t IndexDIP;
    uint16_t IndexPIP;
    uint16_t IndexMCP;
    uint16_t IndexSWP;
    uint16_t MiddleDIP;
    uint16_t MiddlePIP;
    uint16_t MiddleMCP;
    uint16_t MiddleSWP;
    uint16_t RingDIP;
    uint16_t RingPIP;
    uint16_t RingMCP;
    uint16_t RingSWP;
    uint16_t LittleDIP;
    uint16_t LittlePIP;
    uint16_t LittleMCP;
    uint16_t LittleSWP;
    uint16_t BatteryState;
    uint32_t ErrorMask;
    uint64_t timestamp;
} GloveJointAngles;
```

**Document：**`typedef.h`

C/C\+\+ interface is universal, a structure\. It is a model for the raw angle data of all joints of the glove\. During the data acquisition process, the angle data of all joints of the glove device, battery status, joint sensor status bitmap, and timestamp collected by the SDK at the current moment can be obtained through the corresponding data access interface\.

Among them, the joint angle data are all of type uint16\_t, with values being the actual angle value \* 100\. The timestamp is the system time at the moment when the SDK samples and acquires this frame of data\.



#### **2\.1\.10 ****`SkeletonJointAngles`**

**Statement**** ：**

```Plaintext
typedef struct SkeletonJointAngles_t
{
    uint16_t LArm1;
    uint16_t LArm2;
    uint16_t LArm3;
    uint16_t LArm4;
    uint16_t LArm5;
    uint16_t LArm6;
    uint16_t LArm7;
    uint16_t LArm8;
    uint16_t LArm9;
    uint16_t RArm1;
    uint16_t RArm2;
    uint16_t RArm3;
    uint16_t RArm4;
    uint16_t RArm5;
    uint16_t RArm6;
    uint16_t RArm7;
    uint16_t RArm8;
    uint16_t RArm9;
    uint16_t Back1;
    uint16_t Back2;
    uint16_t Back3;
    uint16_t Back4;
    uint16_t Back5;
    uint16_t Reserved;
    uint64_t timestamp;
} SkeletonJointAngles;
```

**Document：**`typedef.h`

C/C\+\+ interface is universal, a structure\. It is a model for the raw angle data of all joints of the glove\. During data acquisition, the angle data of all joints of the upper limb exoskeleton device and the status bitmap of the joints collected by the SDK at the current moment can be obtained through the corresponding data access interface\.

Among them, the joint angle data are all of type uint16\_t, with the value being the actual angle value \* 100\. The timestamp is the system time at the moment when the SDK samples and acquires this frame of data\.



#### 2\.1\.11 `MainBatteryState`

**Statement**** ：**

```Plaintext
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



#### 2\.1\.12 `InertialUnitData`

**Statement****：**

```Plaintext
typedef struct InertialUnitData_t
{
    double roll;
    double pitch;
    double yaw;
    double quat[4];       // quaternion
    double accel[3];      // accelerometer (x, y, z)
    double gyscp[3];      // gyroscope (x, y, z)
    double magnt[3];      // magnetometer (x, y, z)
    double air_pressure;  // air pressure
    double temp;          // Temperature
    uint32_t system_time; //system time
    uint64_t timestamp;
} InertialUnitData
```

**Document：**`typedef.h`

C/C\+\+ interfaces are universal, a structure\. The IMU module of the waist of the DexCap upper limb exoskeleton\. The IMU module of the DexCap upper limb exoskeleton is an optional device and serves only as an auxiliary functional module\. The DexCap SDK and related software only expose the raw data of the IMU module and do not provide IMU\-based practical applications\.

Capable users can use the raw data on their own, combined with their own scenarios, to implement applications for functions such as posture recognition and mileage calculation\.

Note: DexRobot does not assume responsibility for any losses caused by users' operations and control of devices using the IMU\.




#### 2\.1\.13 `DexCapJointData`

**Statement****：**

```Plaintext
typedef struct DexCapJointData_t
{
    uint32_t mask;
    uint16_t LGlove[24];
    uint16_t ExBody[24];
    uint16_t RGlove[24];
    double   InetMU[19];
    uint64_t timestamp;
} DexCapJointData
```

**Document：**`typedef.h`

C/C\+\+ interface is universal, a structure\. The joint angle data of all modules of the DexCap device are stored in the form of uint16\_t arrays respectively\. Among them, the mask field is a bitmap of available data, which indicates the valid data arrays in the structure\. LGlove, ExBody, and RGlove respectively represent the angle data of all degrees of freedom of the left glove, upper limb exoskeleton, and right glove, with each element taking a value of the actual angle value \* 100\. InetMU is the raw data of the IMU module\. timestamp is the synchronized timestamp when the SDK acquires this set of data\.

The bitmap description of mask is specifically as shown in the following table:

|Member array name|Corresponding mask bit value|meaning|
|---|---|---|
|LGlove|0x8000|Left hand glove data is valid|
|ExBody|0x4000|Upper limb exoskeleton data is valid|
|RGlove|0x2000|Right hand glove data is valid|
|InetMU|0x1000|IMU module data is valid|
||||

#### 2\.1\.14 `DexCapEndPoses`

**Statement****：**

```Plaintext
typedef struct DexCapEndPoses_t
{
    double LArm[4][4];
    double RArm[4][4];
    uint64_t timestamp;
} DexCapEndPoses;
```

**Document：**typedef\.h

C/C\+\+ interface is universal, structure\. Pose data of the end \(wrist\) of both arms of the DexCap upper limb exoskeleton\.



#### 2\.1\.15 `DexCapJointDataProc`

**Statement**** ：**
`typedef void (* DexCapJointDataProc)(const DexCapJointData *);`

**Document：**`typedef.h`

C interface, function pointer Statement, which accepts a pointer to DexCapJointData as an input parameter\.

Users can implement their own DexCap device data processing function according to this Statement and their own data processing logic, and register this function as a data processing callback function with the DexCap SDK through the register\_joint\_data\_callback\(\) interface\.

The internal logic of the SDK will synchronize the timestamps of the real\-time collected data, load it into a temporary DexCapJointData object, and pass the pointer to this object as an input parameter to this callback function\.


#### **2\.1\.16 ****`DexCapStatusDataProc`**

**Statement**** ：**

```Plaintext
using DexCapStatusDataProc = std::function<void (const DexCapJointData *)>;
```

**Document：** `DexCap.hpp`

Specifically for C\+\+ interfaces\. A function wrapper of the form `std::function<void (const DexCapJointData *)>`, with the same functionality as DexCapJointDataProc:

The user implements this function themselves and registers it with the SDK as a callback function\. In this function, the user uses custom logic to receive and process in real\-time the raw data of all joints of the DexCapSuit device\.

This function accepts a constant pointer of type DexCapJointData\. The SDK's internal background thread will load all the collected device joint data into this object in real\-time and pass it to the callback function in the form of a pointer\.

It should be noted that since the SDK will encapsulate the collected data into the DexCapJointData object in real\-time, after the callback function registered by the user is called and completed, the DexCapJointData object will be immediately destroyed or the data it holds will be overwritten by subsequent sampled data\. Therefore, the user needs to immediately copy the data in this pointer to the cache of their own application in the callback function to maintain data validity and avoid program crashes caused by accessing memory after it has been reclaimed\.




#### **2\.1\.17 ****`SuitJointState`**

**Statement**** ：**

```Plaintext
struct SuitJointState
{
    DexCapJointData jointData;
    MainBatteryState mainBatteryState;
    ......
};
```

**Document：**`TypeDef.hpp`\_\_
For C\+\+ interface use, the DexCap set device's complete data\. This structure contains joint angle data for all degrees of freedom of the DexCap set device, as well as status information for the main battery of the DexCap upper limb device\.


### **2\.2 C\+\+**** Interface Class**

#### **2\.2\.1 ****`DexCapSuit`**

**Statement**** ：**

```Plaintext
class DexCapSuit;
```

**Document：**`DexCap.hpp`

The core class of the C\+\+ interface, DexCapSuit, represents a complete set of DexCap devices, which includes an upper limb exoskeleton and left/right hand gloves\.

The DexCapSuit class provides all management interfaces and data access interfaces that the DexCap device suite exposes to users, including connection management with the device, status management and monitoring, and real\-time data access\.

The DexCapSuit class does not require all of the above devices to be online in order to use an object of this class\. Users can use an instance of this class to operate on a single device when using any one of the upper limb exoskeleton, left/right hand gloves, etc\.

At the same time, the DexCapSuit class does not support using multiple devices of the same type in the same instance\. The number of all sub\-devices must be unique, i\.e\., when using a DexCapSuit object, only a single upper limb exoskeleton and a single left/right hand glove are supported\.

When the number of connections for any of the devices exceeds 1, the SDK will overwrite the information of previously connected devices of the same type with the information of the subsequently connected device, and the previously connected devices will no longer be available in the current program instance\.


#### **2\.2\.2 ****`DexCapSuit`****` `****constructor function**

**Statement**** ：**

```Plaintext
DexCapSuit() = delete;
explicit DexCapSuit(ProductVersion version, const std::string & configFile = "./config.yaml");
```

**Document：**`DexCap.hpp`

The DexCapSuit class does not support parameterless construction, and users must provide product version number information \(currently only V4 device compatibility is completed\) when constructing an instance\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|version|ProductVersion|Version of DexCap product|The device versions historically published for the DexCap product include V3\.5 and V4\. Currently, the version of the SDK that has completed product adaptation only supports V4\.|
|configFile|const std::string \&|The path of the configuration file, applicable to devices of version V3\.5|V4 users please ignore this parameter\.<br>|
|||||

#### **2\.2\.3 ****`DexCapSuit::ConnectDevice`**

**Statement**** ：**

```Plaintext
virtual ExoApparatus ConnectDevice(const std::string & adapterName, AdapterType adapterType);
```

**Document：**`DexCap.hpp`

Connect to the specified DexCap device via the given adapter device name and adapter type\. The adapter device name refers to the device name recognized by the connection adapter of the DexCap device on the PC system running the SDK and application\.

For example, when a DexCap device is connected to a computer via a USB serial port, the adapter device name is the enumerated name assigned by the system to that serial port\. Typically on Windows systems, this name is COM\.

On Linux, this name is usually /dev/ttyUSB or /dev/ttyACM\*\. If the device uses Bluetooth, the adapter device name is the device's Bluetooth name\.



**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|adapterName|const std::string \&|The enumeration name assigned by the system to the device adapter|For serial ports, it is COM on Windows, and usually /dev/ttyUSB or /dev/ttyACM\* on Linux\.|
|adapterType|AdapterType|Connection adapter type|Refer to 2\.1\.4 for value reference\. Commonly used values for V4 users are WIREDUSB and BLUETOOTH\.|

**Return value description**

The return type ExoApparatus represents the type of DexCap device, such as upper limb exoskeleton, glove, or others\. When the function is called, the connection adapter between the SDK and the device initiates the connection\.

After the connection is successfully established, the SDK will automatically read the device's firmware information to obtain basic information such as the device type, ID, etc\.

After the device's basic information is successfully loaded, the function call returns the real type of the device\. If the connection cannot be successfully established or the device information cannot be successfully loaded from the firmware, UnDef is returned, indicating an unknown device\.

If UnDef is returned, it does not always mean that the device is not a DexCap device or that the device is malfunctioning\. For unstable connections, it may also cause the SDK to fail to load device information successfully or in a timely manner\. For example, an unstable Bluetooth connection may cause the information loading from the firmware to time out, resulting in the function returning UnDef\. In such cases, users can try increasing the number of connection attempts to ensure that the SDK can fully establish a connection with the device\.


#### **2\.2\.4 ****`DexCapSuit::DisconnectDevice`**

**Statement**** ：**

```Plaintext
virtual bool DisconnectDevice(const std::string & adapterName);
virtual bool DisconnectDevice(ExoApparatus device);
```

**Document：** `DexCap.hpp`

This function has two overloaded forms\. One disconnects from the device using the given connection adapter device name, and the second uses the device type\.

When the SDK disconnects from the device, it will actively disable the device's sensor enablement and turn off sampling and data transmission at the firmware level\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|adapterName|const std::string \&|The enumeration name assigned by the system to the device adapter||
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**

If the connection is successfully disconnected, the function returns true; if it fails, it returns false\.

During the disconnection process, if the device firmware fails to disable the sensor or fails to successfully turn off data transmission, the function will return false\.

In this case, it is recommended that the user power off the device to ensure it enters a power\-saving state and then power it on again to be in the correct state for the next use\.


#### **2\.2\.5 ****`DexCapSuit::GetAdapterName`**

**Statement**** ：**

```Plaintext
std::string GetAdapterName(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Used to obtain the name of the connection adapter of the connected DexCap device\. For example, for an upper limb exoskeleton connected via a USB serial port, this function can be used to obtain its serial port enumeration name, which should be /dev/ttyACM on Linux and COM on Windows\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**

If the connection is successfully disconnected, return the name of the connection adapter assigned by the system to the device; if it fails \(usually when the specified device is not connected\), return an empty string\.


#### **2\.2\.6 ****`DexCapSuit::productVersion`**

**Statement**** ：**

```Plaintext
const ProductVersion & productVersion() const;
```

**Document：** `DexCap.hpp`

Used to obtain the product version corresponding to the current DexCapSuit instance\.

**Return value description**
V4 corresponds to the DexCap V4 device, while V3 corresponds to the DexCap V3\.5 device\.


#### **2\.2\.7 ****`DexCapSuit::GetDeviceID`**

**Statement**** ：**

```Plaintext
uint8_t GetDeviceID(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Used to obtain the device ID of a specified DexCap device, which is written into the firmware during product manufacturing and serves as the identifier for a single DexCap device, usually defaulting to 1\.

Currently, the DexCap SDK does not use this ID to distinguish or operate devices\. This ID is only reserved for future scenarios where DexCap devices are used in non\-setting usage scenarios \(i\.e\., multiple devices of the same type are used online simultaneously in the same application\)\.



**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Specify the ID of the device\. If the specified device is not connected, return 0;


#### **2\.2\.8 ****`DexCapSuit::getSerialNumber`**

**Statement**** ：**

```Plaintext
std::string getSerialNumber(ExoApparatus device);
```

**Document：** `DexCap.hpp`

Used to obtain the factory serial number of the DexCap device\. For details on the format and rules of the serial number, please refer to the user instruction manual\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Returns the factory serial number of the specified device as a string\. If the specified device is not connected, returns an empty string\.


#### **2\.2\.9 ****`DexCapSuit::Start`**

**Statement**** ：**

```Plaintext
bool Start(ExoApparatus device) const;
DEX_RETURN Start();
```

**Document：** `DexCap.hpp`

This function has two overloaded forms\. Calling this function enables the device sensor and initiates real\-time sampling and data transmission of the firmware\.

Overloaded form 1: The overloaded form with the ExoApparatus parameter enables real\-time data sampling for the specified DexCap device\.

Overloaded form 2: The overloaded form without parameters enables real\-time data sampling for all currently connected DexCap devices\.

After successful activation, the SDK will cache the data collected in real\-time by the device firmware for subsequent processing and pass it to the callback function registered by the user for data processing\.



**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Overload Form 1: Returns true if successful, false if failed\.

Overload Form 2: Returns DEX\_SUCCESS if all devices are successfully turned on\.

Returns DEX\_ERROR if all devices fail\.

Returns DEX\_SUCCESS\_WITH\_INFO if some devices are successfully turned on while some fail, in which case the user can call getErrorMessage\(\) to obtain specific error information;




#### **2\.2\.10 ****`DexCapSuit::IsConnected`**

**Statement**** ：**

```Plaintext
bool IsConnected(ExoApparatus device);
```

**Document：** `DexCap.hpp`

Determine whether the specified device is already connected\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Return true if successful, false if failed\.



#### **2\.2\.11 ****`DexCapSuit::IsBluetoothConnected`**

**Statement**** ：**

```Plaintext
bool IsBluetoothConnected(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Determine whether the specified device has been connected via Bluetooth\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Return true if successful, false if failed\.



#### **2\.2\.12 ****`DexCapSuit::IsRunning`**

**Statement**** ：**

```Plaintext
bool IsRunning() const;
bool IsRunning(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

This function has two overloaded forms\.

Overload 1: The overload with no parameters\. It determines whether all devices have started sampling\.

Overload 2: The overload with an ExoApparatus parameter\. It determines whether the specified device has started sampling\.



**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Return true if successful, false if failed\.



#### **2\.2\.13 ****`DexCapSuit::Pause`**

**Statement**** ：**

```Plaintext
DEX_RETURN Pause();
bool Pause(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

This function has two overloaded forms\.

Overload 1: The overload with no parameters\. Stops sampling for all currently connected devices\.

Overload 2: The overload with an ExoApparatus parameter\. Stops sampling for the specified device\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Return true if successful, false if failed\.



#### **2\.2\.14 ****`DexCapSuit::Resume`**

**Statement**** ：**

```Plaintext
DEX_RETURN Resume();
bool Resume(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Corresponding to the Pause\(\) interface, this function resumes data sampling\. This function has two overloaded forms\.

Overload 1: The parameterless overload\. Resumes sampling for all currently connected devices\.

Overload 2: The overload with an ExoApparatus parameter\. Resumes sampling for the specified device\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Return true if successful, false if failed\.



#### **2\.2\.14 ****`DexCapSuit::Close`**

**Statement**** ：**

```Plaintext
DEX_RETURN Close();
bool Close(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Stop device sampling and disconnect from the device\. This function has two overloads\.

Overload 1: The overload with no parameters\. Stops sampling and disconnects all currently connected devices\.

Overload 2: The overload with an ExoApparatus parameter\. Stops sampling and disconnects the specified device\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**


If successful, it returns DEX\_SUCCESS\. If all operations fail, it returns DEX\_ERROR\.

When the function returns DEX\_SUCCESS\_WITH\_INFO, there are usually the following situations:

1. Some devices failed to stop real\-time firmware sampling and data transmission\. At this time, it is necessary to power off the devices to ensure that they will be in a normal state when powered on for the next use\.

2. Some devices failed to successfully disable the sensor\. In this case, there is no need to force the device to power off, which does not affect the normal connection and operation between the SDK and the device next time\. However, the sensor remains operational, which will consume battery power\. If you need to make the device energy\-efficient, you can power it off\.

3. All devices exhibit either of the above two situations\.


#### **2\.2\.15 ****`DexCapSuit::GetAdapterType`**

**Statement**** ：**

```Plaintext
AdapterType GetAdapterType() const;
```

**Document：** `DexCap.hpp`

Find out how all current devices are connected to the SDK\. This function does not have an overloaded form that specifies a particular device; instead, it uses the connection status of the device detected first as its return value\.

Normally, the DexCap SDK does not require users to use the same connection method to connect all devices, but it will record cases where the connection methods are inconsistent and reduce the overall sampling rate to that of the connection method with the lowest data transfer rate among all the connection methods used\.

**Return value description**
The type of connection adapter for the device detected first\. If no device is connected, returns INVALID\.



#### **2\.2\.16 ****`DexCapSuit::GetDeviceType`**

**Statement**** ：**

```Plaintext
ExoApparatus GetDeviceType(const std::string & adapterName) const;
```

**Document：** `DexCap.hpp`

Obtain the DexCap device type of the specified connection adapter device name

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
The DexCap device type corresponding to the specified adapter device name\.



#### **2\.2\.17 ****`DexCapSuit::GetSuitJointState`**

**Statement**** ：**

```Plaintext
const SuitJointState & GetSuitJointState() const;
```

**Document：** `DexCap.hpp`

Obtain the angle data of all joints of the DexCap device at the current moment\.

**Return value description**
Constant reference to SuitJointState\. For detailed descriptions of the data in the SuitJointState object, please refer to the description in 2\.1\.16\. During the operation of the application, the SDK will update the data collected from the device to the internal cache in real time, so the data in SuitJointState will be continuously updated\. It is recommended that users who use this interface to obtain device joint data copy the data to their own application's cache when they obtain the reference to this object to maintain data validity\.



#### **2\.2\.18 ****`DexCapSuit::GetBodyJointState`**

**Statement**** ：**

```Plaintext
const SkeletonJointAngles & GetBodyJointState() const;
```

**Document：** `DexCap.hpp`

Obtain the angle data of all joints at the current moment in the DexCap upper limb exoskeleton device, namely the angle data of the degrees of freedom of both arms and the waist\.

**Return value description**
A constant reference to SkeletonJointAngles\. For detailed descriptions of the data in the SkeletonJointAngles object, please refer to the description in 2\.1\.10\. During the operation of the application, the SDK will update the data collected from the device to the internal cache in real time, so the data in SkeletonJointAngles will be continuously updated\. It is recommended that users who use this interface to obtain device joint data copy the data to their own application's cache when they obtain the reference to this object to maintain data validity\.



#### **2\.2\.19 ****`DexCapSuit::GetLeftGloveJointState`**

**Statement**** ：**

```Plaintext
const GloveJointAngles & GetLeftGloveJointState() const;
```

**Document：** `DexCap.hpp`

Obtain the angle data of all joints at the current moment in the DexCap left\-hand glove device\.

**Return value description**
A constant reference to GloveJointAngles, which contains the current angle data of all joints in the left\-hand glove\. For detailed descriptions of the data in the GloveJointAngles object, please refer to the description in 2\.1\.9\.

During the operation of the application, the SDK will update the data collected from the device to the internal cache in real time, so the data in GloveJointAngles will be continuously updated\. It is recommended that users who use this interface to obtain device joint data copy the data to their own application cache when they obtain a reference to this object to maintain data validity\.




#### **2\.2\.20 ****`DexCapSuit::GetRightGloveJointState`**

**Statement**** ：**

```Plaintext
const GloveJointAngles & GetRightGloveJointState() const;
```

**Document：** `DexCap.hpp`

Obtain the angle data of all joints at the current moment in the DexCap right\-hand glove device\.

**Return value description**
A constant reference to GloveJointAngles, containing the angle data of all joints in the right\-hand glove at the current moment



#### **2\.2\.21 ****`DexCapSuit::GetInertialMUJointState`**

**Statement**** ：**

```Plaintext
const InertialUnitData & GetInertialMUJointState() const;
```

**Document：** `DexCap.hpp`

Obtain the angle data of all joints at the current moment in the DexCap right\-hand glove device\.

**Return value description**
A constant reference to InertialUnitData, containing the pose and state data of the waist IMU\.


#### **2\.2\.22 ****`DexCapSuit::IsChargeNeeded`**

**Statement**** ：**

```Plaintext
bool IsChargeNeeded(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Determine whether the battery of the currently specified device needs to be charged\. If the user uses Bluetooth for communication and the device is not plugged into a USB power supply for charging, the accuracy of the data collected by the device's angle sensor depends on its power supply status\. When the voltage is below a certain threshold, the data collected by the sensor will be distorted, and at this time, the device's battery needs to be charged\. The DexCap system will determine whether the current battery needs to be charged based on the battery's current status and the set threshold\.

**Return value description**
Return true if the device needs to be charged, and return false if the device's battery can still keep the sensor functioning properly\.



#### **2\.2\.23 ****`DexCapSuit::GetEndPose`**

**Statement**** ：**

```Plaintext
const DexCapEndPoses & GetEndPose() const;
```

**Document：** `DexCap.hpp`

Obtain the end pose data of the DexCap upper limb exoskeleton device at the current moment, i\.e\., the pose matrices of the left and right wrists at the current moment\.

**Return value description**
Constant reference to DexCapEndPoses\. During the operation of the DexCap system, the SDK will calculate the end poses of the upper limb exoskeleton's arms in real time and use the calculation results to update the internal cached data in real time, i\.e\., the cached DexCapEndPoses object\. It is recommended that users immediately copy the data to their own cache after obtaining it through this interface to maintain data validity\.



#### **2\.2\.24 ****`DexCapSuit::GetBatteryLevel`**

**Statement**** ：**

```Plaintext
uint16_t GetBatteryLevel(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Get the current voltage value of the battery of the specified device at the current moment, of type uint16\_t, with the value being the actual voltage value \* 100\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device types, such as upper limb exoskeleton \(UpBody\), or gloves \(LGlove/RGlove\), etc\.||

**Return value description**
Get the current voltage value of the battery of the specified device at the current moment, of type uint16\_t, with the value being the actual voltage value \* 100\.



#### **2\.2\.25 ****`DexCapSuit::GetMainBatteryState`**

**Statement**** ：**

```Plaintext
const MainBatteryState *GetMainBatteryState() const;
```

**Document：** `DexCap.hpp`

Obtain the status data of the main battery of the upper limb exoskeleton at the current moment\.

**Return value description**
A constant pointer to the MainBatteryState object that points to the internal cache of the SDK\. For a description of the internal data structure of MainBatteryState, please refer to 2\.1\.11\.



#### **2\.2\.26 ****`DexCapSuit::VibeMotors`**

**Statement**** ：**

```Plaintext
void VibeMotors(ExoApparatus hand, const std::vector<uint8_t> & vibeVals) const;
```

**Document：** `DexCap.hpp`

Send control commands to the DexCap glove to vibrate the fingertip vibration motors\. The vibration intensity of the vibration motors is given by the parameter vibeVals, where each element sequentially represents the vibration intensity value of the corresponding finger\. The number of elements in the vector vibeVals is at most 5; elements exceeding 5 will be ignored, and when the number is less than 5, the vibration intensity of the finger at the corresponding index will be set to 0\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device type: only DexCap gloves \(LGlove/RGlove\) support fingertip vibration\.||
|vibeVals|const std::vector\<uint8\_t\> \&|A vector representing the vibration intensity values of the fingertip vibration motors for each finger, starting from index 0, sequentially representing the thumb, index finger, middle finger, ring finger, and little finger\. When the number of elements is less than 5, the vibration intensity of the corresponding finger is forced to be 0; when the number of elements is greater than 5, the extra elements are ignored\.||
|||||

#### **2\.2\.27 ****`DexCapSuit::GetFirmwareVersion`**

**Statement**** ：**

```Plaintext
std::string GetFirmwareVersion(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Retrieve the firmware version number from the specified device\. The format of the firmware version number is typically divided into 4 segments by "\.", with the highest bit being the major version number \(product or design change\), the second segment being the minor version number \(feature updates, improvements, etc\.\), the third segment being the update number \(mainly minor feature improvements and bug fixes\), and the last segment being the build number\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device type, upper limb exoskeleton UpBody, or glove \(LGlove/RGlove\)||
|||||

**Return value description**

Device firmware version number



#### **2\.2\.28 ****`DexCapSuit::anyError`**

**Statement**** ：**

```Plaintext
bool anyError(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

Determine whether the specified device is currently experiencing an error\. Typically, after sending management commands such as Start\(\), Resume\(\), Close\(\), VibeMotors\(\), etc\., to all devices or some specified devices, when the interface returns a result of false or non\-DEX\_SUCCESS, or after an interface with a void return value has been called, if the user fails to obtain the expected behavior or data feedback during subsequent operations on the device, this interface can be used to determine whether the device is in an error state\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device type, upper limb exoskeleton UpBody, or glove \(LGlove/RGlove\)||

**Return value description**

If the device is in an error state, return true; if the system does not detect or record an error indicating the existence of the specified device, return false\.



#### **2\.2\.29 ****`DexCapSuit::getErrorCode`**

**Statement**** ：**

```Plaintext
ErrorCode getErrorCode() const;
ErrorCode getErrorCode(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

This function has two overloaded versions\.

Overload 1: Without parameters\. Retrieves the current error code of the DexCapSuit device instance\. If all devices are currently in normal state, i\.e\., there are no errors, it returns ERROR\_NONE\.

Overload 2: With an ExoApparatus parameter\. Retrieves the current error code of the specified device\. If the device is currently in normal state, it returns ERROR\_NONE\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device type, upper limb exoskeleton UpBody, or glove \(LGlove/RGlove\)||

**Return value description**

If the device currently has an error, return the error code; if the device status is normal, return ERROR\_NONE\.



#### **2\.2\.30 ****`DexCapSuit::getErrorMessage`**

**Statement**** ：**

```Plaintext
const std::string & getErrorMessage();
std::string getErrorMessage(ExoApparatus device) const;
```

**Document：** `DexCap.hpp`

This function has two overloaded versions\.

Overload 1: Without parameters\. Retrieves the specific text information of the current error of the DexCapSuit device instance; if all device states are currently normal, returns an empty string\.

Overload 2: With an ExoApparatus parameter\. Retrieves the specific text information of the current error of the specified device; if the current device state is normal, returns an empty string\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|device|ExoApparatus|Device type, upper limb exoskeleton UpBody, or glove \(LGlove/RGlove\)||

**Return value description**

If the device currently has an error, return the error code; if the device status is normal, return ERROR\_NONE\.



#### **2\.2\.31 ****`DexCapSuit::registerStatusDataProc`**

**Statement**** ：**

```Plaintext
void registerStatusDataProc(const DexCapStatusDataProc & callback);
```

**Document：** `DexCap.hpp`

Register a function of a statement in the form of DexCapStatusDataProc as a callback function for receiving and processing all joint raw real\-time data from the DexCapSuit device\. This function takes a pointer of type const DexCapJointData \* as a parameter\.

For detailed instructions on how users can implement this function, please refer to Section 2\.1\.16\.

**Parameter Description****：**

|Parameter name|Parameter type|meaning|Supplementary Note|
|---|---|---|---|
|callback|DexCapStatusDataProc|The user wishes to register a callback function for the DexCapSuit instance\.||
|||||

