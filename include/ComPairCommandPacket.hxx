#ifndef COMPAIR_COMMAND_PACKET_HXX
#define COMPAIR_COMMAND_PACKET_HXX

#include <cstdint>
#include <vector>

class ComPairCommandPacket {
	public:
		void ParseData(std::vector< uint8_t > &data);

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
		bool HasData() {return hasData_;}
		void SetDestination(Destination dest) {destination_ = dest;}

	private:
		Destination destination_;
		uint8_t command_;
		uint8_t address_;
		short siTracker_;
		bool hasData_;
		uint16_t data_;

		void ParseDestination(const uint8_t &firstWord, const uint8_t &secondWord);
		void ParseType(const uint8_t &firstWord);

};

#endif // COMPAIR_COMMAND_PACKET_HXX
