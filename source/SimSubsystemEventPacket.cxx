#include "SimSubsystemEventPacket.hxx"

#include <stdexcept>

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

