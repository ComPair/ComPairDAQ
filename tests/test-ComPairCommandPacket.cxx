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
			if (pair.first == CmdPacket::Destination::SI_TR) data.at(1) = 0x1;
			else data.at(1) = 0x0;
			p.ParseData(data);
			REQUIRE(p.GetDestination() == pair.first);
		}
	}

	SECTION("Si Tracker Tests") {
		//Set the data type to the silicon tracker
		data.at(0) = types.at(1).second;

		// Loop over possible silicon tracker indexes and set the appropriate bit.
		for (short siIndex = 0; siIndex < 10; siIndex++) {
			if (siIndex < 8) {
				data.at(1) = 0x1 << siIndex;
			}
			else {
				data.at(0) = types.at(1).second | (0x1 << (siIndex - 8));
				data.at(1) = 0x0;
			}
			p.ParseData(data);
			REQUIRE(p.GetSiTracker() == siIndex);
		}

	}

	SECTION("Data Command") {
		//Data with the MSB set indicating the packet has data.
		data.at(0) = 0x0 + 0x80;
		p.ParseData(data);
		REQUIRE(p.HasData() == true);

		//Data with the MSB not set indicating the packet has no data.
		data.at(0) = 0x0;
		p.ParseData(data);
		REQUIRE(p.HasData() == false);
	}


	SECTION("Bad Data Test") {
		//CZT destination with a silicon tracker bit set
		data.at(0) = types.at(2).second | 0x1;
		REQUIRE_THROWS(p.ParseData(data));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);

		//CZT destination with a silicon tracker bit set
		data.at(0) = types.at(2).second;
		data.at(1) = 0x1;
		REQUIRE_THROWS(p.ParseData(data));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);
		//Si Tracker is set to 0, even though we have an invalid destination.
		REQUIRE(p.GetSiTracker() == 0);

		//Si Tracker destination with no si tracker flag
		data.at(0) = types.at(1).second;
		data.at(1) = 0x0;
		REQUIRE_THROWS(p.ParseData(data));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
		REQUIRE(p.GetSiTracker() == -1);

		//Si Tracker destination with multiple si tracker flags
		data.at(1) = 0x3;
		REQUIRE_THROWS(p.ParseData(data));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
		REQUIRE(p.GetSiTracker() == -1);

	}
}
