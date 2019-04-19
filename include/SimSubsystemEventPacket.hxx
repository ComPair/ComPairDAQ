#ifndef SIM_SUBSYSTEM_EVENT_PACKET_HXX
#define SIM_SUBSYSTEM_EVENT_PACKET_HXX

#include "ComPairSubsystemEventPacket.hxx"

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

#endif // SIM_SUBSYSTEM_EVENT_PACKET_HXX
