#ifndef COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
#define COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX

#include <cstdint>
#include <vector>

/// Helper function: concatenates a pair of uint16's into a single uint32.
/**     ms_half: the most-significant half.
 *      ls_half: the least significant half.
 */
inline uint32_t uint16_pair_to_uint32(uint16_t ms_half, uint16_t ls_half);

/// ComPairSubsystemEventPacket: abstract base class for subsystem packet.
/**    All sub-systems should define their own subclass of `ComPairSubsystemEventPacket`
 *     to deal with the unique sets of data being passed around.
 */
class ComPairSubsystemEventPacket {
	public:
		virtual bool ParseData(const std::vector< uint16_t > &data) = 0;
		//virtual uint32_t get_packet_nbytes() = 0;
        void ParseHeader(const std::vector<uint16_t> &data);
        uint32_t get_event_id() { return evtId; };
		bool get_trigger_ack() { return trigger_ack; };
        uint16_t get_packet_header() { return packet_header; };
        uint16_t get_packet_type() { return packet_type; };
        uint16_t get_crc() { return crc; };
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
class TrivialSubsystemEventPacket : public ComPairSubsystemEventPacket {
    public:
		bool ParseData(const std::vector< uint16_t > &data); 
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
        bool ParseData(const std::vector<uint16_t> &data);
        void set_event_data(const std::vector<float> &x, const std::vector<float> &y,
                            const std::vector<float> &z, const std::vector<float> &E);
        uint16_t get_nhit() { return nhit; };
        float get_hit_x(int i) { return hit_x.at(i); };
        float get_hit_y(int i) { return hit_y.at(i); };
        float get_hit_z(int i) { return hit_z.at(i); };
        float get_hit_E(int i) { return hit_E.at(i); };
    private:
        uint16_t nhit;
        std::vector<float> hit_x;
        std::vector<float> hit_y;
        std::vector<float> hit_z;
        std::vector<float> hit_E;
};

#endif // COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
