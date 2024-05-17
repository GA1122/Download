UINT PktGetTCPCoalescedSegmentsCount(PNET_PACKET_INFO PacketInfo, UINT nMaxTCPPayloadSize)
{
    return PacketInfo->L2PayloadLen / nMaxTCPPayloadSize +
        !!(PacketInfo->L2PayloadLen % nMaxTCPPayloadSize);
}
