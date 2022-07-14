#ifndef REMOTE_FRAME_BUFFER_COMMUNICATION
#define REMOTE_FRAME_BUFFER_COMMUNICATION

#include "icommunication.hpp"
#include <string>
#include "socket.hpp"

class Communication : public ICommunication
{
public:
	Communication(const std::string& ip, const std::string& port);
	~Communication() override;
	void Open() override;
	void Close() override;
	size_t Send(const unsigned char* buffer, size_t size) override;
	size_t Receive(unsigned char* buffer, size_t size, bool fillEntireBuffer) override;
	bool IsConnectionOpen() override;

private:
	Socket m_socket;
	std::string m_ip;
	std::string m_port;
};

#endif // REMOTE_FRAME_BUFFER_COMMUNICATION
