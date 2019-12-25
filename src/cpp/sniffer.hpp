#pragma once

#include <tins/tins.h>
#include <string>
#include <thread>
#include "packetPool.hpp"

using std::string;

namespace pinat {
	class Sniffer {
	public:
		Sniffer(string interface, string filter);
		~Sniffer();

		unsigned long getPacket() const;
		string getLayers(Tins::PDU* packet) const;
	private:
		Tins::Sniffer* _sniffer;
        PacketPool* _packetPool;
	};
}