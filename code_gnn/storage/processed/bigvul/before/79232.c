static int StreamTcpTest02 (void)
{
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    FAIL_IF(unlikely(p == NULL));
    Flow f;
    ThreadVars tv;
    StreamTcpThread stt;
    uint8_t payload[4];
    TCPHdr tcph;
    PacketQueue pq;
    memset(&pq,0,sizeof(PacketQueue));
    memset(p, 0, SIZE_OF_PACKET);
    memset (&f, 0, sizeof(Flow));
    memset(&tv, 0, sizeof (ThreadVars));
    memset(&stt, 0, sizeof (StreamTcpThread));
    memset(&tcph, 0, sizeof (TCPHdr));

    FLOW_INITIALIZE(&f);
    p->flow = &f;
    tcph.th_win = htons(5480);
    tcph.th_flags = TH_SYN;
    p->tcph = &tcph;
    p->flowflags = FLOW_PKT_TOSERVER;

    StreamTcpUTInit(&stt.ra_ctx);

    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    p->tcph->th_ack = htonl(1);
    p->tcph->th_flags = TH_SYN | TH_ACK;
    p->flowflags = FLOW_PKT_TOCLIENT;

    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    p->tcph->th_ack = htonl(1);
    p->tcph->th_seq = htonl(1);
    p->tcph->th_flags = TH_ACK;
    p->flowflags = FLOW_PKT_TOSERVER;

    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    p->tcph->th_ack = htonl(1);
    p->tcph->th_seq = htonl(2);
    p->tcph->th_flags = TH_PUSH | TH_ACK;
    p->flowflags = FLOW_PKT_TOSERVER;

    StreamTcpCreateTestPacket(payload, 0x41, 3, 4);  
    p->payload = payload;
    p->payload_len = 3;

    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    p->flowflags = FLOW_PKT_TOCLIENT;
    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    p->tcph->th_ack = htonl(1);
    p->tcph->th_seq = htonl(6);
    p->tcph->th_flags = TH_PUSH | TH_ACK;
    p->flowflags = FLOW_PKT_TOSERVER;

    StreamTcpCreateTestPacket(payload, 0x42, 3, 4);  
    p->payload = payload;
    p->payload_len = 3;

    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    p->flowflags = FLOW_PKT_TOCLIENT;
    FAIL_IF(StreamTcpPacket(&tv, p, &stt, &pq) == -1);

    StreamTcpSessionClear(p->flow->protoctx);

    SCFree(p);
    FLOW_DESTROY(&f);
    StreamTcpUTDeinit(stt.ra_ctx);
    PASS;
}