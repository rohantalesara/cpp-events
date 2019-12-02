#include "EventsPublisher.h"
#include <iostream>

EventsPublisher::EventsPublisher()
{
	m_timer.Tick += [this](char c) {
		std::cout << "Emitting " << c << std::endl;
		ReceiveCharacter(c);
	};
}

EventsPublisher::~EventsPublisher()
{
}

void EventsPublisher::Start()
{
	m_timer.Start(1000); // 2 seconds interval
}

void EventsPublisher::Stop()
{
	m_timer.Stop();
}

