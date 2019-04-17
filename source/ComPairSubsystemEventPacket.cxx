#include "ComPairSubsystemEventPacket.hxx"

#include <stdexcept>

// Create and fill with random data
//bool TrivialSubsystemEventPacket::random() {
//    
//}

// Parse input raw data
bool TrivialSubsystemEventPacket::ParseData(const std::vector<uint16_t> &data) {
    // Expect data to only be 8 bytes long...
    if (data.size() != 4) 
        throw std::runtime_error("Expect trivial subsystem packet to by 8 bytes long");
    uint16_t data0 = data.at(0);
    uint32_t data1 = ((uint32_t)data.at(1) << 16) | (uint32_t)data.at(2);
    // Unpack data0
    packet_type = data0 & 0x000F;
    packet_header = data0 >> 4 & 0x0FFF;
    // Unpack data1
    trigger_ack = ((data1 >> 31) & 0x1) == 1;
    evtId = data1 & 0x7FFFFFFF;
    // Get checksum
    crc = data.at(3);

    return true;
}
