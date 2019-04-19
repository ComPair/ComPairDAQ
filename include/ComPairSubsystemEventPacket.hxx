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

/// Packet for holding ideal data for a subsystem event.
/**  Simulation Subsystem Packets contain the x, y, z, and E for each hit.
 *   Each packet can contain many hits.
 *   The raw data following the packet header should be packed as follows:
 *   N, x[0], y[0], z[0], E[0],..., x[N-1], y[N-1], z[N-1], E[N-1]
 *   N is uint16, , all others are 32 bit floats.
 */
class SimSubsystemEventPacket : public ComPairSubsystemEventPacket {
    public:
        bool parse(std::vector<uint8_t> &data);
        void set_event_data(const std::vector<float> &x, const std::vector<float> &y,
                            const std::vector<float> &z, const std::vector<float> &E);
        uint16_t nhit() { return nhit_; };
        float hit_x(int i) { return hit_x_.at(i); };
        float hit_y(int i) { return hit_y_.at(i); };
        float hit_z(int i) { return hit_z_.at(i); };
        float hit_E(int i) { return hit_E_.at(i); };
    private:
        uint16_t nhit_;
        std::vector<float> hit_x_;
        std::vector<float> hit_y_;
        std::vector<float> hit_z_;
        std::vector<float> hit_E_;
};

#endif // COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
