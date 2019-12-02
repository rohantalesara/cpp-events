#ifndef __MYTIMEDSTREAM_H__
#define __MYTIMEDSTREAM_H__

#include "szevent.h"
#include <thread>

class MyTimedStream
{
public:
	MyTimedStream();
	~MyTimedStream();

	sz::event<char> Tick;

	bool Start(unsigned int millisecondsInterval);
	bool Stop();

private:
	int MAX = 26;
	void TimedStreamFunc();
	bool m_isRunning;
	unsigned int m_millisecondsInterval;
	std::thread m_timerThread;
	char alphabet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
};

#endif // __MYTIMEDSTREAM_H__
