#include "ComPairCommandPacket.hxx"

#include <stdexcept>

void ComPairCommandPacket::Parse(std::vector< uint8_t > &packet) {
	//Parse whether this command has packet.
	ParseType(packet.at(0));

	if (hasData_) {
		if (packet.size() != 6) {
			throw std::runtime_error("Invalid length command packet with data.");
		}
		//Parse the command data.
		data_ = (packet.at(4) << 8) | packet.at(5);
	}
	else {
		if (packet.size() != 4) {
			throw std::runtime_error("Invalid length command packet without data.");
		}
	}

	//Parse the first two words for the destination
	ParseDestination(packet.at(0), packet.at(1));

	command_ = packet.at(2);
	address_ = packet.at(3);
}

/** Parse the command packet type based on the MSB from the first word.
 * \param[in] firstWord The first word from the command packet.
 */
void ComPairCommandPacket::ParseType(const uint8_t &firstWord) {
	hasData_ = (firstWord & 0x80) >> 7;
}

void ComPairCommandPacket::ParseDestination(const uint8_t &firstWord, const uint8_t &secondWord) {
	typedef ComPairCommandPacket::Destination Destination;

	destination_ = Destination::UNKNOWN;

	// Mask first word to get the subsystem type
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

	// Check if the si tracker index flag is set for si tracker type packets.
	if (destination_ == Destination::SI_TR && siTracker_ == -1) {
		throw std::runtime_error("Invalid type in command packet, si tracker type found with invalid SiTracker layer information.");
	}

}

void ComPairCommandPacket::SetDestination(
		ComPairCommandPacket::Destination dest,
		short siTracker /* = -1 */)
{
	if (dest != ComPairCommandPacket::Destination::SI_TR
		 && siTracker >= 0) {
		throw std::runtime_error("Command Packet Destination Si Tracker can not be set when the destination is not the Si Tracker!");
	}
	if (dest == ComPairCommandPacket::Destination::SI_TR
		 && (siTracker < 0 || siTracker > 9)) {
		throw std::runtime_error("Command Packer Si Tracker Destination is invalid!");
	}

	destination_ = dest;
	siTracker_ = siTracker;
}

void ComPairCommandPacket::SetData(uint16_t data) {
	hasData_ = true;
	data_ = data;
}

/** Create a vector of 8 bit words representing the packet.
 */
std::vector< uint8_t > ComPairCommandPacket::GetPacket() {
	std::vector< uint8_t > packet(4, 0);

	// Set the destination
	packet.at(0) = static_cast< uint8_t > (destination_);
	// Set the Si Tracker bits
	if (siTracker_ < 8) {
		packet.at(1) = 0x1 << siTracker_;
	}
	else {
		packet.at(0) |= 0x1 << (siTracker_ - 8);
	}

	// Set the command and address.
	packet.at(2) = command_;
	packet.at(3) = address_;

	// If this command has data set the data flag bit and the data bits.
	if (hasData_) {
		packet.push_back(data_ & 0xFF);
		packet.push_back((data_ >> 16) & 0xFF);
		packet.at(0) |= 0x80;
	}

	return packet;
}

uint16_t ComPairCommandPacket::GetData() {
	if ( ! hasData_) {
		throw std::runtime_error("Requested data from packet with no data!");
	}

	return data_;
}
