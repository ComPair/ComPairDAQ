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

}


// vim: set ts=4 sw=4 sts=4 et:
