#include <iostream>

#if defined(_MSC_VER) || defined(_WIN32)
#define strcasecmp _stricmp
#endif

#include <cstring>
#include <thread>
#include "configuration.h"
#include "exoskeleton.h"

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

int main(int argc, const char ** argv)
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);  
#endif // WIN32

    const auto config = DexRobot::DexCapConfig::Load("./config.yaml");

    if(config == nullptr)
    {
        std::cout << "Configuration file open failed" << std::endl;
    }

    auto networkType = config->GetServerProtoType();
    auto serverAddrs = config->GetServerAddress();
    auto serverPort = config->GetServerPortNum();
    auto networkOn = config->IsNetworkEnabled();

    auto lAdapterName = config->GetLeftHandSPortName();
    auto lDeviceId = config->GetLeftHandDeviceId();
    auto lAdapterType = config->GetLeftHandAdapterType();
    auto lHandEnabled = config->IsLeftHandEnabled();

    auto rAdapterName = config->GetRightHandSPortName();
    auto rDeviceId = config->GetRightHandDeviceId();
    auto rAdapterType = config->GetRightHandAdapterType();
    auto rHandEnabled = config->IsRightHandEnabled();

    auto bAdapterName = config->GetBodySPortName();
    auto bDeviceId = config->GetBodyDeviceId();
    auto bAdapterType = config->GetBodyAdapterType();
    auto bodyEnabled = config->IsBodyEnabled();

    auto lDebug = config->IsDebugOnLeftHand();
    auto rDebug = config->IsDebugOnRightHand();
    auto bDebug = config->IsDebugOnBody();

    DexRobot::ExoSkeleton dexcap(networkType);

    if(networkOn)
    {
        bool connected = true;
        do {
            connected = dexcap.InitNetwork(serverAddrs, serverPort);
            if(!connected)
            {
                printf("网络连接失败, 是否重试? Y/n\n");
                printf("DexCap> ");
                const auto cmd = CmdReadLine();
                if(strlen(cmd) == 0)
                {
                    free(cmd);
                    continue;
                }

                std::string action(cmd);
                free(cmd);
                if(action != "Y" && action != "y")
                {
                    break;
                }
            }
            else
            {
                connected = true;
                printf("网络服务器连接成功\n");
            }
        } while(!connected);
    }

    if(bodyEnabled)
    {
        if(!dexcap.InitBody(bAdapterType, bAdapterName.c_str(), bDeviceId))
        {
            printf("背部外骨骼连接失败\n");
        }
        else
        {
            printf("背部外骨骼连接成功\n");
            if(bDebug)
                dexcap.SetBodyLogLevel(DexRobot::LOG_LEVEL::DX_DEBUG);
        }
    }

    if(lHandEnabled)
    {
        if(!dexcap.InitLeftHand(lAdapterType, lAdapterName.c_str(), lDeviceId))
        {
            printf("左手连接失败\n");
        }
        else
        {
            printf("左手连接成功\n");
            if(lDebug)
                dexcap.SetLeftHandLogLevel(DexRobot::LOG_LEVEL::DX_DEBUG);
        }
    }

    if(rHandEnabled)
    {
        if(!dexcap.InitRightHand(rAdapterType, rAdapterName.c_str(), rDeviceId))
        {
            printf("右手连接失败\n");
        }
        else
        {
            printf("右手连接成功\n");
            if(rDebug)
                dexcap.SetRightHandLogLevel(DexRobot::LOG_LEVEL::DX_DEBUG);
        }
    }
    // std::this_thread::sleep_for(std::chrono::seconds(2));

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

        if(strcasecmp(cmd, "mark") == 0)
        {
            if(lHandEnabled)
                dexcap.CalibrateLeftHandInitPosition();

            if(rHandEnabled)
                dexcap.CalibrateRightHandInitPosition();
        }

        if(strcasecmp(cmd, "vibe on") == 0)
        {
            if(lHandEnabled)
                dexcap.VibeMotors(DexRobot::ExoApparatus::LGlove, {50, 50, 50, 50, 50});

            if(rHandEnabled)
                dexcap.VibeMotors(DexRobot::ExoApparatus::RGlove, {50, 50, 50, 50, 50});
        }

        if(strcasecmp(cmd, "vibe off") == 0)
        {
            if(lHandEnabled)
                dexcap.VibeMotors(DexRobot::ExoApparatus::LGlove, {0, 0, 0, 0, 0});

            if(rHandEnabled)
                dexcap.VibeMotors(DexRobot::ExoApparatus::RGlove, {0, 0, 0, 0, 0});
        }

        free(cmd);
    } while(true);

    // if(strcasecmp(argv[1], "-c") == 0)
    //     RunUsbCable();
    //
    // if(strcasecmp(argv[1], "-w") == 0)
    //     RunWireless();

    return 0;
}
