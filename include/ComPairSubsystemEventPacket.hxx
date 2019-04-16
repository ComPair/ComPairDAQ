#ifndef COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
#define COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX

#include <cstdint>
#include <vector>

class ComPairSubsystemEventPacket {
	public:
		virtual bool ParseData(const std::vector< uint16_t > &data) = 0;

	private:
		uint16_t evtId;

};

#endif // COMPAIR_SUBSYSTEM_EVENT_PACKET_HXX
