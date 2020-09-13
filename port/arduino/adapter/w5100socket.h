#pragma once
#include <Arduino.h>
#include <IPAddress.h>

class W5100Socket {
public:
	static uint16_t getTXFreeSize(uint8_t s);
	static uint16_t getRXReceivedSize(uint8_t s);

	static void Close(uint8_t s);
	static uint8_t Begin(uint8_t protocol, uint16_t port);
	static uint8_t BeginMulticast(uint8_t protocol, IPAddress ip, uint16_t port);
	static uint16_t SendTo(uint8_t s, const uint8_t* buf, uint16_t len, uint8_t* addr, uint16_t port);
	static uint16_t RecvFrom(uint8_t s, uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t *port);
	static uint8_t Status(uint8_t);

	static uint8_t socket(uint8_t s, uint8_t protocol, uint16_t port, uint8_t flag);

private:
	static bool StartUDP(uint8_t s, uint8_t* addr, uint16_t port);
	static bool SendUDP(uint8_t s);
	static uint16_t BufferData(uint8_t s, uint16_t offset, const uint8_t* buf, uint16_t len);
	static int Recv(uint8_t s, uint8_t *buf, int16_t len);

	static uint16_t RecvAvailable(uint8_t s);
	static void PortRand(uint16_t n);
	static uint8_t Peek(uint8_t s);
	static uint8_t Listen(uint8_t s);
	static void Connect(uint8_t s, uint8_t * addr, uint16_t port);
	static void Disconnect(uint8_t s);
	static uint16_t Send(uint8_t s, const uint8_t * buf, uint16_t len);
	static uint16_t SendAvailable(uint8_t s);

	static uint16_t local_port;
};
