#ifndef COMPAIR_EVENT_PACKET_HXX
#define COMPAIR_EVENT_PACKET_HXX

#include <cstdint>
#include <vector>
#include "ComPairSubsystemEventPacket.hxx"

class ComPairEventPacket {
	public:
        enum class PacketType {
            EVENT = true,
            MONITOR = false
        }; 
        enum class PacketSource {
            SI0 = 0x0001 | 0x0800,
            SI1 = 0x0002 | 0x0800,
            SI2 = 0x0004 | 0x0800,
            SI3 = 0x0008 | 0x0800,
            SI4 = 0x0010 | 0x0800,
            SI5 = 0x0020 | 0x0800,
            SI6 = 0x0040 | 0x0800,
            SI7 = 0x0080 | 0x0800,
            SI8 = 0x0100 | 0x0800,
            SI9 = 0x0200 | 0x0800,
            ACD = 0x0400,
            SI  = 0x0800,
            CZT = 0x1000,
            CSI = 0x2000,
            TRG = 0x4000,
            SIM = 0x000F
        };
        bool parse(std::vector<uint8_t> &data);
        bool parse(std::vector<uint16_t> &data);
        PacketType packet_type() { return packet_type_; };
        PacketSource packet_source() { return source_; };
        uint32_t seconds() { return seconds_; };
        uint32_t useconds() { return useconds_; };
        ComPairSubsystemEventPacket* subsystem_packet() { return subsystem_packet_; };

	private:
        uint16_t packet_size_;   // Packet size in bytes.
        PacketType packet_type_; // 
        PacketSource source_;    // From packet attribute. Value is from `PacketSource` enum.
        uint32_t seconds_;
        uint32_t useconds_;
        ComPairSubsystemEventPacket *subsystem_packet_;
};

#endif
