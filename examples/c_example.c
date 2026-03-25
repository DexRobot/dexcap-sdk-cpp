#include <stdio.h>
#include <stdint.h>
#include "dexcap.h"

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

int FingersErrorMask[] = {
    0x80000000, // Thumb DIP Error Bit
    0x40000000, // Thumb PIP Error Bit
    0x20000000, // Thumb MCP Error Bit
    0x10000000, // Thumb SWP Error Bit
    0x8000000,  // Thumb ROP Error Bit
    0x4000000,  // Index DIP Error Bit
    0x2000000,  // Index PIP Error Bit
    0x1000000,  // Index MCP Error Bit
    0x800000,   // Index SWP Error Bit
    0x400000,   // Middle DIP Error Bit
    0x200000,   // Middle PIP Error Bit
    0x100000,   // Middle MCP Error Bit
    0x80000,    // Middle SWP Error Bit
    0x40000,    // Ring DIP Error Bit
    0x20000,    // Ring PIP Error Bit
    0x10000,    // Ring MCP Error Bit
    0x8000,     // Ring SWP Error Bit
    0x4000,     // Little DIP Error Bit
    0x2000,     // Little PIP Error Bit
    0x1000,     // Little MCP Error Bit
    0x800,      // Little SWP Error Bit
    0x80,       // Thumb Vide Motor Error Bit
    0x40,       // Index Vide Motor Error Bit
    0x20,       // Middle Vide Motor Error Bit
    0x10,       // Ring Vide Motor Error Bit
    0x8,        // Little Vide Motor Error Bit
};

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

int main(int argc, const char ** argv)
{
    CInterfaceTest();

    return 0;
}

