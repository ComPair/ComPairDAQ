#include "ComPairCommandPacket.hxx"

#include <stdexcept>

bool ComPairCommandPacket::ParseData(std::vector< uint8_t > &data) {
	//Parse the first two words for the destination
	ParseDestination(data.at(0), data.at(1));
	command_ = data.at(2);
	address_ = data.at(3);

	return true;
}

void ComPairCommandPacket::ParseDestination(uint8_t firstWord, uint8_t secondWord) {
	typedef ComPairCommandPacket::Destination Destination;

	destination_ = Destination::UNKNOWN;

	// Mask first word to get the subsytem type
	destination_ = static_cast< Destination >(firstWord & 0x7C);

	siTracker_ = -1;
	uint16_t siTrackerFlags = ((firstWord & 0x3) << 8) | (secondWord);
	switch(siTrackerFlags) {
		case(0x1): siTracker_ = 0; break;
		case(0x1 << 1): siTracker_ = 1; break;
		case(0x1 << 2): siTracker_ = 2; break;
		case(0x1 << 3): siTracker_ = 3; break;
		case(0x1 << 4): siTracker_ = 4; break;
		case(0x1 << 5): siTracker_ = 5; break;
		case(0x1 << 6): siTracker_ = 6; break;
		case(0x1 << 7): siTracker_ = 7; break;
		case(0x1 << 8): siTracker_ = 8; break;
		case(0x1 << 9): siTracker_ = 9; break;
	}

	// Check if si tracker layer is set when the type is from another subsystem.
	if (destination_ != Destination::SI_TR && siTracker_ != -1) {
		throw std::runtime_error("Invalid type in command packet, type is not a Si Tracker, but a Si Tracker bit is set.");
	}

	// The MSB of the first word should be 0
	if (firstWord & 0x80) {
		throw std::runtime_error("Invalid type in command packet, MSB is set.");
	}
}
