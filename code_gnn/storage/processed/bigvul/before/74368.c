static VOID ParaNdis_DeviceFiltersUpdateRxMode(PARANDIS_ADAPTER *pContext)
{
    u8 val;
    ULONG f = pContext->PacketFilter;
    val = (f & NDIS_PACKET_TYPE_ALL_MULTICAST) ? 1 : 0;
    pContext->CXPath.SendControlMessage(VIRTIO_NET_CTRL_RX_MODE, VIRTIO_NET_CTRL_RX_MODE_ALLMULTI, &val, sizeof(val), NULL, 0, 2);
    val = (f & (NDIS_PACKET_TYPE_MULTICAST | NDIS_PACKET_TYPE_ALL_MULTICAST)) ? 0 : 1;
    pContext->CXPath.SendControlMessage(VIRTIO_NET_CTRL_RX_MODE, VIRTIO_NET_CTRL_RX_MODE_NOMULTI, &val, sizeof(val), NULL, 0, 2);
    val = (f & NDIS_PACKET_TYPE_DIRECTED) ? 0 : 1;
    pContext->CXPath.SendControlMessage(VIRTIO_NET_CTRL_RX_MODE, VIRTIO_NET_CTRL_RX_MODE_NOUNI, &val, sizeof(val), NULL, 0, 2);
    val = (f & NDIS_PACKET_TYPE_BROADCAST) ? 0 : 1;
    pContext->CXPath.SendControlMessage(VIRTIO_NET_CTRL_RX_MODE, VIRTIO_NET_CTRL_RX_MODE_NOBCAST, &val, sizeof(val), NULL, 0, 2);
    val = (f & NDIS_PACKET_TYPE_PROMISCUOUS) ? 1 : 0;
    pContext->CXPath.SendControlMessage(VIRTIO_NET_CTRL_RX_MODE, VIRTIO_NET_CTRL_RX_MODE_PROMISC, &val, sizeof(val), NULL, 0, 2);
}