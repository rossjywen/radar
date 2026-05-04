#include "platform/RadarDeviceCommon.hpp"
#include "strata/BridgeSerial.hpp"

#include <glob.h>   // 負責處理路徑通配符 (glob_t, glob, globfree, GLOB_APPEND)
#include <cstdio>   // 負責標準輸入輸出 (printf)
#include <iostream> // 負責輸出到控制台 (std::cout, std::endl)
#include <memory>   // 負責智能指針 (std::unique_ptr)

namespace
{
#ifdef __APPLE__
    const char *devList[] = {
        "/dev/cu.usb*",
    };
#elif __linux__
    const char *devList[] = {
        "/dev/ttyAMA*",
        "/dev/ttyACM*",
        "/dev/ttyUSB*",
    };
#endif
}

//void enumerate(BoardData::const_iterator begin, BoardData::const_iterator end)
std::unique_ptr<BridgeSerial> enumerate()
{
    glob_t glob_results = {};

    for (auto d = std::begin(devList); d < std::end(devList); d++)
    {
        glob(*d, GLOB_APPEND, nullptr, &glob_results);
    }

    for (uint_fast16_t i = 0; i < glob_results.gl_pathc; i++)
    {
        std::cout << "[Ross] Found device: " << glob_results.gl_pathv[i] << std::endl;
        auto bridgeSerial = std::make_unique<BridgeSerial>(glob_results.gl_pathv[i], 921600);
        globfree(&glob_results);

        return bridgeSerial;

        // if (enumerateFunction<BoardSerial>(listener, begin, end, glob_results.gl_pathv[i]))
        // {
        //     break;
        // }
    }

    globfree(&glob_results);

    return nullptr;
}


std::unique_ptr<BoardInstance> open_board(void)
{
    auto bridgeSerial = enumerate();
    if (!bridgeSerial)
    {
        std::cout << "[Ross] No device found." << std::endl;
        return nullptr;
    }

    

    return nullptr;
}

