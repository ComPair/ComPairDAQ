#include "catch.hpp"

#include "ComPairCommandPacket.hxx"

typedef ComPairCommandPacket CmdPacket;

TEST_CASE("COMMAND_PACKET") {
	CmdPacket p;
	std::vector< uint8_t > data = {0x0, 0x0, 0x4, 0x0};

	std::vector< std::pair< CmdPacket::Destination, uint8_t > > types = {
			std::make_pair(CmdPacket::Destination::ACD, 0x4),
			std::make_pair(CmdPacket::Destination::SI_TR, 0x8),
			std::make_pair(CmdPacket::Destination::CZT, 0x10),
			std::make_pair(CmdPacket::Destination::CSI, 0x20),
			std::make_pair(CmdPacket::Destination::TRIG, 0x40),
		};

	SECTION("Type Test") {
		//Loop over all pairs of type defined above and check that we get the correct type.
		for (auto pair : types) {
			data.at(0) = pair.second;
			p.ParseData(data);
			REQUIRE(p.GetDestination() == pair.first);
		}
		//Reset first word
		data.at(0) = 0x0;
	}

	SECTION("Bad Data Test") {
		//Data with the MSB set.
		data.at(0) = 0x0 + 0x80;
		REQUIRE_THROWS(p.ParseData(data));

		//CZT destination with a silicon tracker bit set
		data.at(0) = types.at(2).second + 0x1;
		p.ParseData(data);
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);

		data.at(0) = 0;
	}
}
