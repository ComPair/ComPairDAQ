#ifndef COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
#define COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX

#include <cstdint>
#include <vector>

class ComPairSubsystemEventPacket {
	public:
		virtual bool ParseData(const std::vector< uint16_t > &data) = 0;

	protected:
		uint32_t evtId;
		bool trigger_ack;
        uint16_t packet_header;
        uint16_t packet_type;
        uint16_t crc;
};

/// A trivial packet structure for testing packet system.
/**  The trivial packet is 8 bytes long, and contains all required fields:
 *     * Packet header
 *     * Packet type
 *     * Trigger Ack
 *     * Event ID
 *     * CRC
 *
 *   Trivial subsystem packets do not include any data.
 */
class TrivialSubsystemEventPacket : ComPairSubsystemEventPacket {
    public:
		bool ParseData(const std::vector< uint16_t > &data); 
        uint32_t get_event_id() { return evtId; };
		bool get_trigger_ack() { return trigger_ack; };
        uint16_t get_packet_header() { return packet_header; };
        uint16_t get_packet_type() { return packet_type; };
        uint16_t get_crc() { return crc; };
};

#endif // COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
