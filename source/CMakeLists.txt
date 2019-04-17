add_library(ComPairPackets SHARED
	ComPairSubsystemEventPacket.cxx
	ComPairEventPacket.cxx
	ComPairCommandPacket.cxx)
install(TARGETS ComPairPackets DESTINATION lib)
