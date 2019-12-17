#include "MyTimedStream.h"

#define DEFAULT_TIMER_INTERVAL 1000

MyTimedStream::MyTimedStream()
	: isRunning(false), millisecondsInterval(DEFAULT_TIMER_INTERVAL)
{
}


MyTimedStream::~MyTimedStream()
{
}

bool MyTimedStream::Start(unsigned int millisecondsInterval)
{
	if (isRunning)
	{
		return false;
	}

	isRunning = true;
	millisecondsInterval = millisecondsInterval > 0 ? millisecondsInterval : DEFAULT_TIMER_INTERVAL;
	timerThread = std::thread([this]() { TimedStreamFunc(); });

	return true;
}

bool MyTimedStream::Stop()
{
	if (!isRunning)
	{
		return false;
	}

	isRunning = false;

	if (timerThread.joinable())
	{
		timerThread.join();
	}

	return true;
}

void MyTimedStream::TimedStreamFunc()
{
	while (isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsInterval));


		if (isRunning)
		{
			Tick(alphabet[rand() % MAX]);
		}
	}
}
