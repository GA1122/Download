void StreamTcpPseudoPacketCreateStreamEndPacket(ThreadVars *tv, StreamTcpThread *stt, Packet *p, TcpSession *ssn, PacketQueue *pq)
{
    SCEnter();

    if (p->flags & PKT_PSEUDO_STREAM_END) {
        SCReturn;
    }

     
    if (ssn->server.seg_list == NULL && ssn->client.seg_list == NULL) {
        SCReturn;
    }

    Packet *np = StreamTcpPseudoSetup(p, GET_PKT_DATA(p), GET_PKT_LEN(p));
    if (np == NULL) {
        SCLogDebug("The packet received from packet allocation is NULL");
        StatsIncr(tv, stt->counter_tcp_pseudo_failed);
        SCReturn;
    }
    PKT_SET_SRC(np, PKT_SRC_STREAM_TCP_STREAM_END_PSEUDO);

     
    StreamTcpPseudoPacketSetupHeader(np,p);

    np->tenant_id = p->flow->tenant_id;

    np->flowflags = p->flowflags;

    np->flags |= PKT_STREAM_EST;
    np->flags |= PKT_STREAM_EOF;
    np->flags |= PKT_HAS_FLOW;
    np->flags |= PKT_PSEUDO_STREAM_END;

    if (p->flags & PKT_NOPACKET_INSPECTION) {
        DecodeSetNoPacketInspectionFlag(np);
    }
    if (p->flags & PKT_NOPAYLOAD_INSPECTION) {
        DecodeSetNoPayloadInspectionFlag(np);
    }

    if (PKT_IS_TOSERVER(p)) {
        SCLogDebug("original is to_server, so pseudo is to_client");
        np->flowflags &= ~FLOW_PKT_TOSERVER;
        np->flowflags |= FLOW_PKT_TOCLIENT;
#ifdef DEBUG
        BUG_ON(!(PKT_IS_TOCLIENT(np)));
        BUG_ON((PKT_IS_TOSERVER(np)));
#endif
    } else if (PKT_IS_TOCLIENT(p)) {
        SCLogDebug("original is to_client, so pseudo is to_server");
        np->flowflags &= ~FLOW_PKT_TOCLIENT;
        np->flowflags |= FLOW_PKT_TOSERVER;
#ifdef DEBUG
        BUG_ON(!(PKT_IS_TOSERVER(np)));
        BUG_ON((PKT_IS_TOCLIENT(np)));
#endif
    }

    PacketEnqueue(pq, np);

    StatsIncr(tv, stt->counter_tcp_pseudo);
    SCReturn;
}