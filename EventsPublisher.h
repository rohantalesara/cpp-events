#ifndef __EVENTSPUBLISHER_H__
#define __EVENTSPUBLISHER_H__

#include "eventlib.h"
#include "MyTimedStream.h"

class EventsPublisher
{
public:
	EventsPublisher();
	virtual ~EventsPublisher();

	void Start();
	void Stop();

	event<char> ReceiveCharacter;

	// event of <message>.
	event<const std::string&> DisplayDetails;

private:
	MyTimedStream timer;
	unsigned int counter;
};

#endif // __EVENTSPUBLISHER_H__
