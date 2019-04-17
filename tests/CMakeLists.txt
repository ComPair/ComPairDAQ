include_directories(../Catch/include)

add_executable(test-ComPairCommandPacket test-ComPairCommandPacket.cxx)
target_link_libraries(test-ComPairCommandPacket Catch ComPairPackets)
add_test(test-ComPairCommandPacket test-ComPairCommandPacket)

add_executable(test-ComPairEventPacket test-ComPairEventPacket.cxx)
target_link_libraries(test-ComPairEventPacket Catch ComPairPackets)
add_test(test-ComPairEventPacket test-ComPairEventPacket)

add_executable(test-ComPairSubsystemPacket test-ComPairSubsystemPacket.cxx)
target_link_libraries(test-ComPairSubsystemPacket Catch ComPairPackets)
add_test(test-ComPairSubsystemPacket test-ComPairSubsystemPacket)
