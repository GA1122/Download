static ULONG ShallPassPacket(PARANDIS_ADAPTER *pContext, PNET_PACKET_INFO pPacketInfo)
{
    ULONG i;

    if (pPacketInfo->dataLength > pContext->MaxPacketSize.nMaxFullSizeOsRx + ETH_PRIORITY_HEADER_SIZE)
        return FALSE;

    if ((pPacketInfo->dataLength > pContext->MaxPacketSize.nMaxFullSizeOsRx) && !pPacketInfo->hasVlanHeader)
        return FALSE;

    if (IsVlanSupported(pContext) && pPacketInfo->hasVlanHeader)
    {
        if (pContext->VlanId && pContext->VlanId != pPacketInfo->Vlan.VlanId)
        {
            return FALSE;
        }
    }

    if (pContext->PacketFilter & NDIS_PACKET_TYPE_PROMISCUOUS)
        return TRUE;

    if(pPacketInfo->isUnicast)
    {
        ULONG Res;

        if(!(pContext->PacketFilter & NDIS_PACKET_TYPE_DIRECTED))
            return FALSE;

        ETH_COMPARE_NETWORK_ADDRESSES_EQ(pPacketInfo->ethDestAddr, pContext->CurrentMacAddress, &Res);
        return !Res;
    }

    if(pPacketInfo->isBroadcast)
        return (pContext->PacketFilter & NDIS_PACKET_TYPE_BROADCAST);


    if(pContext->PacketFilter & NDIS_PACKET_TYPE_ALL_MULTICAST)
        return TRUE;

    if(!(pContext->PacketFilter & NDIS_PACKET_TYPE_MULTICAST))
        return FALSE;

    for (i = 0; i < pContext->MulticastData.nofMulticastEntries; i++)
    {
        ULONG Res;
        PUCHAR CurrMcastAddr = &pContext->MulticastData.MulticastList[i*ETH_LENGTH_OF_ADDRESS];

        ETH_COMPARE_NETWORK_ADDRESSES_EQ(pPacketInfo->ethDestAddr, CurrMcastAddr, &Res);

        if(!Res)
            return TRUE;
    }

    return FALSE;
}