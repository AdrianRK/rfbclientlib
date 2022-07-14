#include "communication.hpp"

Communication::Communication(const std::string& ip, const std::string& port):
	m_ip(ip),
	m_port(port)
{}

Communication::~Communication()
{
	Close();
}

void Communication::Open()
{
	m_socket = ConnectToServer(m_ip, m_port);
}

void Communication::Close()
{
	m_socket.fdclose();
}

size_t Communication::Send(const unsigned char* buffer, size_t size)
{
	return m_socket.Send(buffer, size);
}

size_t Communication::Receive(unsigned char* buffer, size_t size, bool fillEntireBuffer)
{
	return m_socket.Receive(buffer, size, fillEntireBuffer);
}

bool Communication::IsConnectionOpen()
{
	return m_socket.isValid();
}
