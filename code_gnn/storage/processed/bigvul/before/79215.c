Packet *StreamTcpPseudoSetup(Packet *parent, uint8_t *pkt, uint32_t len)
{
    SCEnter();

    if (len == 0) {
        SCReturnPtr(NULL, "Packet");
    }

    Packet *p = PacketGetFromQueueOrAlloc();
    if (p == NULL) {
        SCReturnPtr(NULL, "Packet");
    }

     
    if (parent->root != NULL)
        p->root = parent->root;
    else
        p->root = parent;

     
    p->proto = parent->proto;
    p->datalink = parent->datalink;

    PacketCopyData(p, pkt, len);
    p->recursion_level = parent->recursion_level + 1;
    p->ts.tv_sec = parent->ts.tv_sec;
    p->ts.tv_usec = parent->ts.tv_usec;

    FlowReference(&p->flow, parent->flow);
     

     
    SET_TUNNEL_PKT(p);
     
    SET_TUNNEL_PKT(parent);

     
    TUNNEL_INCR_PKT_TPR(p);

    return p;
}
