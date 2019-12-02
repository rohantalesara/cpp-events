#include "MyTimedStream.h"
#include <cstring>

#define DEFAULT_TIMER_INTERVAL 1000

MyTimedStream::MyTimedStream()
	: m_isRunning(false), m_millisecondsInterval(DEFAULT_TIMER_INTERVAL)
{
}


MyTimedStream::~MyTimedStream()
{
}

bool MyTimedStream::Start(unsigned int millisecondsInterval)
{
	if (m_isRunning)
	{
		return false;
	}

	m_isRunning = true;
	m_millisecondsInterval = millisecondsInterval > 0 ? millisecondsInterval : DEFAULT_TIMER_INTERVAL;
	m_timerThread = std::thread([this]() { TimedStreamFunc(); });

	return true;
}

bool MyTimedStream::Stop()
{
	if (!m_isRunning)
	{
		return false;
	}

	m_isRunning = false;

	if (m_timerThread.joinable())
	{
		m_timerThread.join();
	}

	return true;
}

void MyTimedStream::TimedStreamFunc()
{
	while (m_isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(m_millisecondsInterval));


		if (m_isRunning)
		{
			Tick(alphabet[rand() % MAX]);
		}
	}
}
