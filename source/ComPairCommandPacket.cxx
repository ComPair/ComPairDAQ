#include "ComPairCommandPacket.hxx"

#include <stdexcept>

bool ComPairCommandPacket::ParseData(std::vector< uint8_t > &data) {
	//Parse the first two words for the destination
	destination_ = ParseDestination(data.at(0), data.at(1));

	return true;
}

ComPairCommandPacket::Destination ComPairCommandPacket::ParseDestination(uint8_t firstWord, uint8_t secondWord) {
	// The MSB of the first word should be 0
	if (firstWord & 0x80) {
		throw std::runtime_error("ERROR");
	}

	typedef ComPairCommandPacket::Destination Destination;

	Destination dest = Destination::UNKNOWN;

	// Mask first word to get the subsytem type
	uint8_t type = firstWord & 0x7C;

	dest = static_cast<Destination>(type);

	return dest;

}
