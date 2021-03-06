#include "packetPool.hpp"
#include "sniffer.hpp"
#include "coreFunctions.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	if(getuid() != 0)
	{
		cout << "call as root" << endl;
		return 1;
	}

	pinat::Sniffer* s = new pinat::Sniffer("br0", "", "wlp2s0", "enp1s0", "08:6a:0a:22:12:1a");
	unsigned long p = 0;
	while(true)
	{
		p = s->getPacket();
		s->forwardPacket(p);
	}

	delete s;
	// if(argc != 4)
	// {
	// 	cout << "usage: sniffer <sniffing interface> <sending interface> <router mac>" << endl;
	// 	return 1;
	// }

	// pinat::Sniffer* s = new pinat::Sniffer(argv[1], "tcp and port 31337", argv[2], argv[3]);
	// pinat::initCore(s->getPacketPool());
	
	// unsigned long a = 0;
	// std::string command = "";
	// while (command != "x")
	// {
	// 	cout << ">>> ";
	// 	std::cin >> command;
		
	// 		if(command == "s")
	// 		a = s->getPacket();
	// 	else if(command == "m")
	// 	{
	// 		std::vector<std::string>* vec = pinat::getMACs(a);
	// 		if(vec)
	// 		{
	// 			cout << vec->at(0) << " -> " << vec->at(1) << endl;
	// 			delete vec;
	// 		}
	// 		else
	// 		{
	// 			cout << "no mac" << endl;
	// 		}
	// 	}
	// 	else if(command == "i")
	// 	{
	// 		std::vector<std::string>* vec = pinat::getIPs(a);
	// 		if(vec)
	// 		{
	// 			cout << vec->at(0) << " -> " << vec->at(1) << endl;
	// 			delete vec;
	// 		}
	// 		else
	// 		{
	// 			cout << "no ip" << endl;
	// 		}
	// 	}
	// 	else if(command == "p")
	// 	{
	// 		std::vector<unsigned int>* vec = pinat::getPorts(a);
	// 		if(vec)
	// 		{
	// 			cout << vec->at(0) << " -> " << vec->at(1) << endl;
	// 			delete vec;
	// 		}
	// 		else
	// 		{
	// 			cout << "no ports" << endl;
	// 		}
	// 	}

	// 	else if(command == "e")
	// 	{
	// 		std::vector<std::string> blacklist = {"lol"};
	// 		pinat::censorWords(a, "----", &blacklist);
	// 	}
	// 	else if(command == "d")
	// 	{
	// 		std::map<std::string, std::vector<std::string>*>* dnsInfo = pinat::getDNSInfo(a);
	// 		if(dnsInfo)
	// 		{
	// 			for (auto i = dnsInfo->begin(); i != dnsInfo->end(); i++)
	// 			{
	// 				std::cout << "Dname: " + i->first << std::endl;
	// 				std::cout << "IPs: ";
	// 				std::vector<std::string>* ips = i->second;
	// 				for (const std::string address : *ips)
	// 				{
	// 					std::cout << address << ", ";
	// 				}
	// 				std::cout << std::endl;
	// 				delete ips;
	// 			}

	// 			delete dnsInfo;
	// 		}
	// 		else
	// 		{
	// 			cout << "no dns response" << endl;
	// 		}
			
	// 	}
	// 	else if(command == "f")
	// 	{
	// 		s->forwardPacket(a);
	// 	}
	// }


	// cout << "Exiting" << endl;
	// delete s;
	return 0;
}