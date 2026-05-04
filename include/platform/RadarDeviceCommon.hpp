#pragma once

#include "platform/BoardInstance.hpp"
#include <memory> // for std::unique_ptr


struct BoardData
{
    uint16_t vid;
    uint16_t pid;
};


std::unique_ptr<BoardInstance> open_board(void);


