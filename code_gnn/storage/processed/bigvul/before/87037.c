Packet *PacketDefragPktSetup(Packet *parent, uint8_t *pkt, uint32_t len, uint8_t proto)
{
    SCEnter();

     
    Packet *p = PacketGetFromQueueOrAlloc();
    if (unlikely(p == NULL)) {
        SCReturnPtr(NULL, "Packet");
    }

     
    if (parent->root != NULL)
        p->root = parent->root;
    else
        p->root = parent;

     
    if (pkt && len) {
        PacketCopyData(p, pkt, len);
    }
    p->recursion_level = parent->recursion_level;  
    p->ts.tv_sec = parent->ts.tv_sec;
    p->ts.tv_usec = parent->ts.tv_usec;
    p->datalink = DLT_RAW;
    p->tenant_id = parent->tenant_id;
     
    SET_TUNNEL_PKT(p);
    p->vlan_id[0] = parent->vlan_id[0];
    p->vlan_id[1] = parent->vlan_id[1];
    p->vlan_idx = parent->vlan_idx;

    SCReturnPtr(p, "Packet");
}