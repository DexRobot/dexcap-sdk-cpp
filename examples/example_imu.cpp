#include <thread>
#include <cstring>
#include <iostream>
#include <atomic>
#include <mutex>

#if defined(_MSC_VER) || defined(_WIN32)
#define strcasecmp _stricmp

#define DEFAULT_IMU_PORT  "COM4"
#else
#define DEFAULT_IMU_PORT  "/dev/ttyUSB0"
#endif

#include "configuration.h"
#include "cpp/DexCap.hpp"
#include "ImuOdometry.h"

using namespace DexRobot;

// ============================================================================
// Console helpers
// ============================================================================

char* CmdReadLine(void)
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
            char *temp = (char *)realloc(buffer, bufsize);
            if (!temp) {
                printf("CmdReadLine: realloc error\n");
                free(buffer);
                exit(1);
            }
            buffer = temp;
        }
    }
}

void clearWindow()
{
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}

const char* DeviceTypeName(ExoApparatus t)
{
    switch (t)
    {
    case ExoApparatus::UnDefn: return "UnDefn";
    case ExoApparatus::LGlove: return "LGlove";
    case ExoApparatus::RGlove: return "RGlove";
    case ExoApparatus::UpBody: return "UpBody";
    case ExoApparatus::IMUnit: return "IMUnit";
    case ExoApparatus::WRecvr: return "WRecvr";
    default:                   return "Unknown";
    }
}

// ============================================================================
// Helper: callback-based data counter (captures every frame)
// ============================================================================

void imuDataFetch(DexCapSuit* suit, uint32_t durationSec, bool printData)
{
    std::atomic<uint32_t> sampleCount{0};

    auto callback = [&](uint8_t, const std::shared_ptr<DeviceStatuData>& statusData)
    {
        if (statusData == nullptr || !statusData->valid())
            return;

        InertialUnitData data{};
        memcpy(&data, statusData->statusData(), sizeof(InertialUnitData));
        sampleCount++;

        if (printData)
        {
            fprintf(stdout,
                "sys:%u | Euler(%.1f, %.1f, %.1f) | "
                "Accel(%.3f,%.3f,%.3f) | Gyro(%.3f,%.3f,%.3f) | "
                "Quat(%.3f,%.3f,%.3f,%.3f)\n",
                data.system_time,
                data.roll, data.pitch, data.yaw,
                data.accel[0], data.accel[1], data.accel[2],
                data.gyscp[0], data.gyscp[1], data.gyscp[2],
                data.quat[0], data.quat[1], data.quat[2], data.quat[3]);
        }
    };

    size_t handle = suit->RegisterImuDataCallback(callback);
    std::this_thread::sleep_for(std::chrono::seconds(durationSec));
    suit->UnregisterImuDataCallback(handle);

    uint32_t freq = sampleCount / durationSec;
    fprintf(stdout, "IMU sampling frequency: %u Hz (total samples: %u)\n", freq, sampleCount.load());
}

// ============================================================================
// Test 1: IMU SDK Lifecycle Test
// ============================================================================

void RunImuSdkTest()
{
    fprintf(stdout, "\n========== IMU SDK Lifecycle Test ==========\n\n");

    auto suit = std::make_unique<DexCapSuit>(ProductVersion::V4);

    // ---- Connect ----
    ExoApparatus device = suit->ConnectDevice(DEFAULT_IMU_PORT, AdapterType::MODBUSUSB);
    if (device != ExoApparatus::IMUnit)
    {
        fprintf(stdout, "Failed to connect IMU device.\n");
        return;
    }
    fprintf(stdout, "[OK] Connected to IMU device.\n");

    std::string adapterName = suit->GetAdapterName(device);

    // ========================================================================
    // Device information queries
    // ========================================================================
    fprintf(stdout, "\n--- Device Information ---\n");
    fprintf(stdout, "  Adapter name    : %s\n", adapterName.c_str());
    fprintf(stdout, "  Product version : V%d\n", suit->productVersion());
    fprintf(stdout, "  Device ID       : %d\n", suit->GetDeviceID(device));
    fprintf(stdout, "  Serial number   : %s\n", suit->getSerialNumber(device).c_str());
    fprintf(stdout, "  Firmware version: %s\n", suit->GetFirmwareVersion(device).c_str());
    fprintf(stdout, "  IsConnected     : %s\n", suit->IsConnected(device) ? "true" : "false");
    fprintf(stdout, "  IsSensorEnabled : %s\n", suit->IsSensorEnabled(device) ? "true" : "false");
    fprintf(stdout, "  Adapter type    : %d\n", static_cast<int>(suit->GetAdapterType()));
    fprintf(stdout, "  anyError        : %s\n", suit->anyError(device) ? "true" : "false");

    // ---- Verify no data before start ----
    fprintf(stdout, "\n[TEST] Checking data before sampling starts (should be 0 Hz)...\n");
    imuDataFetch(suit.get(), 3, false);

    // ========================================================================
    // Start sampling
    // ========================================================================
    bool startOk = suit->Start() == DEX_RETURN::DEX_SUCCESS;
    fprintf(stdout, "\n--- Start Sampling ---\n");
    fprintf(stdout, "  Start()         : %s\n", startOk ? "OK" : "FAIL");
    if (!startOk) return;

    fprintf(stdout, "  IsSensorEnabled : %s\n", suit->IsSensorEnabled(device) ? "true" : "false");
    fprintf(stdout, "  anyError        : %s\n", suit->anyError(device) ? "true" : "false");

    // ---- Callback-based data verification ----
    fprintf(stdout, "\n[TEST] Callback data after start (5s)...\n");
    imuDataFetch(suit.get(), 5, false);

    // ---- Polling-based data read (GetIMUSensorData) ----
    fprintf(stdout, "\n[TEST] Polling data via GetIMUSensorData() (3s)...\n");
    {
        auto startTime = std::chrono::steady_clock::now();
        uint32_t count = 0;
        uint64_t lastSysTime = 0;
        while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(3))
        {
            InertialUnitData data = suit->GetIMUSensorData();
            if (data.system_time != lastSysTime)
            {
                count++;
                lastSysTime = data.system_time;
            }
        }
        fprintf(stdout, "  Polling freq: %u Hz (vs callback for comparison)\n", count / 3);
    }

    // ---- Running status ----
    fprintf(stdout, "\n--- Running Status ---\n");
    fprintf(stdout, "  IsRunning()     : %s\n", suit->IsRunning() ? "true" : "false");
    fprintf(stdout, "  IsRunning(dev)  : %s\n", suit->IsRunning(device) ? "true" : "false");

    // ========================================================================
    // Pause / Resume (device-level)
    // ========================================================================
    fprintf(stdout, "\n--- Pause/Resume (device-level) ---\n");
    fprintf(stdout, "  Pause(device)   : %s\n", suit->Pause(device) ? "OK" : "FAIL");
    fprintf(stdout, "[TEST] After device pause (should be 0 Hz)...\n");
    imuDataFetch(suit.get(), 3, false);

    fprintf(stdout, "  Resume(device)  : %s\n", suit->Resume(device) ? "OK" : "FAIL");
    fprintf(stdout, "[TEST] After device resume...\n");
    imuDataFetch(suit.get(), 3, false);

    // ========================================================================
    // Pause / Resume (global)
    // ========================================================================
    fprintf(stdout, "\n--- Pause/Resume (global) ---\n");
    fprintf(stdout, "  Pause()         : %s\n", suit->Pause() == DEX_RETURN::DEX_SUCCESS ? "OK" : "FAIL");
    fprintf(stdout, "[TEST] After global pause (should be 0 Hz)...\n");
    imuDataFetch(suit.get(), 3, false);

    fprintf(stdout, "  Resume()        : %s\n", suit->Resume() == DEX_RETURN::DEX_SUCCESS ? "OK" : "FAIL");
    fprintf(stdout, "[TEST] After global resume...\n");
    imuDataFetch(suit.get(), 3, false);

    // ========================================================================
    // Error state check
    // ========================================================================
    fprintf(stdout, "\n--- Error State ---\n");
    if (suit->anyError(device))
    {
        fprintf(stdout, "  Error code    : 0x%02X\n", static_cast<int>(suit->getErrorCode(device)));
        fprintf(stdout, "  Error message : %s\n", suit->getErrorMessage(device).c_str());
    }
    else
    {
        fprintf(stdout, "  No errors detected.\n");
    }

    // ========================================================================
    // GetDeviceType by adapter name
    // ========================================================================
    fprintf(stdout, "\n--- Device Type Lookup ---\n");
    ExoApparatus deviceType = suit->GetDeviceType(adapterName);
    fprintf(stdout, "  GetDeviceType(\"%s\") = %s\n", adapterName.c_str(), DeviceTypeName(deviceType));

    // ========================================================================
    // Frequency analysis
    // ========================================================================
    fprintf(stdout, "\n--- Frequency Analysis (callback, 3 x 5s) ---\n");
    imuDataFetch(suit.get(), 5, false);
    imuDataFetch(suit.get(), 5, false);
    imuDataFetch(suit.get(), 5, false);

    // ========================================================================
    // Close & Disconnect
    // ========================================================================
    fprintf(stdout, "\n--- Close & Disconnect ---\n");
    fprintf(stdout, "  Close()         : %s\n", suit->Close() == DEX_RETURN::DEX_SUCCESS ? "OK" : "FAIL");
    fprintf(stdout, "  anyError        : %s\n", suit->anyError(device) ? "true" : "false");
    fprintf(stdout, "  DisconnectDevice: %s\n", suit->DisconnectDevice(adapterName) ? "OK" : "FAIL");
    fprintf(stdout, "  IsConnected     : %s\n", suit->IsConnected(device) ? "true" : "false");

    fprintf(stdout, "\n========== IMU SDK Lifecycle Test END ==========\n");
}

// ============================================================================
// Test 2: IMU Callback Frequency Test
// ============================================================================

void RunImuFreqTest()
{
    fprintf(stdout, "\n========== IMU Callback Frequency Test ==========\n\n");

    auto suit = std::make_unique<DexCapSuit>(ProductVersion::V4);

    ExoApparatus device = suit->ConnectDevice(DEFAULT_IMU_PORT, AdapterType::MODBUSUSB);
    if (device != ExoApparatus::IMUnit)
    {
        fprintf(stdout, "Failed to connect IMU device.\n");
        return;
    }
    fprintf(stdout, "[OK] IMU device connected.\n");

    bool startOk = suit->Start() == DEX_RETURN::DEX_SUCCESS;
    fprintf(stdout, "[INFO] Sampling start: %s\n\n", startOk ? "OK" : "FAIL");
    if (!startOk) return;

    // Lightweight callback: store latest data + atomic counter
    std::mutex snapMutex;
    InertialUnitData latestData{};
    std::atomic<uint32_t> cbCount{0};

    auto callback = [&](uint8_t, const std::shared_ptr<DeviceStatuData>& statusData)
    {
        if (statusData == nullptr || !statusData->valid())
            return;

        InertialUnitData imu{};
        memcpy(&imu, statusData->statusData(), sizeof(InertialUnitData));

        {
            std::lock_guard<std::mutex> lock(snapMutex);
            latestData = imu;
        }
        cbCount++;
    };

    size_t handle = suit->RegisterImuDataCallback(callback);

    constexpr int testDurationSec = 30;

    auto testStart = std::chrono::steady_clock::now();
    uint32_t prevCount = 0;
    double curHz = 0.0;
    auto prevTick = testStart;

    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - testStart).count();
        if (elapsed >= testDurationSec) break;

        InertialUnitData snap;
        {
            std::lock_guard<std::mutex> lock(snapMutex);
            snap = latestData;
        }

        uint32_t total = cbCount;
        double tickElapsed = std::chrono::duration<double>(now - prevTick).count();

        if (tickElapsed >= 1.0)
        {
            curHz = static_cast<double>(total - prevCount) / tickElapsed;
            prevCount = total;
            prevTick = now;
        }

        fprintf(stdout,
            "\r sys:%u | Euler(%6.1f,% 6.1f,% 6.1f) | "
            "A(%4.2f,%4.2f,%4.2f) G(%5.2f,%5.2f,%5.2f) | "
            "frm:%5u | %4.0f Hz | %4.1fs  ",
            snap.system_time,
            snap.roll, snap.pitch, snap.yaw,
            snap.accel[0], snap.accel[1], snap.accel[2],
            snap.gyscp[0], snap.gyscp[1], snap.gyscp[2],
            total, curHz, elapsed);
        fflush(stdout);

        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    fprintf(stdout, "\n\n");

    suit->UnregisterImuDataCallback(handle);

    // Verify callback unregistration
    uint32_t finalCount = cbCount;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    fprintf(stdout, "Unregister check: %s\n",
        (cbCount == finalCount) ? "OK (no leaked callbacks)" : "WARNING: callbacks leaked!");

    double totalSec = std::chrono::duration<double>(prevTick - testStart).count();
    if (totalSec <= 0) totalSec = static_cast<double>(testDurationSec);

    fprintf(stdout, "--------------------------------------\n");
    fprintf(stdout, "  Total : %u samples / %.1fs = %.0f Hz avg\n", finalCount, totalSec, finalCount / totalSec);
    fprintf(stdout, "======================================\n");

    suit->Close();
    fprintf(stdout, "\n========== IMU Callback Frequency Test END ==========\n");
}

// ============================================================================
// Test 3: IMU Odometry Test
// ============================================================================

void RunImuOdomTest()
{
    fprintf(stdout, "\n========== IMU Odometry Demo Test ==========\n\n");

    auto suit = std::make_unique<DexCapSuit>(ProductVersion::V4);

    ExoApparatus device = suit->ConnectDevice(DEFAULT_IMU_PORT, AdapterType::MODBUSUSB);
    if (device != ExoApparatus::IMUnit)
    {
        fprintf(stdout, "Failed to connect IMU device.\n");
        return;
    }
    fprintf(stdout, "[OK] IMU device connected.\n");

    bool startOk = suit->Start() == DEX_RETURN::DEX_SUCCESS;
    fprintf(stdout, "[INFO] Sampling start: %s\n", startOk ? "OK" : "FAIL");
    if (!startOk)
    {
        suit->Close();
        return;
    }

    // Create odometry engine (registers IMU callback internally)
    auto odom = std::make_unique<ImuOdometry>(suit.get());

    // Register state callback: print position, velocity, and stationary status
    odom->setStateCallback([](const ImuOdometry::OdometryState& s)
    {
        fprintf(stdout,
            "\r [%s] pos:(% 7.3f, % 7.3f, % 7.3f)m  vel:(% 6.3f, % 6.3f, % 6.3f)m/s  |vel|=%.3fm/s   ",
            s.stationary ? "STATIC" : "MOVING",
            s.position_world_m.x, s.position_world_m.y, s.position_world_m.z,
            s.velocity_world_mps.x, s.velocity_world_mps.y, s.velocity_world_mps.z,
            s.velocity_world_mps.norm());
    });

    fprintf(stdout, "************************************************************************\n");
    fprintf(stdout, "Instructions:\n");
    fprintf(stdout, "  1. Keep the IMU perfectly STILL during calibration (first 2 seconds).\n");
    fprintf(stdout, "  2. After calibration, move the IMU to see odometry tracking.\n");
    fprintf(stdout, "  3. Each test round runs for 30 seconds.\n");
    fprintf(stdout, "  4. After each round, you have 5 seconds to prepare for the next.\n");
    fprintf(stdout, "************************************************************************\n");
    fprintf(stdout, "Press Enter to start testing...\n");

#ifdef _WIN32
    int btn = getchar();
    if (!(btn == '\n' || btn == '\r')) {
        suit->Close();
        return;
    }
#else
    // Press Enter to continue (Linux)
    getchar();
#endif

    int round = 0;
    while (true)
    {
        round++;
        clearWindow();

        if (odom->isRunning()) odom->stop();
        odom->start();
        odom->reset();

        fprintf(stdout, "\n========== Round %d ==========\n", round);

        // Calibration: keep device still
        fprintf(stdout, "Calibrating IMU bias, keep the device STILL...\n");
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Run odometry for 30 seconds
        fprintf(stdout, "Odometry running (30 seconds), move the IMU...\n");
        constexpr int testDurationSec = 30;
        std::this_thread::sleep_for(std::chrono::seconds(testDurationSec));

        odom->stop();

        auto state = odom->getState();
        fprintf(stdout, "\n\n");
        fprintf(stdout, "Final state:\n");
        fprintf(stdout, "  Position: (%.3f, %.3f, %.3f) m\n",
            state.position_world_m.x, state.position_world_m.y, state.position_world_m.z);
        fprintf(stdout, "  Velocity: (%.3f, %.3f, %.3f) m/s\n",
            state.velocity_world_mps.x, state.velocity_world_mps.y, state.velocity_world_mps.z);

        fprintf(stdout, "\nPress Enter for next round, or type 'q' to quit...\n");

        char c = (char)getchar();
        if (c == 'q' || c == 'Q') break;
        // consume the newline
        if (c != '\n') getchar();

        fprintf(stdout, "Starting next round in 3 seconds...\n");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    suit->Close();
    fprintf(stdout, "\n========== IMU Odometry Demo Test END ==========\n");
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, const char** argv)
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    printf("========================================\n");
    printf("  IMU Example - ExoSkeleton SDK\n");
    printf("========================================\n");
    printf("Commands:\n");
    printf("  sdk   - IMU SDK lifecycle test (connect/start/pause/resume/close)\n");
    printf("  freq  - IMU callback frequency test (30s callback-based sampling)\n");
    printf("  odom  - IMU odometry demo (dead-reckoning with ImuOdometry)\n");
    printf("  quit  - Exit\n");
    printf("========================================\n\n");

    do {
        printf("IMU> ");
        auto cmd = CmdReadLine();
        if (strlen(cmd) == 0)
        {
            free(cmd);
            continue;
        }

        if (strcasecmp(cmd, "quit") == 0 || strcasecmp(cmd, "exit") == 0)
        {
            free(cmd);
            break;
        }

        if (strcasecmp(cmd, "sdk") == 0)
        {
            RunImuSdkTest();
        }
        else if (strcasecmp(cmd, "freq") == 0)
        {
            RunImuFreqTest();
        }
        else if (strcasecmp(cmd, "odom") == 0)
        {
            RunImuOdomTest();
        }
        else
        {
            printf("Unknown command: %s\n", cmd);
            printf("Available commands: sdk, freq, odom, quit\n");
        }

        free(cmd);
    } while (true);

    return 0;
}
