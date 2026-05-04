#pragma once

#include <poll.h> // for struct pollfd
#include <cstdint> // for uint32_t, uint16_t
#include <termios.h> // for struct termios, tcgetattr()



class SerialPort
{
public:
    SerialPort();

    void open(const char *port, uint32_t baudrate, uint16_t timeout);
    bool isOpened();
    void close();
    void clearInputBuffer();
    void setBaudrate(struct termios *serialPortSettings, uint32_t baudrate);


private:
   struct pollfd m_fd;
    int m_timeout;
};
