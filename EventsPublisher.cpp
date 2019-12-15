#include "EventsPublisher.h"
#include <iostream>

EventsPublisher::EventsPublisher()
{
	timer.Tick.add([this](char c) {
		std::cout << "Emitting " << c << std::endl;
		ReceiveCharacter(c);
	});
}

EventsPublisher::~EventsPublisher()
{
}

void EventsPublisher::Start()
{
	timer.Start(1000); // 1 second interval
}

void EventsPublisher::Stop()
{
	timer.Stop();
}

