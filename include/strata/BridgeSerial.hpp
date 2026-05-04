#pragma once

#include "strata/BridgeProtocol.hpp"
#include "strata/SerialPort.hpp"

#include <cstdint> // for uint32_t
#include <string> // for std::string
#include <poll.h> // for struct pollfd
#include <chrono> // for std::chrono::duration

class BridgeSerial
{
public:
    BridgeSerial(char *port, uint32_t bautrate); 


    void openConnection();

private:
    uint32_t m_bautrate;
    std::string m_portName;
    BridgeProtocol m_protocol;
    SerialPort m_port;
    std::chrono::duration<unsigned int, std::milli> m_timeout;
};




