//
// Created by ryzuo on 25-8-20.
//
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
class BluetoothDriver;
class DexCapDataHandler;

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

    std::string GetAdapterName(ExoApparatus device) const;

    bool InitNetwork(const std::string & serverAddr, int serverPort, NetProtocolType netType);

    ProductVersion & productVersion();
    const ProductVersion & productVersion() const;
    uint8_t GetDeviceID(ExoApparatus device) const;

    std::string getSerialNumber(ExoApparatus device);

    DEX_RETURN Start();
    bool Start(ExoApparatus device) const;

    bool IsConnected(ExoApparatus device);

    bool IsRunning() const;
    bool IsRunning(ExoApparatus device) const;

    DEX_RETURN Pause();
    bool Pause(ExoApparatus device) const;

    DEX_RETURN Resume();
    bool Resume(ExoApparatus device) const;

    DEX_RETURN Close();
    DEX_RETURN Close(ExoApparatus device);

    AdapterType GetAdapterType() const;

    ExoApparatus GetDeviceType(const std::string & adapterName) const;

    const SuitJointState & GetSuitJointState() const;
    const SkeletonJointAngles & GetBodyJointState() const;
    const GloveJointAngles & GetLeftGloveJointState() const;
    const GloveJointAngles & GetRightGloveJointState() const;
    const InertialUnitData & GetInertialMUJointState() const;//sxl add.

    uint16_t GetBatteryLevel(ExoApparatus device) const;
    const MainBatteryState *GetMainBatteryState() const;

    void VibeMotors(ExoApparatus hand, const std::vector<uint8_t> &) const;
    std::string GetFirmwareVersion(ExoApparatus device) const;

    bool anyError(ExoApparatus device) const;
    ErrorCode getErrorCode(ExoApparatus device) const;
    std::string getErrorMessage(ExoApparatus device) const;

    ErrorCode getErrorCode() const { return this->errCode; }
    const std::string & getErrorMessage();

    void registerStatusDataProc(const DexCapStatusDataProc & callback);

private:
    std::pair<bool, const std::string> IsDeviceExists(ExoApparatus device) const;
    bool IsDeviceExists(const std::string & adapterName) const;

    void RemoveDevice(ExoApparatus device);
    void RemoveDevice(const std::string & adapterName);

protected:
    ProductVersion prodVersion;
    BluetoothDriver * bluetoothDriver;
    ErrorCode errCode;
    std::string errMsg;
    std::string configureFile;
    std::map<std::string, std::shared_ptr<DexCapDevice>> adapterDeviceMap;

private:
    std::shared_ptr<DexGlove> lHand;
    std::shared_ptr<DexGlove> rHand;
    std::shared_ptr<DexoBody> xBody;
    std::shared_ptr<DexIMUnit> imUnit;
    std::shared_ptr<Socket> network;

    std::map<ExoApparatus, ErrorCode> lastError;

    DexCapStatusDataProc statusDataProc;
    DexCapDataHandler * dataHandler;

    friend class DexCapSuitAdmin;
    friend class DexCapDataHandler;
};

}
