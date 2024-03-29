#ifndef __MYTIMEDSTREAM_H__
#define __MYTIMEDSTREAM_H__

#include "eventlib.h"
#include <thread>

class MyTimedStream
{
public:
	MyTimedStream();
	~MyTimedStream();

	event<char> Tick;

	bool Start(unsigned int millisecondsInterval);
	bool Stop();

private:
	int MAX = 31;
	void TimedStreamFunc();
	bool isRunning;
	unsigned int millisecondsInterval;
	std::thread timerThread;
	char alphabet[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZaeiou"; 
};

#endif // __MYTIMEDSTREAM_H__
