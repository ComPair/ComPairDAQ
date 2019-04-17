#ifndef COMPAIR_COMMAND_PACKET_HXX
#define COMPAIR_COMMAND_PACKET_HXX

#include <cstdint>
#include <vector>

/// Command packet for ComPair.
/** The packet indicates a flag whether it has data, the command destination,
 *  the command, and the address.
 */
class ComPairCommandPacket {
	public:
		// Default constructor
		ComPairCommandPacket() : hasData_(false) {}

		// Parses a command packet.
		void Parse(std::vector< uint8_t > &packet);

		/// Command destinations mapped to their respective values.
		enum class Destination {
			UNKNOWN = 0x0,
			ACD = 0x4,
			SI_TR = 0x8,
			CZT = 0x10,
			CSI = 0x20,
			TRIG = 0x40,
		};

		/// Returns the command destination.
		Destination GetDestination() {return destination_;}
		/// Returns the Si Tracker associated to this command.
		short GetSiTracker() {return siTracker_;}
		/// Returns the command for this command packet.
		uint8_t GetCommand() {return command_;}
		/// Returns the address for this command packet.
		uint8_t GetAddress() {return address_;}
		/// Returns whether the command has data associated with it.
		bool HasData() {return hasData_;}
		/// Returns the data value.
		uint16_t GetData();

		/// Sets the command destination.
		void SetDestination(Destination dest, short siTracker_=-1);
		/// Set the data value.
		void SetData(uint16_t data);
		/// Set the command and address.
		void SetCommand(uint8_t command, uint8_t address) {
			command_ = command;
			address_ = address;
		}

		/// Get the raw data packet.
		std::vector< uint8_t > GetPacket();

	private:
		/// Flag indicating data.
		bool hasData_;

		/// Command destination.
		Destination destination_;
		/// Si Tracker for command.
		short siTracker_;

		/// Command value.
		uint8_t command_;
		/// Address value.
		uint8_t address_;

		/// Data value.
		uint16_t data_;

		/// Parse the destination of the command.
		void ParseDestination(const uint8_t &firstWord, const uint8_t &secondWord);
		/// Parse the type of command (with or without data).
		void ParseType(const uint8_t &firstWord);

};

#endif // COMPAIR_COMMAND_PACKET_HXX
