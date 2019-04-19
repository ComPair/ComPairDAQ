#include "catch.hpp"
#include "ComPairSubsystemEventPacket.hxx"
#include "TrivialSubsystemEventPacket.hxx"
#include "SimSubsystemEventPacket.hxx"

TEST_CASE("TRIVIAL_SUBSYSTEM_PACKET") {

    TrivialSubsystemEventPacket ss_packet;
    
    SECTION("Test Initialization") {
        std::vector<uint8_t> data0{0,0,0,0,0,0,0,0};
        REQUIRE(ss_packet.parse(data0) == true);

        std::vector<uint8_t> data1{0,0,0};
        REQUIRE_THROWS(ss_packet.parse(data1));

    }

    SECTION("Test all fields set to `1`") {
        std::vector<uint16_t> data{1<<4 | 1, 1, 1<<15, 1};
        REQUIRE(ss_packet.parse_uint16(data) == true);
        REQUIRE(ss_packet.packet_header() == 1);
        REQUIRE(ss_packet.packet_type() == 1);
        REQUIRE(ss_packet.trigger_ack() == true);
        REQUIRE(ss_packet.event_id() == 1);
        REQUIRE(ss_packet.crc() == 1);
    }

    SECTION("Test interpreting packet of all 1's") {
        std::vector<uint16_t> data{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        REQUIRE(ss_packet.parse_uint16(data) == true);
        REQUIRE(ss_packet.packet_header() == 0xFFF);
        REQUIRE(ss_packet.packet_type() == 0xF);
        REQUIRE(ss_packet.trigger_ack() == true);
        REQUIRE(ss_packet.event_id() == 0x7FFFFFFF);
        REQUIRE(ss_packet.crc() == 0xFFFF);
    }
}

TEST_CASE("SIM_SUBSYSTEM_PACKET") {

    SimSubsystemEventPacket sim_packet;

    SECTION("Test SimSubsystemEventPacket::set_event_data") {
        std::vector<float> x{1.0, 2.0};
        std::vector<float> y{3.0, 4.0};
        std::vector<float> z{5.0, 6.0};
        std::vector<float> E{7.0, 8.0};
        sim_packet.set_event_data(x, y, z, E);
        REQUIRE(sim_packet.nhit() == 2);
        for (int i=0; i<2; i++) {
            REQUIRE(sim_packet.hit_x(i) == x.at(i));
            REQUIRE(sim_packet.hit_y(i) == y.at(i));
            REQUIRE(sim_packet.hit_z(i) == z.at(i));
            REQUIRE(sim_packet.hit_E(i) == E.at(i));
        }
    }

    SECTION("Test simple parsing of sim data") {
        std::vector<uint16_t> data;

        data.resize(13); // single hit: size is 6 + 8*nhit
        // Set header and check-sum:
        data.at(0) = 0xFFFF;
        data.at(1) = 0xFFFF;
        data.at(2) = 0xFFFF;
        data.at(12) = 0xFFFF;

        data.at(3) = 1; // Setting number of hits
        // Feeding in the following hit data (1 hit):
        // x, y, z, E: 1.0, 2.0, 3.0, 4.0
        float fvals[] = {1.0, 2.0, 3.0, 4.0};
        float *pfdata = reinterpret_cast<float *>(data.data() + 4);
        for (int i=0; i<4; i++) {
            *pfdata = fvals[i];
            pfdata++;
        }
        REQUIRE(sim_packet.parse_uint16(data) == true);
        REQUIRE(sim_packet.hit_x(0) == fvals[0]);
        REQUIRE(sim_packet.hit_y(0) == fvals[1]);
        REQUIRE(sim_packet.hit_z(0) == fvals[2]);
        REQUIRE(sim_packet.hit_E(0) == fvals[3]);
    }
}

