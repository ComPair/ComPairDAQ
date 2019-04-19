#include <stdexcept>
#include "ComPairEventPacket.hxx"

// Parse raw data buffer at `buf`.
bool ComPairEventPacket::parse(std::vector<uint8_t> &data) {
    uint8_t *pdata8 = data.data();
    uint16_t *pdata16 = reinterpret_cast<uint16_t*>(pdata8);
    uint16_t check_size;
    packet_size_ = *pdata16;

    // Perform checks on packet size:
    // Packet size must not be too long:
    if (packet_size_ != data.size())
        throw std::runtime_error("Indicated packet size does not match true packet size");

    check_size = *reinterpret_cast<uint16_t*>(pdata8 + packet_size_ - 2);
    // Check that packet ends with the same event size:
    if (check_size != packet_size_)
        throw std::runtime_error("Packet size at head and tail do not match.");

    // Check that packet at least includes a header and the final size:
    if (packet_size_ < 14)
        throw std::runtime_error("Packet size is too small to parse.");
    
    uint16_t raw_source = pdata16[1];
    raw_source &= ~(1 << 15); // Clear packet-type bit.
    source_ = static_cast<PacketSource>(raw_source);
    // Must be a better way to do this:
    seconds_ = ((uint32_t)pdata16[2] << 16) | (uint32_t)pdata16[3];
    useconds_ = ((uint32_t)pdata16[4] << 16) | (uint32_t)pdata16[5];

    // Process the subsystem event packet.
    //subsys_iter = auto buf.begin() + 4;
    //CompairSubsystemEventPacket subsys_event_packet;
    //subsys_event_packet.ParseData(&subsys_iter);
    //
    std::vector<uint8_t> subsystemBuf (data.begin() + 12, data.begin() + packet_size_ - 2);
    switch(source_) {
        case(ComPairEventPacket::PacketSource::SIM):
            subsystem_packet_= new SimSubsystemEventPacket();
            subsystem_packet_->parse(subsystemBuf);
            break;
        case(ComPairEventPacket::PacketSource::SI0):
            break;
        default:
            //throw std::runtime_error("Unknown packet source.");
            break;
    }

    // Deal with the packet attribute:
    // Determine if this is event or monitor packet:
    if ( pdata16[1] & (1<<15) ) {
        // This is an event packet
        packet_type_ = PacketType::EVENT;
    } else {
        packet_type_ = PacketType::MONITOR;
    }
    
    return true;
}

// Parse data that comes in as uint16.
// Likely a temporary crutch until all testing is updated test with uint8.
bool ComPairEventPacket::parse(std::vector<uint16_t> &data) {
    uint8_t *pdata = reinterpret_cast<uint8_t*>(data.data());
    std::vector<uint8_t> data8(pdata, pdata + 2*data.size());
    return this->parse(data8);
}

        
// vim: set ts=4 sw=4 sts=4 et:
