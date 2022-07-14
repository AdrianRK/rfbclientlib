#ifndef REMOTE_FRAME_BUFFER_PROTOCOL
#define REMOTE_FRAME_BUFFER_PROTOCOL

#include <functional>
#include <memory>
#include "icommunication.hpp"

using frameBufferUpdateCallback = std::function<void(void)>;
using setColourMapentriesCallback = std::function<void(void)>;
using bellCallback = std::function<void(void)>;
using ServerCutTextCallback = std::function<void(void)>;

class rfbProtocol final
{
public:
	void SetFrameBufferUpdateCallback(const frameBufferUpdateCallback& cb);
	void SetSetColourMapentriesCallback(const setColourMapentriesCallback& cb);
	void SetBellCallback(const bellCallback& cb);
	void SetServerCutTextCallback(const ServerCutTextCallback& cb);

	rfbProtocol(std::unique_ptr<ICommunication>&& comm);
	rfbProtocol(const rfbProtocol&) = delete;
	rfbProtocol(rfbProtocol&&);

	// not needed
	rfbProtocol& operator=(const rfbProtocol&) = delete;
	rfbProtocol& operator=(rfbProtocol&&) = delete;

	~rfbProtocol();

	bool Init();

	void SetPixelFormat();
	void SetEncoding();
	void FrameBufferUpdateRequest();
	void KeyEvent();
	void PointerEvent();
	void ClientCutText();

private:

	frameBufferUpdateCallback m_frameBufferUpdateCallback;
	setColourMapentriesCallback m_setColourMapentrciesCallback;
	bellCallback m_bellCallback;
	ServerCutTextCallback m_ServerCutTextCallback;
	std::unique_ptr<ICommunication> m_comm;
};

#endif // REMOTE_FRAME_BUFFER_PROTOCOL
