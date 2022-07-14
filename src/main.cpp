#include <iostream>
#include "protocol.hpp"
#include "communication.hpp"
#include <string>
#include <thread>

int main(int argc, char **argv)
{
	std::string ip {"192.168.178.25"};
	std::string port {"5900"};

	std::unique_ptr<ICommunication> comm = std::make_unique<Communication> (ip, port);

	rfbProtocol protocol(std::move(comm));

	std::thread th(
	[&](){

	if (protocol.Init())
	{
		std::cout << "OK" << std::endl;
	}

	//unsigned short buffw = 0;
	//unsigned short buffh = 0;
	//unsigned char PIX_FMT[16] = {0,};
	//unsigned char nameLength[4] = {0,};

	/*//client.Receive(reinterpret_cast<unsigned char*>(&buffw), 2);
	std::cout << buffw << std::endl;

	//client.Receive(reinterpret_cast<unsigned char*>(&buffh), 2);
	std::cout << buffh << std::endl;

	//client.Receive(PIX_FMT, 16);
	for(size_t i = 0; i < 16; ++i)
	{
		std::cout << "0x" << std::hex << int(PIX_FMT[i]) << " ";
	}
	std::cout << std::endl;

	//client.Receive(nameLength, 4);
	for(size_t i = 0; i < 4; ++i)
	{
		std::cout << "0x" << std::hex << int(nameLength[i]) << " ";
	}
	std::cout << std::endl;*/

	});

	if (th.joinable())
	{
		th.join();
	}
	return 0;
}
