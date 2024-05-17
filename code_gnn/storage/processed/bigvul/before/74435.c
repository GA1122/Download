VOID ParaNdis_PadPacketToMinimalLength(PNET_PACKET_INFO packetInfo)
{


    if(packetInfo->dataLength < ETH_MIN_PACKET_SIZE)
    {
        RtlZeroMemory(
                        RtlOffsetToPointer(packetInfo->headersBuffer, packetInfo->dataLength),
                        ETH_MIN_PACKET_SIZE - packetInfo->dataLength);
        packetInfo->dataLength = ETH_MIN_PACKET_SIZE;
    }
}
