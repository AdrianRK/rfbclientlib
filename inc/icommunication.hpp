#ifndef REMOTE_FRAME_BUFFER_COMMUNICATION_INTERFACE
#define REMOTE_FRAME_BUFFER_COMMUNICATION_INTERFACE

#include <cstddef>

class ICommunication
{
public:
	virtual ~ICommunication() = default;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual size_t Send(const unsigned char* buffer, size_t size) = 0;
	virtual size_t Receive(unsigned char* buffer, size_t size, bool fillEntireBuffer) = 0;
	virtual bool IsConnectionOpen() = 0;
};

#endif // REMOTE_FRAME_BUFFER_COMMUNICATION_INTERFACE
