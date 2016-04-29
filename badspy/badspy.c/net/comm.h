#pragma once
#include "socket.h"

// sending frame:
//		header: type[1] length[4]
//		content: bytes[?] --> number of bytes is header.length
// receiving flag: sometime, when we send some data to another then we need
//				   to know response from them about this data such as ignored, accepted...
//		flag: 1 byte
class SPYAPI Comm 
{
private:
	Socket * socket;
protected:
	void send_header(byte content_type, int content_length) const;
	void send_content(const byte * buffer, int offset, int length) const;
	void send_flag(byte flag) const;
	void receive_header(byte * content_type, int * content_length) const;
	int receive_content(byte * buffer, int offset, int length) const;
	void receive_nbytes(byte * buffer, int offset, int length) const;
	int receive_flag() const;
	in_addr get_local_address() const;
public:
	Comm(const char * server_addr, int server_port);
	virtual ~Comm();
};