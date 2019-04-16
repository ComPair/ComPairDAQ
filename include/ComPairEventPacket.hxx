#ifndef COMPAIR_EVENT_PACKET_HXX
#define COMPAIR_EVENT_PACKET_HXX

#include <cstdint>
#include <vector>
#include "ComPairSubsystemEventPacket.hxx"

class ComPairEventPacket {
	public:
        enum class PacketType {
            EVENT_PACKET = true,
            MONITOR_PACKET = false
        }; 
        enum class PacketSource {
            Si0_SOURCE = 0x0001 | 0x0800,
            Si1_SOURCE = 0x0002 | 0x0800,
            Si2_SOURCE = 0x0004 | 0x0800,
            Si3_SOURCE = 0x0008 | 0x0800,
            Si4_SOURCE = 0x0010 | 0x0800,
            Si5_SOURCE = 0x0020 | 0x0800,
            Si6_SOURCE = 0x0040 | 0x0800,
            Si7_SOURCE = 0x0080 | 0x0800,
            Si8_SOURCE = 0x0100 | 0x0800,
            Si9_SOURCE = 0x0200 | 0x0800,
            ACD_SOURCE = 0x0400,
            Si_SOURCE  = 0x0800,
            CZT_SOURCE = 0x1000,
            CsI_SOURCE = 0x2000,
            TRG_SOURCE = 0x4000
        };
		bool ParseData(const std::vector<uint16_t> &data);
        PacketType get_type() { return packet_type; };
        PacketSource get_source() { return source; };
        uint32_t get_seconds() { return seconds; };
        uint32_t get_useconds() { return useconds; };
	private:
        uint16_t packet_size; // Packet size in bytes.
        PacketType packet_type;
        PacketSource source;  // From packet attribute. 
        uint32_t seconds;
        uint32_t useconds;
        //ComPairSubsystemEventPacket event_packet;
};

#endif

// vim: set ts=4 sw=4 sts=4 et:
