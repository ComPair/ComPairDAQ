add_library(ComPairPackets SHARED
	ComPairSubsystemEventPacket.cxx
	ComPairEventPacket.cxx
	ComPairCommandPacket.cxx
    TrivialSubsystemEventPacket.cxx 
    SimSubsystemEventPacket.cxx)
install(TARGETS ComPairPackets DESTINATION lib)
