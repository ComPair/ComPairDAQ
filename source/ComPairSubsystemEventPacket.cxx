#include "ComPairSubsystemEventPacket.hxx"

#include <stdexcept>

#include <iostream>

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


// Parse input raw data
bool TrivialSubsystemEventPacket::parse(std::vector<uint8_t> &data) {
    // Expect data to only be 8 bytes long...
    if (data.size() != 8) 
        throw std::runtime_error("Expect trivial subsystem packet to be 8 bytes long");
    this->parse_header(data);
    return true;
}

bool SimSubsystemEventPacket::parse(std::vector<uint8_t> &data) {
    // Make sure there is enough data:
    if (data.size() < 10) {
        throw std::runtime_error("Packet length does not contain enough data to parse.");
    }
    uint16_t *pdata = reinterpret_cast<uint16_t*>(data.data()); // sim packet is all uint16

    this->parse_header(data);
    nhit_ = pdata[3];
    if ((size_t)16*nhit_ + 10 != data.size()) {
        throw std::runtime_error("Packet length is not correct for given number of hits");
    }
    hit_x_.resize(nhit_);
    hit_y_.resize(nhit_);
    hit_z_.resize(nhit_);
    hit_E_.resize(nhit_);
    float *hit_data = reinterpret_cast<float*>(pdata + 4); // hit information all float's
    int indx = 0;
    for (int ihit=0; ihit<nhit_; ihit++) {
        hit_x_.at(ihit) = hit_data[indx++];
        hit_y_.at(ihit) = hit_data[indx++];
        hit_z_.at(ihit) = hit_data[indx++];
        hit_E_.at(ihit) = hit_data[indx++];
    }
     
    return true;
}

void SimSubsystemEventPacket::set_event_data(const std::vector<float> &x, const std::vector<float> &y,
        const std::vector<float> &z, const std::vector<float> &E) {
    // Note that this does not set any of the header information!
    nhit_ = x.size();
    if (y.size() != nhit_ || z.size() != nhit_ || E.size() != nhit_) {
        throw std::runtime_error("Input data vectors are of different sizes");
    }
    // I am pretty sure assignment resizes and copies data?
    hit_x_ = x;
    hit_y_ = y;
    hit_z_ = z;
    hit_E_ = E;
}

