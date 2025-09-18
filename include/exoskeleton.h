//
// Created by ryzuo on 25-8-20.
//
#pragma once

#include <future>
#include "typedef.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <libserial/SerialPort.h>
using namespace LibSerial;
#endif

namespace DexRobot
{

class Glove;
class Arm;
class Socket;

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


class ExoSkeleton final
{
public:
    ExoSkeleton() = delete;
#ifdef WIN32
    explicit ExoSkeleton(NetProtocolType, DWORD=921600);
#else
    explicit ExoSkeleton(NetProtocolType, BaudRate=BaudRate::BAUD_921600);
#endif
    ~ExoSkeleton();

    bool InitNetwork(const std::string & srvrAddr, int srvrPort);

    bool InitBody(AdapterType, const char * portName, uint8_t deviceId, uint32_t timeout=3000);
    bool InitLeftHand(AdapterType, const char * portName, uint8_t deviceId, uint32_t timeout=3000);
    bool InitRightHand(AdapterType, const char * portName, uint8_t deviceId, uint32_t timeout=3000);

    bool InitDevice(ExoApparatus, AdapterType, const char * portName, uint8_t deviceId, uint32_t timeout=300);

    bool PowerOnDevice(ExoApparatus, bool enable= true);

    uint8_t GetDeviceType(ExoApparatus) const;
    uint8_t GetLeftHandID() const;
    uint8_t GetRightHandID() const;
    uint8_t GetBodyDeviceID() const;

    void UpdateDeviceID(ExoApparatus apptus, uint8_t newDeviceId);

    void SetLeftHandLogLevel(LOG_LEVEL);
    void SetRightHandLogLevel(LOG_LEVEL);
    void SetBodyLogLevel(LOG_LEVEL);

    DEX_RETURN SetAutoFeedback(uint16_t interval, bool enable=true);
    void SetAutoFeedback(ExoApparatus apptus, uint16_t interval, bool enable=true);

    void Start(bool debug=false);

    bool IsConnected(ExoApparatus apparatus);
    bool IsStarted(ExoApparatus apparatus);

    void Pause();
    void Pause(ExoApparatus);

    void Resume();
    void Resume(ExoApparatus);

    void Close();
    void Close(ExoApparatus);

    bool ReOpen(ExoApparatus);

    bool CalibrateBodyInitPosition();
    bool CalibrateLeftHandInitPosition();
    bool CalibrateRightHandInitPosition();
    DEX_RETURN CalibrateInitialPosition();

    void Reset(ExoApparatus);

    void ReadCurrentState(ExoApparatus);
    uint16_t GetBatteryLevel(ExoApparatus) const;

    void VibeMotors(ExoApparatus, const std::vector<uint8_t> &);

    void registerDataCallback(const DataCallback & callback);

#ifdef WIN32
    void SwitchBaudRate(DWORD);
    void SwitchBaudRate(ExoApparatus, DWORD);
#else
    void SwitchBaudRate(BaudRate);
    void SwitchBaudRate(ExoApparatus, BaudRate);
#endif // WIN32

    ErrorCode getErrorCode() const { return this->errCode; }
    const std::string & getErrorMessage();

    bool WLSetChannel(ExoApparatus, uint8_t channel);
    bool WLSetTxAddress(ExoApparatus, uint16_t txAddr);
    bool WLSetRxAddress(ExoApparatus, uint16_t rxAddr);
    bool WLSetWorkPower(ExoApparatus, uint8_t powerLevel);

    uint8_t  WLGetChannel(ExoApparatus);
    uint8_t  WLGetWorkPower(ExoApparatus);
    uint16_t WLGetTxAddress(ExoApparatus);
    uint16_t WLGetRxAddress(ExoApparatus);

private:
    void recvFunc();

private:
    NetProtocolType protocolType;
#ifdef WIN32
	DWORD baudRate;
#else
    BaudRate baudRate;
#endif
    std::shared_ptr<Socket> network;
    std::shared_ptr<Glove> lHand;
    std::shared_ptr<Glove> rHand;
    std::shared_ptr<Arm>   body;

    std::promise<void> exitSignal;
    std::future<void> futureCtl;
    std::shared_ptr<std::thread> readThread;

    ErrorCode errCode;
    std::string errMsg;

    DataCallback dataCallback;
};

}
