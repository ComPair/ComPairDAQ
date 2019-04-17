#include "ComPairSubsystemEventPacket.hxx"

#include <stdexcept>

/// Parse the data packet header and set attributes.
bool ComPairSubsystemEventPacket::ParseHeader(const std::vector<uint16_t> &data) {
    uint16_t data0 = data.at(0);
    uint32_t data1 = uint16_pair_to_uint32(data.at(1), data.at(2));
    // Unpack data0
    packet_type = data0 & 0x000F;
    packet_header = data0 >> 4 & 0x0FFF;
    // Unpack data1
    trigger_ack = ((data1 >> 31) & 0x1) == 1;
    evtId = data1 & 0x7FFFFFFF;
    // Get checksum
    crc = data.at(data.size()-1);
    // Some day we need to check the checksum???
}

// Parse input raw data
bool TrivialSubsystemEventPacket::ParseData(const std::vector<uint16_t> &data) {
    // Expect data to only be 8 bytes long...
    if (data.size() != 4) 
        throw std::runtime_error("Expect trivial subsystem packet to by 8 bytes long");
    this->ParseHeader(data);
    return true;
}

bool SimSubsystemEventPacket::ParseData(const std::vector<uint16_t> &data) {
    this->ParseHeader(data);
    nhit = data.at(3);
    if (8*nhit + 5 != data.size()) {
        throw std::runtime_error("Packet length is not correct for given number of hits");
    }
    hit_x.resize(nhit);
    hit_y.resize(nhit);
    hit_z.resize(nhit);
    hit_E.resize(nhit);
    uint32_t tmp;
    float *fp = reinterpret_cast<float *>(&tmp);;
    int indx = 4;
    for (int ihit=0; ihit<nhit; ihit++) {
        tmp = uint16_pair_to_uint32(data.at(indx), data.at(indx+1));
        hit_x.at(ihit) = *fp;
        indx += 2;
        tmp = uint16_pair_to_uint32(data.at(indx), data.at(indx+1));
        hit_y.at(ihit) = *fp;
        indx += 2;
        tmp = uint16_pair_to_uint32(data.at(indx), data.at(indx+1));
        hit_z.at(ihit) = *fp;
        indx += 2;
        tmp = uint16_pair_to_uint32(data.at(indx), data.at(indx+1));
        hit_E.at(ihit) = *fp;
    }
     
    return true;
}

void SimSubsystemEventPacket::set_event_data(const std::vector<float> &x, const std::vector<float> &y,
        const std::vector<float> &z, const std::vector<float> &E) {
    // Note that this does not set any of the header information!
    nhit = x.size();
    if (y.size() != nhit || z.size() != nhit || E.size() != nhit) {
        throw std::runtime_error("Input data vectors are of different sizes");
    }
    // I am pretty sure assignment resizes and copies data?
    hit_x = x;
    hit_y = y;
    hit_z = z;
    hit_E = E;
}

inline uint32_t uint16_pair_to_uint32(uint16_t ms_half, uint16_t ls_half) {
    return ((uint32_t)ms_half << 16) | (uint32_t)ls_half;
}
