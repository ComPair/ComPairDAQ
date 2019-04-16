#include "catch.hpp"

#include "ComPairCommandPacket.hxx"

#include <vector>
#include <map>

typedef ComPairCommandPacket CmdPacket;

TEST_CASE("COMMAND_PACKET") {
	//Create a dummy packet to be reused.
	CmdPacket p;
	//Create a vector of data to be used as a template.
	std::vector< uint8_t > data = {0x0, 0x0, 0x4, 0x0};

	//Create a  map of the various types to their respective flags.
	std::map< CmdPacket::Destination, uint8_t > types = {
			{CmdPacket::Destination::ACD, 0x4},
			{CmdPacket::Destination::SI_TR, 0x8},
			{CmdPacket::Destination::CZT, 0x10},
			{CmdPacket::Destination::CSI, 0x20},
			{CmdPacket::Destination::TRIG, 0x40},
		};

	SECTION("Type Test") {
		//Loop over all pairs of type defined above and check that we get the correct type.
		for (auto pair : types) {
			data.at(0) = pair.second;
			//Check the silicon tracker separately.
			if (pair.first == CmdPacket::Destination::SI_TR) continue;
			p.ParseData(data);
			REQUIRE(p.GetDestination() == pair.first);
		}
	}

	SECTION("Si Tracker Tests") {
		//Set the data type to the silicon tracker
		data.at(0) = types[CmdPacket::Destination::SI_TR];

		// Loop over possible silicon tracker indexes and set the appropriate bit.
		for (short siIndex = 0; siIndex < 10; siIndex++) {
			if (siIndex < 8) {
				data.at(1) = 0x1 << siIndex;
			}
			else {
				data.at(0) = types[CmdPacket::Destination::SI_TR] | (0x1 << (siIndex - 8));
				data.at(1) = 0x0;
			}
			p.ParseData(data);
			REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
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
		data.at(0) = types[CmdPacket::Destination::CZT] | 0x1;
		REQUIRE_THROWS(p.ParseData(data));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);

		//CZT destination with a silicon tracker bit set
		data.at(0) = types[CmdPacket::Destination::CZT];
		data.at(1) = 0x1;
		REQUIRE_THROWS(p.ParseData(data));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);
		//Si Tracker is set to 0, even though we have an invalid destination.
		REQUIRE(p.GetSiTracker() == 0);

		//Si Tracker destination with no si tracker flag
		data.at(0) = types[CmdPacket::Destination::SI_TR];
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
