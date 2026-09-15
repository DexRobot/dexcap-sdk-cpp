#include <thread>
#include <cstring>
#include <iostream>
#include <fstream>

#if defined(_MSC_VER) || defined(_WIN32)
#define strcasecmp _stricmp
#endif

#include "configuration.h"
#include "cpp/DexCap.hpp"
#include "example_defs.h"

using namespace DexRobot;

bool start_flag=true;


static const std::map<ExoApparatus, std::string> DEVICE_NAMES = {
    { LGlove, "左手套" },
    { RGlove, "右手套" },
    { UpBody, "外骨骼" },
};


char * CmdReadLine(void)
{
    int bufsize = 128;
    int position = 0;
    auto *buffer = (char *)malloc(sizeof(char) * bufsize);

    if (!buffer) {
        printf("CmdReadLine: allocation error\n");
        exit(1);
    }

    while (true) {
        const int c = getchar();
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        }

        buffer[position] = c;

        position++;
        if (position >= bufsize) {
            bufsize += 128;
            //use temporary pointer
            char *temp = (char *)realloc(buffer, bufsize);
            if (!temp) {
                printf("CmdReadLine: realloc error\n");
                free(buffer);
                exit(1);
            }
            buffer = temp; // only realloc success
        }
    }
}


/// A log file for recording sensor data stream
std::fstream logFile;

void SensorDataCallback(const SuitStatusData* data)
{
    if (data == nullptr)
        return;

    const SkeletonArmsData* armsData = &data->jointData;
    printf("[L Arm]: Joint1=%.2f, Joint2=%.2f, Joint3=%.2f, Joint4=%.2f, Joint5=%.2f, Joint6=%.2f, Joint7=%.2f\n",
           (double)armsData->LArm1 / 100,
           (double)armsData->LArm2 / 100,
           (double)armsData->LArm3 / 100,
           (double)armsData->LArm4 / 100,
           (double)armsData->LArm5 / 100,
           (double)armsData->LArm6 / 100,
           (double)armsData->LArm7 / 100);

    printf("[R Arm]: Joint1=%.2f, Joint2=%.2f, Joint3=%.2f, Joint4=%.2f, Joint5=%.2f, Joint6=%.2f, Joint7=%.2f\n",
           (double)armsData->RArm1 / 100,
           (double)armsData->RArm2 / 100,
           (double)armsData->RArm3 / 100,
           (double)armsData->RArm4 / 100,
           (double)armsData->RArm5 / 100,
           (double)armsData->RArm6 / 100,
           (double)armsData->RArm7 / 100);

    printf("[L JoyStick]: Button A clicked=%s, Button B clicked=%s, Axis X=%d, Axis Y=%d,"
           " Axis Z clicked=%s, Trigger A=%d, Trigger B=%d, Trigger A clicked=%s, Trigger B clicked=%s\n",
           armsData->LJoyS.ButtonA ? "Yes" : "No",
           armsData->LJoyS.ButtonB ? "Yes" : "No",
           armsData->LJoyS.RockerX,
           armsData->LJoyS.RockerY,
           armsData->LJoyS.RockerZ ? "Yes" : "No",
           armsData->LJoyS.TgrDistA,
           armsData->LJoyS.TgrDistB,
           armsData->LJoyS.TriggerA ? "Yes" : "No",
           armsData->LJoyS.TriggerB ? "Yes" : "No"
    );

    printf("[R JoyStick]: Button A clicked=%s, Button B clicked=%s, Axis X=%d, Axis Y=%d,"
           " Axis Z clicked=%s, Trigger A=%d, Trigger B=%d, Trigger A clicked=%s, Trigger B clicked=%s\n",
           armsData->RJoyS.ButtonA ? "Yes" : "No",
           armsData->RJoyS.ButtonB ? "Yes" : "No",
           armsData->RJoyS.RockerX,
           armsData->RJoyS.RockerY,
           armsData->RJoyS.RockerZ ? "Yes" : "No",
           armsData->RJoyS.TgrDistA,
           armsData->RJoyS.TgrDistB,
           armsData->RJoyS.TriggerA ? "Yes" : "No",
           armsData->RJoyS.TriggerB ? "Yes" : "No"
    );

    printf("===============================================\n\n");
}


void RunDexCapExample(int timespan=30 /* Run for secdons */)
{
    const auto device_list = alloc_serial_port_device_list();
    size_t device_count = 0;
    enumerate_serial_port_devices(ProductVersion::V4, device_list, &device_count);

    DexCapSuit dexCapSuit(WIREDUSB);
    dexCapSuit.registerStatusDataProc(SensorDataCallback);

    for (int i = 0; i < device_count; ++i)
    {
        const auto deviceType = dexCapSuit.ConnectDevice(device_list[i].serial_port_name);

        if (deviceType == ExoApparatus::UnDefn)
        {
            std::cout << "Device connection failed: " << device_list[i].serial_port_name << std::endl;
            continue;
        }

        const auto it = DEVICE_NAMES.find(deviceType);
        if (it == DEVICE_NAMES.end())
            continue;

        auto deviceId = dexCapSuit.GetDeviceID(deviceType);
        std::cout << "Device device ID: " << static_cast<int>(deviceId)
        << ". Device Name: " << it->second << std::endl;

        auto connState = dexCapSuit.IsConnected(deviceType);
        std::cout << "Device connection status: " << (connState ? "Connected" : "Disconnected")
            << " on " << device_list[i].serial_port_name << std::endl;

        auto fmwVersion = dexCapSuit.GetFirmwareVersion(deviceType);
        std::cout << "Device " << DEVICE_NAMES.at(deviceType) << " firmware version: " << fmwVersion << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    const auto startTs = current_timestamp();
    const std::string logFileName = "./sensor_data_stream-" + timestamp_to_datetime_string(startTs) + ".log";
    logFile.open(logFileName, std::ios_base::out | std::ios_base::app);
    std::cout << "Output log file: " << logFileName << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto retCode = dexCapSuit.Start();
    if (retCode != DEX_SUCCESS)
    {
        std::cout << "Failed to start sampling" << std::endl;
        return;
    }

    bool timeout = false;
    do {
        const auto batteryState = dexCapSuit.GetMainBatteryStatus();
        if (batteryState != nullptr)
        {
            std::cout << "[Main Battery State]: CURR=" << batteryState->Currency << "mA"
                << ", VOLT=" << (float)batteryState->Voltage / 1000 << "V"
                << ", PWRM=" << batteryState->RemainPower << "%"
                << ", TEMP=" << (float)batteryState->Temperature / 10 << "℃"
                << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        const auto currentTs = current_timestamp();
        const auto duration = currentTs - startTs;
        timeout = duration >= timespan * 1000;
    } while (!timeout);

    dexCapSuit.Close();
    logFile.close();
}


void WifiBasedConnectionExample(const std::string & address, uint64_t timespan)
{
    DexCapSuit dexCapSuit(WLAN_TCP);
    dexCapSuit.registerStatusDataProc(SensorDataCallback);

    const auto deviceType = dexCapSuit.ConnectDevice(address);

    if (deviceType != LGlove && deviceType != RGlove && deviceType != UpBody && deviceType != JSBody)
    {
        std::cerr << address << " is not a DexCap device!" << std::endl;
        return;
    }

    const auto startTs = current_timestamp();
    const std::string logFileName = "./sensor_data_stream-" + timestamp_to_datetime_string(startTs) + ".log";
    logFile.open(logFileName, std::ios_base::out | std::ios_base::app);
    std::cout << "Output log file: " << logFileName << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    const auto retCode = dexCapSuit.Start();
    if (retCode != DEX_SUCCESS)
    {
        std::cout << "Failed to start sampling" << std::endl;
        return;
    }

    bool timeout = false;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        const auto currentTs = current_timestamp();
        const auto duration = currentTs - startTs;
        timeout = duration >= timespan * 1000;
    } while (!timeout);

    dexCapSuit.Close();
    logFile.close();
}


int main(int argc, const char ** argv)
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    printf("退出请输入quit\n");
    do {
        printf("DexCap> ");
        auto cmd = CmdReadLine();
        if(strlen(cmd) == 0)
        {
            free(cmd);
            continue;
        }

        if(strcasecmp(cmd, "quit") == 0 || strcasecmp(cmd, "exit") == 0)
        {
            free(cmd);
            break;
        }

        std::string strCmd = cmd;
        if (strCmd.starts_with("run"))
        {
            int seconds = 30;
            std::string bleDevName;
            std::vector<std::string> args;
            split(args, strCmd, " ");
            if (args.size() == 4)
            {
                seconds = std::stoi(args[3]);

                if(args[1] == "-w")
                {
                    const auto ipAddr = std::string(args[2]);
                    WifiBasedConnectionExample(ipAddr, seconds);
                }
            }
            else if(args.size() == 3)
            {
                if(args[1] == "-w")
                {
                    const auto ipAddr = std::string(args[2]);
                    WifiBasedConnectionExample(ipAddr, seconds);
                }
                else if(args[1] == "-u")
                {
                    seconds = std::stoi(args[2]);
                    RunDexCapExample(seconds);
                }
            }
            else if(args.size() < 2)
            {
                RunDexCapExample(seconds);
            }
            else
            {
                std::cout << "[Usage]: To run data sampling demo on DexCap devices, please use command below:" << std::endl;
                std::cout << "[Demo]: run [-w <ip address> | -u] [seconds]" << std::endl;
                std::cout << "        -w for wifi connection, -u for USB connection" << std::endl;
            }
        }
        else
        {
            std::cout << "[Usage]: To run data sampling demo on DexCap devices, please use command below:" << std::endl;
            std::cout << "[Demo]: run [-w <ip address> | -u] [seconds]" << std::endl;
            std::cout << "        -w for wifi connection, -u for USB connection" << std::endl;
        }

        free(cmd);
    } while(true);

    return 0;
}
