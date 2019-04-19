#include "catch.hpp"

#include "ComPairEventPacket.hxx"
#include "SimSubsystemEventPacket.hxx"

TEST_CASE("EVENT_PACKET") {

    ComPairEventPacket ev_packet;

    SECTION("Test trivial packet, with only the header and size.") {
        std::vector<uint16_t> data{14, 0, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.parse(data) == true);
    }

    SECTION("Test that we parse source correctly") {
        // This should end up being from the SI0 source
        std::vector<uint16_t> data = {14, 0x0001 | 0x0800, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.parse(data) == true);
        REQUIRE(ev_packet.packet_source() == ComPairEventPacket::PacketSource::SI0);
    }

    SECTION("Test that we are setting the packet type correctly to EVENT") {
        std::vector<uint16_t> data = {14, 0x0001 | 0x0800 | 1<<15, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.parse(data) == true);
        REQUIRE(ev_packet.packet_source() == ComPairEventPacket::PacketSource::SI0);
        REQUIRE(ev_packet.packet_type() == ComPairEventPacket::PacketType::EVENT);
    }

    SECTION("Test that we are setting the packet type to MONITOR") {
        std::vector<uint16_t> data = {14, 0x0001 | 0x0800, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.parse(data) == true);
        REQUIRE(ev_packet.packet_source() == ComPairEventPacket::PacketSource::SI0);
        REQUIRE(ev_packet.packet_type() == ComPairEventPacket::PacketType::MONITOR);
    }

    SECTION("Test setting seconds and useconds correctly") {
        std::vector<uint16_t> data0 = {14, 0x0001 | 0x0800, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.parse(data0) == true);
        REQUIRE(ev_packet.seconds() == 1);
        REQUIRE(ev_packet.useconds() == 2);

        std::vector<uint16_t> data1 = {14, 0x0001 | 0x0800, 1, 0, 2, 0, 14};
        REQUIRE(ev_packet.parse(data1) == true);
        REQUIRE(ev_packet.seconds() == (1<<16));
        REQUIRE(ev_packet.useconds() == (2<<16));

        std::vector<uint16_t> data2 = {14, 0x0001 | 0x0800, 1, 2, 3, 4, 14};
        REQUIRE(ev_packet.parse(data2) == true);
        REQUIRE(ev_packet.seconds() == (1<<16 | 2));
        REQUIRE(ev_packet.useconds() == (3<<16 | 4));
    }

     SECTION("An event packet") {
        std::vector<uint16_t> subsystemData;
        subsystemData.resize(13); // single hit: size is 6 + 8*nhit
        // Set header and check-sum:
        subsystemData.at(0) = 0xFFFF;
        subsystemData.at(1) = 0xFFFF;
        subsystemData.at(2) = 0xFFFF;
        subsystemData.at(12) = 0xFFFF;

        subsystemData.at(3) = 1; // Setting number of hits
        // x, y, z, E: 1.0, 2.0, 3.0, 4.0
        float fvals[] = {1.0, 2.0, 3.0, 4.0};
        float *pfdata = reinterpret_cast<float*>(subsystemData.data() + 4);
        for (int i=0; i<4; i++) {
            *pfdata = fvals[i];
            pfdata++;
        }
        std::vector< uint16_t > data (subsystemData.size() + 7);
        std::copy(subsystemData.begin(), subsystemData.end(), data.begin() + 6);

        data.at(0) = data.size() * 2;
        data.at(data.size() - 1) = data.size() * 2;
        data.at(1) = uint16_t(ComPairEventPacket::PacketSource::SIM);

        ev_packet.parse(data);

        auto sim_packet = dynamic_cast< SimSubsystemEventPacket* >(ev_packet.subsystem_packet());

        REQUIRE(sim_packet->hit_x(0) == 1.0);
        REQUIRE(sim_packet->hit_y(0) == 2.0);
        REQUIRE(sim_packet->hit_z(0) == 3.0);
        REQUIRE(sim_packet->hit_E(0) == 4.0);

     }

}

