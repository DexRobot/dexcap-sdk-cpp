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

void SensorDataCallback(const DexCapJointData * data)
{
    if (data == nullptr)
        return;

    const auto & datetime = timestamp_to_datetime_string(data->timestamp);

    if (data->mask & 0x8000)
    {
        std::string row = "[Left Glove]-[" + datetime + "]: ";
        size_t totalCnt = sizeof(data->LGlove)/sizeof(uint16_t);
        size_t jointCnt = totalCnt - 3;
        uint32_t errorMask = (data->LGlove[totalCnt-2] << 16) | data->LGlove[totalCnt-1];
        for (int i=0; i < jointCnt; ++i)
        {
            if(errorMask & FingersErrorMask[i])
            {
                row += "Joint" + std::to_string(i+1) + "=ERR, ";
            }
            else
            {
                row += "Joint" + std::to_string(i+1) + "=" + std::to_string((double)data->LGlove[i]/100) + ", ";
            }
        }

        logFile << row << std::endl;
    }

    if (data->mask & 0x4000)
    {
        std::string row = "[Exo UpBody]-[" + datetime + "]: ";
        size_t totalCnt = sizeof(data->ExBody)/sizeof(uint16_t);
        size_t jointCnt = totalCnt - 1;
        uint16_t batterState = data->ExBody[totalCnt-1];
        for (int i=0; i < jointCnt; ++i)
        {
            printf("Joint%d: %.2f, ", i+1, (double)data->ExBody[i]/100);
            row += "Joint" + std::to_string(i+1) + "=" + std::to_string((double)data->ExBody[i]/100) + ", ";
        }
        logFile << row << std::endl;
    }

    if (data->mask & 0x2000)
    {
        std::string row = "[Right Glove]-[" + datetime + "]: ";
        size_t totalCnt = sizeof(data->RGlove)/sizeof(uint16_t);
        size_t jointCnt = totalCnt - 3;
        uint32_t errorMask = (data->LGlove[totalCnt-2] << 8) | data->LGlove[totalCnt-1];
        for (int i=0; i < jointCnt; ++i)
        {
            if(errorMask & FingersErrorMask[i])
            {
                row += "Joint" + std::to_string(i+1) + "=ERR, ";
            }
            else
            {
                row += "Joint" + std::to_string(i+1) + "=" + std::to_string((double)data->RGlove[i]/100) + ", ";
            }
        }

        logFile << row << std::endl;
    }

    logFile << std::endl;
}

void RunDexCapExample(int seconds=30)
{
    const auto device_list = alloc_serial_port_device_list();
    size_t device_count = 0;
    enumerate_serial_port_devices(ProductVersion::V4, device_list, &device_count);
    DexCapSuit dexCapSuit(ProductVersion::V4);
    dexCapSuit.registerStatusDataProc(SensorDataCallback);

    for (int i=0; i < device_count; ++i)
    {
        const auto deviceType = dexCapSuit.ConnectDevice(device_list[i].serial_port_name, AdapterType::WIREDUSB);

        if (deviceType == ExoApparatus::UnDefn)
        {
            std::cout << "Device connection failed: " << device_list[i].serial_port_name << std::endl;
        }

        auto deviceId = dexCapSuit.GetDeviceID(deviceType);
        std::cout << "Device device ID: " << static_cast<int>(deviceId) << std::endl;

        auto connState = dexCapSuit.IsConnected(deviceType);
        std::cout << "Device connection status: " << (connState ? "Connected" : "Disconnected") << std::endl;

        auto fmwVersion = dexCapSuit.GetFirmwareVersion(deviceType);
        std::cout << "Device firmware version: " << fmwVersion << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    const auto startTs = current_timestamp();
    const std::string logFileName = "./sensor_data_stream-" + timestamp_to_datetime_string(startTs) + ".log";
    logFile.open(logFileName, std::ios_base::out | std::ios_base::app);
    std::cout << "Output log file: " << logFileName << std::endl;

    dexCapSuit.Start();

    bool timeout = false;
    do {
        /// Run for 1 minute
        timeout = current_timestamp() - startTs >= seconds*1000;
    } while (!timeout);

    dexCapSuit.Close();
    logFile.close();
}

int main(int argc, const char ** argv)
{
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
            std::vector<std::string> args;
            split(args, strCmd, " ");
            if (args.size() >= 2)
                seconds = std::stoi(args[1]);

            RunDexCapExample(seconds);
        }
        else
        {
            std::cout << "[Usage]: To run data sampling demo on DexCap devices, please use command below:" << std::endl;
            std::cout << "[Demo]: run [seconds]" << std::endl;
        }

        free(cmd);
    } while(true);

    return 0;
}
