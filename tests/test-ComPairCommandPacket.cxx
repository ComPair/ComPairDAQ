#include "catch.hpp"

#include "ComPairCommandPacket.hxx"

#include <vector>
#include <map>

typedef ComPairCommandPacket CmdPacket;

TEST_CASE("COMMAND_PACKET") {
	//Create a dummy packet to be reused.
	CmdPacket p;
	//Create a vector of data to be used as a template.
	std::vector< uint8_t > packetData = {0x0, 0x0, 0x4, 0x0};

	//Create a  map of the various types to their respective flags.
	std::map< CmdPacket::Destination, uint8_t > types = {
			{CmdPacket::Destination::ACD, 0x4},
			{CmdPacket::Destination::SI_TR, 0x8},
			{CmdPacket::Destination::CZT, 0x10},
			{CmdPacket::Destination::CSI, 0x20},
			{CmdPacket::Destination::TRIG, 0x40},
		};

	SECTION("Setters") {
		// Check destination setter.
		p.SetDestination(CmdPacket::Destination::CSI);
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CSI);

		// Check that an exception is thrown when siTracker flag is set for CsI destination.
		REQUIRE_THROWS(p.SetDestination(CmdPacket::Destination::CSI, 8));

		// Check that the destination Si tracker is set properly
		p.SetDestination(CmdPacket::Destination::SI_TR, 5);
		REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
		REQUIRE(p.GetSiTracker() == 5);

		// Check that an exception is thrown if Si Tracker Layer is not specified for Si Tracker destination.
		REQUIRE_THROWS(p.SetDestination(CmdPacket::Destination::SI_TR));

		// Check data setter
		p.SetData(0xF123);
		REQUIRE(p.HasData());
		REQUIRE(p.GetData() == 0xF123);

		p.SetCommand(0x12, 0xAB);
		REQUIRE(p.GetCommand() == 0x12);
	}

	SECTION("Test Packet") {
		p.SetDestination(CmdPacket::Destination::ACD);
		p.SetCommand(0x12, 0xAB);

		std::vector< uint8_t > correctPacket = {types[CmdPacket::Destination::ACD], 0x0, 0x12, 0xAB};
		REQUIRE(p.GetPacket() == correctPacket);

		uint16_t data = 0xBEEF;
		p.SetData(data);
		correctPacket.at(0) |= 0x80;
		correctPacket.push_back(data & 0xFF);
		correctPacket.push_back(data >> 16);
		REQUIRE(p.GetPacket() == correctPacket);
	}

	SECTION("Type Test") {
		//Loop over all pairs of type defined above and check that we get the correct type.
		for (auto pair : types) {
			packetData.at(0) = pair.second;
			//Check the silicon tracker separately.
			if (pair.first == CmdPacket::Destination::SI_TR) continue;
			p.Parse(packetData);
			REQUIRE(p.GetDestination() == pair.first);
		}
	}

	SECTION("Si Tracker Tests") {
		//Set the data type to the silicon tracker
		packetData.at(0) = types[CmdPacket::Destination::SI_TR];

		// Loop over possible silicon tracker indexes and set the appropriate bit.
		for (short siIndex = 0; siIndex < 10; siIndex++) {
			if (siIndex < 8) {
				packetData.at(1) = 0x1 << siIndex;
			}
			else {
				packetData.at(0) = types[CmdPacket::Destination::SI_TR] | (0x1 << (siIndex - 8));
				packetData.at(1) = 0x0;
			}
			p.Parse(packetData);
			REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
			REQUIRE(p.GetSiTracker() == siIndex);
		}

	}

	SECTION("Data Command") {
		//Data with the MSB not set indicating the packet has no data.
		packetData.at(0) = 0x0;
		p.Parse(packetData);
		REQUIRE(p.HasData() == false);
		REQUIRE_THROWS(p.GetData());

		//Data with the MSB set indicating the packet has data.
		packetData.at(0) = 0x0 + 0x80;
		packetData.push_back(0xF1);
		packetData.push_back(0x23);
		p.Parse(packetData);
		REQUIRE(p.HasData() == true);
		REQUIRE(p.GetData() == 0xF123);
	}


	SECTION("Bad Data Test") {
		//CZT destination with a silicon tracker bit set
		packetData.at(0) = types[CmdPacket::Destination::CZT] | 0x1;
		REQUIRE_THROWS(p.Parse(packetData));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);

		//CZT destination with a silicon tracker bit set
		packetData.at(0) = types[CmdPacket::Destination::CZT];
		packetData.at(1) = 0x1;
		REQUIRE_THROWS(p.Parse(packetData));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::CZT);
		//Si Tracker is set to 0, even though we have an invalid destination.
		REQUIRE(p.GetSiTracker() == 0);

		//Si Tracker destination with no si tracker flag
		packetData.at(0) = types[CmdPacket::Destination::SI_TR];
		packetData.at(1) = 0x0;
		REQUIRE_THROWS(p.Parse(packetData));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
		REQUIRE(p.GetSiTracker() == -1);

		//Si Tracker destination with multiple si tracker flags
		packetData.at(1) = 0x3;
		REQUIRE_THROWS(p.Parse(packetData));
		REQUIRE(p.GetDestination() == CmdPacket::Destination::SI_TR);
		REQUIRE(p.GetSiTracker() == -1);
	}
}
