#include <stdio.h>
#include <stdint.h>
#include "dexcap.h"
#include "example_defs.h"

#ifndef BOOL
#define BOOL int
#else
#undef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#else
#undef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#else
#undef FALSE
#define FALSE 0
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
            data->InetMU[0], data->InetMU[1], data->InetMU[2],
            data->InetMU[3], data->InetMU[4], data->InetMU[5], data->InetMU[6],
            data->InetMU[7], data->InetMU[8], data->InetMU[9],
            data->InetMU[10], data->InetMU[11], data->InetMU[12],
            data->InetMU[13], data->InetMU[14], data->InetMU[15],
            data->InetMU[16], data->InetMU[17]);
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

    DEXCAP_DEVICE_TYPE deviceType;
    const char * devicePath = "/dev/ttyUSB1";
    retCode = dexcap_connect_suit_device(hSuit, devicePath, &deviceType, WIREDUSB);
    if (retCode == DEX_INVALID_DEVICE || deviceType == UnDefn)
    {
        fprintf(stderr, "Device at given path %s is not a recognized device type of DexCap\n", devicePath);
        return FALSE;
    }

    retCode = register_joint_data_callback(hSuit, JointDataCallback);
    if (retCode != DEX_SUCCESS)
    {
        return FALSE;
    }

    retCode = dexcap_start_device_sampling(hSuit, deviceType);
    if (retCode != DEX_SUCCESS)
    {
        fprintf(stderr, "Failed to start sampling on device %s\n", devicePath);
        return FALSE;
    }

    while (TRUE)
    {
        /// Do something or just leave it this way to keep process running.
    }

    return TRUE;
}