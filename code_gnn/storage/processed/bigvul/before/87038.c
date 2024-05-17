void PacketDefragPktSetupParent(Packet *parent)
{
     
    SET_TUNNEL_PKT(parent);

     
    TUNNEL_INCR_PKT_TPR(parent);

     
    DecodeSetNoPayloadInspectionFlag(parent);
}
