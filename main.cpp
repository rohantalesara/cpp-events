#include <mutex>
#include <iostream>
#include<cctype>
#include "eventlib.h"
#include "EventsPublisher.h"

int main()
{
	EventsPublisher ep;
	std::mutex printLocker;

	std::cout << "Press <Enter> to stop."<< std::endl;

	event_handler<char> streamHandler1([&ep](char incomingChar) {
		if ( isupper(incomingChar))
		{
			ep.DisplayDetails.call_async("Detected uppercase");
		}
	});

	event_handler<char> streamHandler2([&ep](char incomingChar) {
		if ( incomingChar == 'A' || incomingChar == 'E' || incomingChar == 'I' || incomingChar == 'O' || incomingChar == 'U')
		{
			std::cout<<incomingChar<<std::endl;
			ep.DisplayDetails.call_async("Detected vowel");
		}
	});

	auto displayDetailsHandlerId = ep.DisplayDetails.add(
		[&printLocker](const std::string& message) {
		std::lock_guard<std::mutex> lock(printLocker);

		std::cout << "Details Event- Message: "
			<< message.c_str()
			<< std::endl;
	});

	ep.ReceiveCharacter.add(streamHandler1);
	ep.ReceiveCharacter.add(streamHandler2);
	ep.Start();

	getchar();

	ep.DisplayDetails.remove_id(displayDetailsHandlerId);

	ep.ReceiveCharacter.remove(streamHandler1);
	ep.ReceiveCharacter.remove(streamHandler2);
	ep.Stop();

	return 0;
}
