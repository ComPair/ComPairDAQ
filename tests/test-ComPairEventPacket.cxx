#include "catch.hpp"

#include "ComPairEventPacket.hxx"

TEST_CASE("EVENT_PACKET") {

    ComPairEventPacket ev_packet;

    SECTION("Test trivial packet, with only the header and size.") {
        std::vector<uint16_t> data = {14, 0, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.ParseData(data) == true);
    }

    SECTION("Test that we parse source correctly") {
        // This should end up being the Si0_SOURCE:
        std::vector<uint16_t> data = {14, 0x0001 | 0x0800, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.ParseData(data) == true);
        REQUIRE(ev_packet.get_source() == ComPairEventPacket::PacketSource::Si0_SOURCE);
    }

    SECTION("Test that we are setting the packet type correctly to EVENT_PACKET") {
        std::vector<uint16_t> data = {14, 0x0001 | 0x0800 | 1<<15, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.ParseData(data) == true);
        REQUIRE(ev_packet.get_source() == ComPairEventPacket::PacketSource::Si0_SOURCE);
        REQUIRE(ev_packet.get_type() == ComPairEventPacket::PacketType::EVENT_PACKET);
    }

    SECTION("Test that we are setting the packet type to MONITOR_PACKET") {
        std::vector<uint16_t> data = {14, 0x0001 | 0x0800, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.ParseData(data) == true);
        REQUIRE(ev_packet.get_source() == ComPairEventPacket::PacketSource::Si0_SOURCE);
        REQUIRE(ev_packet.get_type() == ComPairEventPacket::PacketType::MONITOR_PACKET);
    }

    SECTION("Test setting seconds and useconds correctly") {
        std::vector<uint16_t> data0 = {14, 0x0001 | 0x0800, 0, 1, 0, 2, 14};
        REQUIRE(ev_packet.ParseData(data0) == true);
        REQUIRE(ev_packet.get_seconds() == 1);
        REQUIRE(ev_packet.get_useconds() == 2);

        std::vector<uint16_t> data1 = {14, 0x0001 | 0x0800, 1, 0, 2, 0, 14};
        REQUIRE(ev_packet.ParseData(data1) == true);
        REQUIRE(ev_packet.get_seconds() == (1<<16));
        REQUIRE(ev_packet.get_useconds() == (2<<16));

        std::vector<uint16_t> data2 = {14, 0x0001 | 0x0800, 1, 2, 3, 4, 14};
        REQUIRE(ev_packet.ParseData(data2) == true);
        REQUIRE(ev_packet.get_seconds() == (1<<16 | 2));
        REQUIRE(ev_packet.get_useconds() == (3<<16 | 4));
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
        float fval = 1.0;
        int indx = 4;
        uint32_t *uval = reinterpret_cast<uint32_t *>(&fval);
        uint16_t mshalf, lshalf;
        for (int cnt=0; cnt<4; cnt++) {
            mshalf = (*uval >> 16) & 0x0000FFFF;
            lshalf = *uval & 0x0000FFFF;
            subsystemData.at(indx++) = mshalf;
            subsystemData.at(indx++) = lshalf;
            fval += 1.0;
        }

        std::vector< uint16_t > data (subsystemData.size() + 7);
        std::copy(subsystemData.begin(), subsystemData.end(), data.begin() + 6);

        data.at(0) = data.size() * 2;
        data.at(data.size() - 1) = data.size() * 2;
        data.at(1) = uint16_t(ComPairEventPacket::PacketSource::SIM_SOURCE);

        ev_packet.ParseData(data);

        auto sim_packet = dynamic_cast< SimSubsystemEventPacket* >(ev_packet.GetSubsystemPacket());

        REQUIRE(sim_packet->get_hit_x(0) == 1.0);
        REQUIRE(sim_packet->get_hit_y(0) == 2.0);
        REQUIRE(sim_packet->get_hit_z(0) == 3.0);
        REQUIRE(sim_packet->get_hit_E(0) == 4.0);

     }

}


// vim: set ts=4 sw=4 sts=4 et:
