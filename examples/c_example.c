#include <stdio.h>
#include <stdint.h>
#include "dexcap.h"
#include "example_defs.h"

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

void JointDataCallback(const DexCapJointData * data)
{
    if (data == NULL)
        return;

    if (data->mask & 0x8000)
    {
        printf("[Left Glove]:");
        size_t totalCnt = sizeof(data->LGlove)/sizeof(uint16_t);
        size_t jointCnt = totalCnt - 3;
        uint16_t batterState = data->LGlove[totalCnt-3];
        uint32_t errorMask   = data->LGlove[totalCnt-3] << 8 | data->LGlove[totalCnt-1];
        for (int i=0; i < jointCnt; ++i)
        {
            if(errorMask & FingersErrorMask[i])
            {
                printf("Joint%d: ERR, ", i+1);
            }
            else
            {
                printf("Joint%d: %.2f, ", i+1, (double)data->LGlove[i]/100);
            }
        }
        printf("\n\n");
    }

    if (data->mask & 0x4000)
    {
        printf("[Exo Body]:");
        size_t totalCnt = sizeof(data->ExBody)/sizeof(uint16_t);
        size_t jointCnt = totalCnt - 1;
        uint16_t batterState = data->ExBody[totalCnt-1];
        for (int i=0; i < jointCnt; ++i)
        {
            printf("Joint%d: %.2f, ", i+1, (double)data->ExBody[i]/100);
        }
        printf("\n\n");
    }

    if (data->mask & 0x2000)
    {
        printf("[Right Glove]:");
        size_t totalCnt = sizeof(data->RGlove)/sizeof(uint16_t);
        size_t jointCnt = totalCnt - 3;
        uint16_t batterState = data->RGlove[totalCnt-3];
        uint32_t errorMask   = data->RGlove[totalCnt-2] | (data->RGlove[totalCnt-1] << 8);
        for (int i=0; i < jointCnt; ++i)
        {
            if(errorMask & FingersErrorMask[i])
            {
                printf("Joint%d: ERR, ", i+1);
            }
            else
            {
                printf("Joint%d: %.2f, ", i+1, (double)data->RGlove[i]/100);
            }
        }
        printf("\n");
    }

    if (data->mask & 0x1000)
    {
        /// TODO: Not implemented yet
        printf("[IMU]:");
        printf("Euler Angles:[%.3f,%.3f,%.3f],Quaternion[%.3f,%.3f,%.3f,%.3f],Accel:[%.3f,%.3f,%.3f],Gyroscope:[%.3f, %.3f, %.3f],Magnet:[%.3f,%.3f,%.3f],air_pressure:%.3f,Temperature:%.2f\n",
            data->InetMU.poseData[0], data->InetMU.poseData[1], data->InetMU.poseData[2],
            data->InetMU.poseData[3], data->InetMU.poseData[4], data->InetMU.poseData[5], data->InetMU.poseData[6],
            data->InetMU.poseData[7], data->InetMU.poseData[8], data->InetMU.poseData[9],
            data->InetMU.poseData[10], data->InetMU.poseData[11], data->InetMU.poseData[12],
            data->InetMU.poseData[13], data->InetMU.poseData[14], data->InetMU.poseData[15],
            data->InetMU.poseData[16], data->InetMU.temp);
        printf("\n");
    }
}

BOOL CInterfaceTest()
{
    DEXCAP_SUIT_HANDLE hSuit;
    DEX_RETURN retCode = dexcap_create_suit_instance(&hSuit);
    if (retCode != DEX_SUCCESS || hSuit == NULL)
    {
        fprintf(stderr, "Failed to create DexCap Suit instance");
        return FALSE;
    }

    DEXCAP_DEVICE_TYPE deviceType0;
    DEXCAP_DEVICE_TYPE deviceType1;
    const char * devicePath0 = "/dev/ttyUSB0"; // Assume this is one of my glove
    const char * devicePath1 = "/dev/ttyACM1"; // Assume this is the exo-skeleton body frame
    retCode = dexcap_connect_suit_device(hSuit, devicePath0, &deviceType0, WIREDUSB);
    if (retCode == DEX_INVALID_DEVICE || deviceType0 == UnDefn)
    {
        fprintf(stderr, "Device at given path %s is not a recognized device type of DexCap\n", devicePath0);
        return FALSE;
    }

    retCode = dexcap_connect_suit_device(hSuit, devicePath1, &deviceType1, WIREDUSB);
    if (retCode == DEX_INVALID_DEVICE || deviceType1 == UnDefn)
    {
        fprintf(stderr, "Device at given path %s is not a recognized device type of DexCap\n", devicePath1);
        return FALSE;
    }

    retCode = register_joint_data_callback(hSuit, JointDataCallback);
    if (retCode != DEX_SUCCESS)
    {
        return FALSE;
    }

    retCode = dexcap_start_suit_sampling(hSuit);
    if (retCode == DEX_ERROR)
    {
        fprintf(stderr, "Failed to start sampling for DexCap Suit\n");
        return FALSE;
    }

    if (retCode == DEX_SUCCESS_WITH_INFO)
    {
        fprintf(stderr, "Failed to start sampling on one of the devices\n");
    }

    int count = 0;
    MainBatteryState mainBatteryState;
    do {
        retCode = dexcap_get_main_battery_state(hSuit, &mainBatteryState);
        if (retCode == DEX_SUCCESS)
        {
            printf("[Main Battery State]: CURR=%d, VOLT=%f, PWRM=%d, TEMP=%f\n",
                mainBatteryState.Currency,
                (float)mainBatteryState.Voltage / 1000,
                mainBatteryState.RemainPower,
                (float)mainBatteryState.Temperature / 10);
        }

        if (dexcap_is_device_connected(hSuit, LGlove))
        {
            uint16_t batVoltage = 0;
            retCode = dexcap_get_l_battery_state(hSuit, &batVoltage);
            if (retCode == DEX_SUCCESS && batVoltage < 3500)
            {
                printf("LGlove is running out of battery, please charge via main battery on ExoSkeleton."
                    " The operatio needs to be done on DexCapGUI application.\n");
            }
        }

        if (dexcap_is_device_connected(hSuit, RGlove))
        {
            uint16_t batVoltage = 0;
            retCode = dexcap_get_l_battery_state(hSuit, &batVoltage);
            if (retCode == DEX_SUCCESS && batVoltage < 3500)
            {
                printf("RGlove is running out of battery, please charge via main battery on ExoSkeleton."
                    " The operatio needs to be done on DexCapGUI application.\n");
            }
        }

#ifndef WIN32
        sleep(5);
#else
        Sleep(5000);
#endif
        count++;
    } while (count < 12);

    return TRUE;
}

int main(int argc, const char **argv)
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    CInterfaceTest();
}
