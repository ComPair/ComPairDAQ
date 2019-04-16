#include "catch.hpp"

#include "ComPairCommandPacket.hxx"

typedef ComPairCommandPacket CmdPacket;

TEST_CASE("COMMAND_PACKET") {
	std::vector< uint8_t > data = {0x10, 0x0, 0x4, 0x0};
	SECTION("Test the type") {
		CmdPacket p;
		p.ParseData(data);

		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);
	}
}
