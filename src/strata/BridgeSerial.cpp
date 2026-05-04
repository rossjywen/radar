#include "strata/BridgeSerial.hpp"


namespace
{
    // constexpr bool setLocalTimestamp = false;

    // constexpr const uint16_t packetStartSize = 4;
    // constexpr const uint16_t packetCrcSize   = 2;
    // constexpr const uint16_t frameHeaderSize = 6;
    // constexpr const uint32_t timestampSize   = sizeof(uint64_t);

    constexpr const uint16_t portTimeout = 100;

    constexpr const std::chrono::milliseconds enumerateTimeout(portTimeout);
    constexpr const std::chrono::milliseconds defaultTimeout(1000);
}


BridgeSerial::BridgeSerial(char *port, uint32_t bautrate) :
    m_bautrate{bautrate},
    m_portName{port},
    m_protocol{this}
{
    openConnection();
}


void BridgeSerial::openConnection()
{
    // m_packetCounter = 0;
    // m_commandActive = false;
    // m_resynchronize = false;

    // m_cachedPacket = None;

    m_port.open(m_portName.c_str(), m_bautrate, 100);
    m_port.clearInputBuffer();

    m_timeout = enumerateTimeout;
}



