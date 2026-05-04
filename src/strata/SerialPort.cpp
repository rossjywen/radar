#include "strata/SerialPort.hpp"

#include <stdexcept> // for std::runtime_error
#include <iostream> // for std::cout, std::endl
#include <fcntl.h> // for O_RDWR, O_NOCTTY
#include <unistd.h> // for close()
#include <termios.h> // for termios, tcgetattr(), tcsetattr(), cfsetispeed(), cfsetospeed(), tcflush()

#ifdef __APPLE__
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
#endif


namespace
{
    speed_t getBaudrateFlags(uint32_t baudrate)
    {
        switch (baudrate)
        {
            case 9600:   return B9600;
            case 19200:  return B19200;
            case 38400:  return B38400;
            case 57600:  return B57600;
            case 115200: return B115200;
            case 230400: return B230400;

            // 以下宏在 macOS 上未定義，使用預處理器檢查
            #ifdef B460800
            case 460800:  return B460800;
            #endif

            #ifdef B500000
            case 500000:  return B500000;
            #endif

            #ifdef B576000
            case 576000:  return B576000;
            #endif

            #ifdef B921600
            case 921600:  return B921600;
            #endif

            #ifdef B1000000
            case 1000000: return B1000000;
            #endif

            #ifdef B1152000
            case 1152000: return B1152000;
            #endif

            #ifdef B1500000
            case 1500000: return B1500000;
            #endif

            #ifdef B2000000
            case 2000000: return B2000000;
            #endif

            #ifdef B2500000
            case 2500000: return B2500000;
            #endif

            #ifdef B3000000
            case 3000000: return B3000000;
            #endif

            #ifdef B3500000
            case 3500000: return B3500000;
            #endif

            #ifdef B4000000
            case 4000000: return B4000000;
            #endif

            default:
                // 如果在 macOS 上傳入了高速波特率，這裡會拋出異常
                // 這是合理的，因為 macOS 的 tcsetattr 確實不支持這些宏
                throw std::runtime_error("SerialPort::getBaudrateFlags - baud rate unsupported on this platform");
        }
    }
}


SerialPort::SerialPort() : 
    m_fd{.fd = -1, .events = POLLIN, .revents = 0},
    m_timeout(0)
{
}



bool SerialPort::isOpened()
{
    return (m_fd.fd != -1);
}


void SerialPort::open(const char *port, uint32_t baudrate, uint16_t timeout)
{
    if (isOpened())
    {
        std::cout << "[Ross] Serial port already opened." << std::endl;
        return;
    }

    std::cout << "[Ross] Opening serial port: " << port << " at baudrate: " << baudrate << std::endl;

    m_fd.fd = ::open(port, O_RDWR | O_NOCTTY);
    if (m_fd.fd == -1)
    {
        throw std::runtime_error("Failed to open serial port");
    }

#ifdef __APPLE__
// 這是 macOS 設定任意波特率的特有方式
    speed_t speed = baudrate; 
    if (ioctl(m_fd.fd, IOSSIOSPEED, &speed) == -1) {
        // 處理錯誤
    }
#endif

    int ret;
    struct termios serialPortSettings;

    ret = tcgetattr(m_fd.fd, &serialPortSettings);
    if (ret < 0)
    {
        SerialPort::close();
        throw std::runtime_error("SerialPort::open - tcgetattr() failed");
    }

    //--------------------------------------------------------------------------------
    // SETTINGS
    //--------------------------------------------------------------------------------
    setBaudrate(&serialPortSettings, baudrate);  // read speed

    serialPortSettings.c_cflag &= static_cast<unsigned int>(~(PARENB | CSTOPB | CRTSCTS | CSIZE));  // no parity, 1 stopbit, clear CS mask
    serialPortSettings.c_cflag |= static_cast<unsigned int>(CREAD | CLOCAL | CS8);                  // enable receiving, 8 databits
    serialPortSettings.c_lflag     = 0u;                                                            // raw mode (non-canonical mode)
    serialPortSettings.c_iflag     = 0u;                                                            // no input flags
    serialPortSettings.c_oflag     = 0u;                                                            // no output flags
    serialPortSettings.c_cc[VMIN]  = 0;
    serialPortSettings.c_cc[VTIME] = 1;  // return when for 100ms there hasn't been a new byte received while still expecting bytes

    ret = tcsetattr(m_fd.fd, TCSANOW, &serialPortSettings);  // apply settings immediately
    if (ret < 0)
    {
        SerialPort::close();
        throw std::runtime_error("SerialPort::open - tcsetattr() failed");
    }

    // Flush the input buffer
    clearInputBuffer();

    m_timeout = timeout;
}

void SerialPort::close()
{
    if (isOpened())
    {
        std::cout << "[Ross] Closing serial port: " << m_fd.fd << std::endl;
        ::close(m_fd.fd);
        m_fd.fd = -1;
    }
}


void SerialPort::clearInputBuffer()
{
    const int ret = tcflush(m_fd.fd, TCIFLUSH);
    if (ret < 0)
    {
        throw std::runtime_error("SerialPort::clearInputBuffer - tcflush() failed");
    }
}


void SerialPort::setBaudrate(struct termios *serialPortSettings, uint32_t baudrate)
{
    const auto br = getBaudrateFlags(baudrate);

    cfsetispeed(serialPortSettings, br);
    cfsetospeed(serialPortSettings, br);
}


