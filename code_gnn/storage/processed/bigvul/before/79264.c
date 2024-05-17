static int StreamTcpTest43 (void)
{
    int ret = 0;
    Flow f;
    ThreadVars tv;
    StreamTcpThread stt;
    TCPHdr tcph;
    PacketQueue pq;
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    TcpSession *ssn;

    if (unlikely(p == NULL))
        return 0;
    memset(p, 0, SIZE_OF_PACKET);

    memset(&pq,0,sizeof(PacketQueue));
    memset (&f, 0, sizeof(Flow));
    memset(&tv, 0, sizeof (ThreadVars));
    memset(&stt, 0, sizeof (StreamTcpThread));
    memset(&tcph, 0, sizeof (TCPHdr));

    StreamTcpUTInit(&stt.ra_ctx);

    FLOW_INITIALIZE(&f);
    p->tcph = &tcph;
    tcph.th_win = htons(5480);
    p->flow = &f;

     
    tcph.th_flags = TH_SYN;
    tcph.th_seq = htonl(100);
    p->flowflags = FLOW_PKT_TOSERVER;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

     
    p->tcph->th_seq = htonl(500);
    p->tcph->th_ack = htonl(101);
    p->tcph->th_flags = TH_SYN | TH_ACK;
    p->flowflags = FLOW_PKT_TOCLIENT;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

     
    p->tcph->th_seq = htonl(1000);
    p->tcph->th_ack = htonl(101);
    p->tcph->th_flags = TH_SYN | TH_ACK;
    p->flowflags = FLOW_PKT_TOCLIENT;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

     
    p->tcph->th_ack = htonl(1001);
    p->tcph->th_seq = htonl(101);
    p->tcph->th_flags = TH_ACK;
    p->flowflags = FLOW_PKT_TOSERVER;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    ssn = p->flow->protoctx;

    if (ssn->state != TCP_ESTABLISHED) {
        printf("state not TCP_ESTABLISHED: ");
        goto end;
    }

    if (ssn->server.isn != 1000) {
        SCLogDebug("ssn->server.isn %"PRIu32" != %"PRIu32"",
            ssn->server.isn, 1000);
        goto end;
    }
    if (ssn->client.isn != 100) {
        SCLogDebug("ssn->client.isn %"PRIu32" != %"PRIu32"",
            ssn->client.isn, 100);
        goto end;
    }

    StreamTcpSessionClear(p->flow->protoctx);

    ret = 1;
end:
    SCFree(p);
    FLOW_DESTROY(&f);
    StreamTcpUTDeinit(stt.ra_ctx);
    return ret;
}