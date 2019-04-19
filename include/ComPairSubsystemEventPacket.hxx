#ifndef COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
#define COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX

#include <cstdint>
#include <vector>

/// ComPairSubsystemEventPacket: abstract base class for subsystem packet.
/**    All sub-systems should define their own subclass of `ComPairSubsystemEventPacket`
 *     to deal with the unique sets of data being passed around.
 */
class ComPairSubsystemEventPacket {
	public:
		virtual bool parse(std::vector< uint8_t > &data) = 0;
		bool parse_uint16(std::vector<uint16_t> &data);
        void parse_header(std::vector<uint8_t> &data);
        uint32_t event_id() { return event_id_; };
		bool trigger_ack() { return trigger_ack_; };
        uint16_t packet_header() { return packet_header_; };
        uint16_t packet_type() { return packet_type_; };
        uint16_t crc() { return crc_; };
	protected:
		uint32_t event_id_;
		bool trigger_ack_;
        uint16_t packet_header_;
        uint16_t packet_type_;
        uint16_t crc_;
};

#endif // COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
