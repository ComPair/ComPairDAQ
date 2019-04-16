#ifndef COMPAIR_COMMAND_PACKET_HXX
#define COMPAIR_COMMAND_PACKET_HXX

#include <cstdint>
#include <vector>

class ComPairCommandPacket {
	public:
		bool ParseData(std::vector< uint8_t > &data);

		enum class Destination {
			UNKNOWN = 0x0,
			ACD = 0x4,
			SI_TR = 0x8,
			CZT = 0x10,
			CSI = 0x20,
			TRIG = 0x40,
		};

		Destination GetDestination() {return destination_;}
		short GetSiTracker() {return siTracker_;}
		void SetDestination(Destination dest) {destination_ = dest;}

	private:
		Destination destination_;
		uint8_t command_;
		uint8_t address_;
		short siTracker_;

		void ParseDestination(uint8_t firstWord, uint8_t secondWord);

};

#endif // COMPAIR_COMMAND_PACKET_HXX
