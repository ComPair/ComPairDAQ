include_directories(../Catch/include)

add_executable(test-ComPairCommandPacket test-ComPairCommandPacket.cxx)
target_link_libraries(test-ComPairCommandPacket Catch ComPairPackets)
add_test(test-ComPairCommandPacket test-ComPairCommandPacket)
