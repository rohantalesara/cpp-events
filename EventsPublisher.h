#ifndef __EVENTSPUBLISHER_H__
#define __EVENTSPUBLISHER_H__

#include "szevent.h"
#include "MyTimedStream.h"

class EventsPublisher
{
public:
	EventsPublisher();
	virtual ~EventsPublisher();

	void Start();
	void Stop();

	sz::event<char> ReceiveCharacter;

	// event of <message>.
	sz::event<const std::string&> DisplayDetails;

private:
	MyTimedStream m_timer;
	unsigned int m_counter;
};

#endif // __EVENTSPUBLISHER_H__
