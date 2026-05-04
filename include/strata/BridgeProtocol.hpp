#pragma once


class BridgeSerial;


class BridgeProtocol
{
public:
    BridgeProtocol(BridgeSerial *commands);

private:
    BridgeSerial *m_commands;
};


