#include "protocol.hpp"
#include "logging.hpp"
#include <iostream>

void rfbProtocol::SetFrameBufferUpdateCallback(const frameBufferUpdateCallback& cb)
{
	m_frameBufferUpdateCallback = cb;
}

void rfbProtocol::SetSetColourMapentriesCallback(const setColourMapentriesCallback& cb)
{
	m_setColourMapentrciesCallback = cb;
}

void rfbProtocol::SetBellCallback(const bellCallback& cb)
{
	m_bellCallback = cb;
}

void rfbProtocol::SetServerCutTextCallback(const ServerCutTextCallback& cb)
{
	m_ServerCutTextCallback = cb;
}

rfbProtocol::rfbProtocol(std::unique_ptr<ICommunication>&& comm):
	m_comm(std::move(comm))
{}

rfbProtocol::rfbProtocol(rfbProtocol&& obj):
	m_frameBufferUpdateCallback(obj.m_frameBufferUpdateCallback),
	m_setColourMapentrciesCallback(obj.m_setColourMapentrciesCallback),
	m_bellCallback(obj.m_bellCallback),
	m_ServerCutTextCallback(obj.m_ServerCutTextCallback),
	m_comm(std::move(obj.m_comm))
{}

rfbProtocol::~rfbProtocol()
{}

bool rfbProtocol::Init()
{
	m_comm->Open();

	if(m_comm->IsConnectionOpen())
	{
		printLog("Connected to server");
		const unsigned char buffer[] = {0x52, 0x46, 0x42, 0x20, 0x30, 0x30, 0x33, 0x2e, 0x30, 0x30, 0x33, 0x0a};

		// RFB protocl handshake
		unsigned char recbuffer[12] = {0,};
		printLog("Received header of size ", m_comm->Receive(recbuffer, sizeof(recbuffer), true ));

		for(const auto &it: recbuffer)
		{
			printLog(it);
		}
		m_comm->Send(buffer, sizeof(buffer));

		unsigned char security[10] = {0,};
		m_comm->Receive(security, 4, true);
		for(size_t i = 0; i < 4; ++i)
		{
			printLog(int(security[i]));
		}

		if (int(security[3]) == 0)
		{
			m_comm->Receive(recbuffer, 4, true);
			for(size_t i = 0; i < 4; ++i)
			{
				printLog(recbuffer[i]);
			}
			return false;
		}

		unsigned char sec = 0x00;
		m_comm->Send(&sec,1);

		// Handhsake ended
		return true;
	}
	else
	{
		printLog("Unable to connect to server");
		return false;
	}
}

void rfbProtocol::SetPixelFormat()
{}
void rfbProtocol::SetEncoding()
{}
void rfbProtocol::FrameBufferUpdateRequest()
{}
void rfbProtocol::KeyEvent()
{}
void rfbProtocol::PointerEvent()
{}
void rfbProtocol::ClientCutText()
{}
