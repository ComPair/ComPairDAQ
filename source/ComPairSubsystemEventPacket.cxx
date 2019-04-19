#include "ComPairSubsystemEventPacket.hxx"
#include <stdexcept>

/// Parse the data packet header and set attributes.
void ComPairSubsystemEventPacket::parse_header(std::vector<uint8_t> &data) {
    uint8_t *pdata8 = data.data();
    uint16_t *pdata16 = reinterpret_cast<uint16_t*>(pdata8);
    uint16_t data0 = pdata16[0];
    uint32_t data1 = *reinterpret_cast<uint32_t*>(pdata16 + 1);
    // Unpack data0
    packet_type_ = data0 & 0x000F;
    packet_header_ = data0 >> 4 & 0x0FFF;
    trigger_ack_ = ((data1 >> 31) & 0x1) == 1;
    event_id_= data1 & 0x7FFFFFFF;
    // Get checksum
    crc_ = *reinterpret_cast<uint16_t*>(pdata8 + data.size() - 2);
    // Some day we need to check the checksum here:
}

bool ComPairSubsystemEventPacket::parse_uint16(std::vector<uint16_t> &data) {
    uint8_t *pdata = reinterpret_cast<uint8_t*>(data.data());
    std::vector<uint8_t> data8(pdata, pdata + 2*data.size());
    return this->parse(data8);
}


