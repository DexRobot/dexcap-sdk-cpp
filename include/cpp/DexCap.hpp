#pragma once

#include <map>
#include <future>

#ifdef WIN32
#include <WinSock2.h>
#include <Windows.h>
#else
#include <libserial/SerialPort.h>
using namespace LibSerial;
#endif

#include "TypeDef.hpp"
#include "Utils.hpp"

namespace DexRobot
{

class Socket;
class DexCapDevice;
class DexGlove;
class DexoBody;
class DexIMUnit;
class DeviceStatuData;
class BluetoothDriver;
class DexCapDataHandler;
class ForwardKinematics;

typedef DEXCAP_DEVICE_TYPE ExoApparatus;

using DexCapStatusDataProc = std::function<void (const DexCapJointData *)>;

class WLReceiverHelper final
{
public:
    WLReceiverHelper() = delete;
    ~WLReceiverHelper() = delete;

    static bool SetChannel(const char * serPort, uint8_t channel);
    static bool SetTxAddress(const char * serPort, uint16_t txAddr);
    static bool SetRxAddress(const char * serPort, uint16_t rxAddr);
    static bool SetWorkPower(const char * serPort, uint8_t powerLevel);

    static uint8_t  GetChannel(const char * serPort);
    static uint16_t GetTxAddress(const char * serPort);
    static uint16_t GetRxAddress(const char * serPort);
    static WLPowerLevel GetWorkPower(const char * serPort);
};

class DexCapSuit
{
public:
    DexCapSuit() = delete;
    explicit DexCapSuit(ProductVersion version, const std::string & configFile = "./config.yaml");
    virtual ~DexCapSuit();

    virtual ExoApparatus ConnectDevice(const std::string & adapterName, AdapterType adapterType);
    virtual bool DisconnectDevice(const std::string & adapterName);
    virtual bool DisconnectDevice(ExoApparatus device);

    [[nodiscard]] std::string GetAdapterName(ExoApparatus device) const;

    bool InitNetwork(const std::string & serverAddr, int serverPort, NetProtocolType netType);
    bool CloseNetwork();

    ProductVersion & productVersion();
    [[nodiscard]] const ProductVersion & productVersion() const;
    [[nodiscard]] uint8_t GetDeviceID(ExoApparatus device) const;

    std::string getSerialNumber(ExoApparatus device);

    DEX_RETURN Start();
    [[nodiscard]] bool Start(ExoApparatus device) const;

    [[nodiscard]] bool IsConnected(ExoApparatus device);
    [[nodiscard]] bool IsSensorEnabled(ExoApparatus device) const;
    [[nodiscard]] bool IsBluetoothConnected(ExoApparatus device) const;
    [[nodiscard]] bool IsChargeNeeded(ExoApparatus device) const;

    [[nodiscard]] bool IsRunning() const;
    [[nodiscard]] bool IsRunning(ExoApparatus device) const;

    DEX_RETURN Pause();
    [[nodiscard]] bool Pause(ExoApparatus device) const;

    DEX_RETURN Resume();
    [[nodiscard]] bool Resume(ExoApparatus device) const;

    DEX_RETURN Close();
    DEX_RETURN Close(ExoApparatus device);

    [[nodiscard]] AdapterType GetAdapterType() const;

    [[nodiscard]] ExoApparatus GetDeviceType(const std::string & adapterName) const;

    [[nodiscard]] const SuitJointState & GetSuitJointState() const;
    [[nodiscard]] const SkeletonJointAngles & GetBodyJointState() const;
    [[nodiscard]] const GloveJointAngles & GetLeftGloveJointState() const;
    [[nodiscard]] const GloveJointAngles & GetRightGloveJointState() const;
    [[nodiscard]] const InertialUnitData & GetIMUSensorData() const;

    [[nodiscard]] const DexCapEndPoses & GetEndPose() const;

    [[nodiscard]] uint16_t GetBatteryLevel(ExoApparatus device) const;
    [[nodiscard]] const MainBatteryState *GetMainBatteryState() const;

    void VibeMotors(ExoApparatus hand, const std::vector<uint8_t> &) const;
    [[nodiscard]] std::string GetFirmwareVersion(ExoApparatus device) const;

    [[nodiscard]] bool anyError(ExoApparatus device) const;
    [[nodiscard]] ErrorCode getErrorCode(ExoApparatus device) const;
    [[nodiscard]] std::string getErrorMessage(ExoApparatus device) const;

    [[nodiscard]] ErrorCode getErrorCode() const;
    const std::string & getErrorMessage();

    void registerStatusDataProc(const DexCapStatusDataProc & callback);

    [[nodiscard]] size_t RegisterImuDataCallback(const std::function<void(uint8_t, const std::shared_ptr<DeviceStatuData>&)>& callback) const;
    void UnregisterImuDataCallback(size_t handle) const;

private:
    class DexCapSuitImpl *impl;
};

}
