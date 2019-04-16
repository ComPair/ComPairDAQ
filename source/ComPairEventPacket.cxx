#include <stdexcept>
#include "ComPairEventPacket.hxx"

#include <iostream>

// Parse raw data buffer at `buf`.
bool ComPairEventPacket::ParseData(const std::vector<uint16_t> &buf) {
    uint16_t check_size;
    packet_size = buf.at(0);
    // Perform checks on packet size:
    // Packet size must be even:
    if ((packet_size % 2) == 1)
        throw std::runtime_error("Packet size must be even");
    // Packet size is even.. change to number of uint16 elements:
    packet_size = packet_size / 2;

    // Packet size must not be too long:
    if (packet_size > buf.size())
        throw std::runtime_error("Packet size is too long");

    check_size = buf.at(packet_size-1);
    // Check that packet ends with the same event size:
    if (check_size != 2*packet_size)
        throw std::runtime_error("Packet size at head and tail do not match");

    // Check that packet at least includes a header:
    if (packet_size < 7)
        throw std::runtime_error("Packet size is too small");
    
    uint16_t raw_source = buf.at(1);
    raw_source &= ~(1 << 15); // Clear packet-type bit.
    source = static_cast<PacketSource>(raw_source);
    // Must be a better way to do this:
    seconds = ((uint32_t)buf.at(2) << 16) | (uint32_t)buf.at(3);
    useconds = ((uint32_t)buf.at(4) << 16) | (uint32_t)buf.at(5);

    // Process the subsystem event packet.
    //subsys_iter = auto buf.begin() + 4;
    //CompairSubsystemEventPacket subsys_event_packet;
    //subsys_event_packet.ParseData(&subsys_iter);

    // Deal with the packet attribute:
    // Determine if this is event or monitor packet:
    if ( buf.at(1) & (1<<15) ) {
        // This is an event packet
        packet_type = PacketType::EVENT_PACKET;
    } else {
        packet_type = PacketType::MONITOR_PACKET;
    }
    
    return true;
}

        
// vim: set ts=4 sw=4 sts=4 et:
