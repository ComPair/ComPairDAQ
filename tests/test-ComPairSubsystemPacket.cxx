#include "catch.hpp"

#include "ComPairEventPacket.hxx"

TEST_CASE("TRIVIAL_SUBSYSTEM_PACKET") {

    TrivialSubsystemEventPacket ss_packet;
    
    SECTION("Test Initialization") {
        std::vector<uint16_t> data0 = {0,0,0,0};
        REQUIRE(ss_packet.ParseData(data0) == true);

        std::vector<uint16_t> data1 = {0,0,0};
        REQUIRE_THROWS(ss_packet.ParseData(data1));

    }

    SECTION("Test all fields set to `1`") {
        std::vector<uint16_t> data = {1<<4 | 1, 1<<15, 1, 1};
        REQUIRE(ss_packet.ParseData(data) == true);
        REQUIRE(ss_packet.get_packet_header() == 1);
        REQUIRE(ss_packet.get_packet_type() == 1);
        REQUIRE(ss_packet.get_trigger_ack() == true);
        REQUIRE(ss_packet.get_event_id() == 1);
        REQUIRE(ss_packet.get_crc() == 1);
    }

    SECTION("Test more interesting set of values") {
        std::vector<uint16_t> data = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        REQUIRE(ss_packet.ParseData(data) == true);
        REQUIRE(ss_packet.get_packet_header() == 0xFFF);
        REQUIRE(ss_packet.get_packet_type() == 0xF);
        REQUIRE(ss_packet.get_trigger_ack() == true);
        REQUIRE(ss_packet.get_event_id() == 0x7FFFFFFF);
        REQUIRE(ss_packet.get_crc() == 0xFFFF);
    }

    /*
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
    */

}


// vim: set ts=4 sw=4 sts=4 et:
