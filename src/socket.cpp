/*
 * =====================================================================================
 *
 *       Filename:  socket.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13/08/2018 13:39:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "socket.hpp"
#include "logging.hpp"

Socket::Socket(Socket&&s): fdsocket(s.fdsocket), mType(s.mType), servinfo(s.servinfo)
{
	s.fdsocket = -1;
	s.servinfo = nullptr;
}
	
Socket::~Socket() 
{ 
	fdclose();
}

size_t Socket::getBufferSize() const
{
	return 2048;
}

bool Socket::isValid ()const
{
	if (-1 == fdsocket)
	{
		return false;
	}
	return true;
}	

Socket Socket::operator = (Socket &&s)
{
	std::swap(fdsocket, s.fdsocket);
	std::swap(servinfo, s.servinfo);
	mType = s.mType;
	return std::move(*this);
}

void Socket::fdclose()
{
	if (-1 != fdsocket) 
	{
		close(fdsocket);
		if (nullptr != servinfo)
		{
			freeaddrinfo(servinfo);
			servinfo = nullptr;
		}
	}
}

const int Socket::getSocket() const
{
	return fdsocket;
}

size_t Socket::Send(const unsigned char *buffer, size_t size)
{
	if (nullptr != buffer && fdsocket == -1)
	{
		printLog("Incorrect arguments");
		return 0;	
	}
	int ret = 0;
	int retry = 5;
	do
	{
		ret += send(fdsocket, reinterpret_cast<const void*>(buffer + ret), size - ret, 0);
		if (ret == -1)
		{
			int err = errno;
			printLog("Error sending ", strerror(err));
			return 0;
		}
		if (ret != int(size))
		{
			printLog("Did not send everyhting ", size, " ", ret);
			if (!retry --)
			{
				break;
			}
		}
	}
	while(ret != int(size));
	return ret;
}

size_t Socket::Receive(unsigned char *buffer, size_t size, bool fillEntireBuffer)
{
	if (nullptr != buffer && fdsocket == -1)
	{
		printLog("Incorrect arguments");
		return 0;	
	}
	int ret = 0;
	int retry = 5;
	do
	{
		ret += recv(fdsocket, reinterpret_cast<void*>(buffer + ret), size - ret, 0);
		if (ret == -1)
		{
			int err = errno;
			printLog("Error receiving ", strerror(err));
			return 0;
		}
		if (ret != int(size))
		{
			printLog("Did not fill the eintire buffer", size, " ", ret);
			if (! retry --)
			{
				break;
			}
		}
	}
	while(ret != int(size) && fillEntireBuffer);
	return ret;
}

Socket::Socket():
	fdsocket(-1),
	mType(INVALID),
	servinfo(nullptr)
{
}

Socket::Socket(const int &s, Socket_type_t st): fdsocket(s),mType(st),servinfo(nullptr) 
{
}

Socket::Socket(const int &s): fdsocket(s), mType(SERVER),servinfo(nullptr) 
{
}

Socket::Socket(const std::string &ip, const std::string &port):fdsocket(-1)
{
	if (ip.size() == 0 && port.size() == 0)
	{
		printLog("Error! No port or IP!");
		return;
	}	

	int status;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	if ((status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &servinfo)) != 0) 
	{	
		int err = errno;
    	printLog("getaddrinfo error:", gai_strerror(status)," ", strerror(err));
	    return;
	}
				
	int	s = socket(servinfo->ai_family, servinfo->ai_socktype | SOCK_CLOEXEC, servinfo->ai_protocol);
	if (-1 == s)
	{
		int err = errno;
		printLog(strerror(err));
		return;
	}	
	//printLog("Socket returned ", s);
	fdsocket = s;
	printLog(*this);
}

Socket Socket::operator = (const int&s)
{
	fdclose();
	fdsocket = s;
	mType = CLIENT;
	return std::move(*this);
}

std::ostream & operator << (std::ostream &st, const Socket &s)
{
	st << "Socket value "<< s.fdsocket << " Socket type " << (s.mType == Socket::SERVER ? "Server":"Client") << " Serv info " << (s.servinfo == nullptr? "nullptr": (void*)s.servinfo)	;
	return st;
}

Socket startServer(const std::string &ip, const std::string &port)
{
	Socket s(ip, port);
	if (-1 == s.fdsocket)
	{
		printLog("Invalid socket");
		return Socket(-1);
	}
	int ret = bind(s.fdsocket, s.servinfo->ai_addr, s.servinfo->ai_addrlen);		
	if (-1 == ret)
	{
		int err = errno;
		printLog(strerror(err));
		return Socket(-1);
	}

	ret = listen(s.fdsocket, 10);
	if (-1 == ret)
	{
		int err = errno;
		printLog(strerror(err));
		return Socket(-1);
	}

	s.mType = Socket::SERVER;
	printLog(s);
	return s;	
}

Socket ConnectToServer(const std::string &ip, const std::string &port)
{
	Socket s(ip, port);
	if (-1 == s.fdsocket)
	{
		return -1;
	}
	
	int ret = connect(s.fdsocket, s.servinfo->ai_addr, s.servinfo->ai_addrlen);

	if (-1 == ret)
	{
		int err = errno;
		printLog(strerror(err));
		return -1;
	}
	printLog(s);
	return s;
}

Socket getNewClientConnection(const Socket &s)
{
	if (s.fdsocket == -1 || s.mType != Socket::SERVER)
	{
		printLog("Invalid Socket ");
		return -1;
	}
	struct sockaddr_storage their_addr;	
	socklen_t addr_size = sizeof their_addr;
	int new_fd = accept(s.fdsocket, (struct sockaddr *)&their_addr, &addr_size);
	if (-1 == new_fd)
	{
		int err = errno;
		printLog(strerror(err));
		return -1;
	}
	
	return Socket(new_fd, Socket::CLIENT);
}
