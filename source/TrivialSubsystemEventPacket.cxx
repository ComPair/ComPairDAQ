#include "TrivialSubsystemEventPacket.hxx"
#include <stdexcept>

// Parse input raw data
bool TrivialSubsystemEventPacket::parse(std::vector<uint8_t> &data) {
    // Expect data to only be 8 bytes long...
    if (data.size() != 8) 
        throw std::runtime_error("Expect trivial subsystem packet to be 8 bytes long");
    this->parse_header(data);
    return true;
}

