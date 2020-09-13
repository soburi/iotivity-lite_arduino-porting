/* Copyright 2018 Paul Stoffregen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include <Ethernet.h>
#include <utility/w5100.h>
#include "w5100socket.h"

uint16_t W5100Socket::SendTo(uint8_t s, const uint8_t* buf, uint16_t len, uint8_t* addr, uint16_t port)
{
    if(!StartUDP(s, addr, port)) {
        return 0;
    }
    uint16_t ret = BufferData(s, 0, buf, len);

    if(!SendUDP(s)) {
        return 0;
    }
    return ret;
}

uint16_t W5100Socket::RecvFrom(uint8_t s, uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t *port)
{
	if (RecvAvailable(s) <= 0) return 0;

	uint16_t _remaining = 0;
	//HACK - hand-parse the UDP packet using TCP recv method
	uint8_t tmpBuf[8];
	int ret=0;
	//read 8 header bytes and get IP and port from it
	ret = Recv(s, tmpBuf, 8);

	if (ret <= 0) return 0;

	//TODO htons
	addr[0] = tmpBuf[0];
	addr[1] = tmpBuf[1];
	addr[2] = tmpBuf[2];
	addr[3] = tmpBuf[3];
	*port = tmpBuf[4];
	*port = (*port << 8) + tmpBuf[5];
	_remaining = tmpBuf[6];
	_remaining = (_remaining << 8) + tmpBuf[7];

	if (_remaining <= 0) return 0;

	int got;
	if (_remaining <= len) {
		// data should fit in the buffer
		got = Recv(s, buf, _remaining);
	} else {
		// too much data for the buffer,
		// grab as much as will fit
		got = Recv(s, buf, len);
	}
	if (got > 0) {
		_remaining -= got;
		//Serial.printf("UDP read %d\n", got);
		return got;
	}

	// If we get here, there's no data available or recv failed
	return 0;
}


uint16_t W5100Socket::getTXFreeSize(uint8_t s)
{
    uint16_t val=0, val1=0;
    do {
        val1 = W5100.readSnTX_FSR(s);
        if (val1 != 0)
            val = W5100.readSnTX_FSR(s);
    } 
    while (val != val1);
    return val;
}

uint16_t W5100Socket::getRXReceivedSize(uint8_t s)
{
    uint16_t val=0,val1=0;
    do {
        val1 = W5100.readSnRX_RSR(s);
        if (val1 != 0)
            val = W5100.readSnRX_RSR(s);
    } 
    while (val != val1);
    return val;
}

/**
 * @brief	This Socket function initialize the channel in perticular mode, and set the port and wait for w5500 done it.
 * @return 	1 for success else 0.
 */
uint8_t W5100Socket::socket(uint8_t s, uint8_t protocol, uint16_t port, uint8_t flag)
{
  if ((protocol == SnMR::TCP) || (protocol == SnMR::UDP) || (protocol == SnMR::IPRAW) || (protocol == SnMR::MACRAW) || (protocol == SnMR::PPPOE))
  {
    Close(s);
    W5100.writeSnMR(s, protocol | flag);
    if (port != 0) {
      W5100.writeSnPORT(s, port);
    } 
    else {
      local_port++; // if don't set the source port, set local_port number.
      W5100.writeSnPORT(s, local_port);
    }

    W5100.execCmdSn(s, Sock_OPEN);
    
    return 1;
  }

  return 0;
}
