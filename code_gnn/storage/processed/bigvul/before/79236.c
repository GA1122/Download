static int StreamTcpTest06 (void)
{
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    if (unlikely(p == NULL))
        return 0;
    Flow f;
    TcpSession ssn;
    ThreadVars tv;
    StreamTcpThread stt;
    TCPHdr tcph;
    memset(p, 0, SIZE_OF_PACKET);
    PacketQueue pq;
    memset(&pq,0,sizeof(PacketQueue));
    memset (&f, 0, sizeof(Flow));
    memset(&ssn, 0, sizeof (TcpSession));
    memset(&tv, 0, sizeof (ThreadVars));
    memset(&stt, 0, sizeof (StreamTcpThread));
    memset(&tcph, 0, sizeof (TCPHdr));
    FLOW_INITIALIZE(&f);
    p->flow = &f;
    int ret = 0;

    StreamTcpUTInit(&stt.ra_ctx);

    tcph.th_flags = TH_FIN;
    p->tcph = &tcph;

     
    if (StreamTcpPacket(&tv, p, &stt, &pq) != -1) {
        printf("StreamTcpPacket failed: ");
        goto end;
    }

    if (((TcpSession *)(p->flow->protoctx)) != NULL) {
        printf("we have a ssn while we shouldn't: ");
        goto end;
    }

    p->tcph->th_flags = TH_RST;
     
    if (StreamTcpPacket(&tv, p, &stt, &pq) != -1) {
        printf("StreamTcpPacket failed (2): ");
        goto end;
    }

    if (((TcpSession *)(p->flow->protoctx)) != NULL) {
        printf("we have a ssn while we shouldn't (2): ");
        goto end;
    }

    ret = 1;
end:
    SCFree(p);
    FLOW_DESTROY(&f);
    StreamTcpUTDeinit(stt.ra_ctx);
    return ret;
}