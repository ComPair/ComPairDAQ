#ifndef TRIVIAL_SUBSYSTEM_EVENT_PACKET_HXX
#define TRIVIAL_SUBSYSTEM_EVENT_PACKET_HXX

#include "ComPairSubsystemEventPacket.hxx"

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
class TrivialSubsystemEventPacket : public ComPairSubsystemEventPacket {
    public:
		bool parse(std::vector< uint8_t > &data); 
};

#endif // TRIVIAL_SUBSYSTEM_EVENT_PACKET_HXX
