static int StreamTcpTest16 (void)
{
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    if (unlikely(p == NULL))
        return 0;
    Flow f;
    ThreadVars tv;
    StreamTcpThread stt;
    TCPHdr tcph;
    uint8_t payload[4];
    struct in_addr addr;
    IPV4Hdr ipv4h;
    char os_policy_name[10] = "windows";
    const char *ip_addr;
    PacketQueue pq;
    memset(&pq,0,sizeof(PacketQueue));

    memset(p, 0, SIZE_OF_PACKET);
    memset (&f, 0, sizeof(Flow));
    memset(&tv, 0, sizeof (ThreadVars));
    memset(&stt, 0, sizeof (StreamTcpThread));
    memset(&tcph, 0, sizeof (TCPHdr));
    memset(&addr, 0, sizeof(addr));
    memset(&ipv4h, 0, sizeof(ipv4h));
    FLOW_INITIALIZE(&f);
    p->flow = &f;
    int ret = 0;

    StreamTcpUTInit(&stt.ra_ctx);

     
    ConfCreateContextBackup();
    ConfInit();
    ConfYamlLoadString(dummy_conf_string1, strlen(dummy_conf_string1));

     
    ip_addr = StreamTcpParseOSPolicy(os_policy_name);
    SCHInfoAddHostOSInfo(os_policy_name, ip_addr, -1);
    strlcpy(os_policy_name, "linux\0", sizeof(os_policy_name));
    ip_addr = StreamTcpParseOSPolicy(os_policy_name);
    SCHInfoAddHostOSInfo(os_policy_name, ip_addr, -1);
    addr.s_addr = inet_addr("192.168.0.1");
    tcph.th_win = htons(5480);
    tcph.th_seq = htonl(10);
    tcph.th_ack = htonl(20);
    tcph.th_flags = TH_ACK|TH_PUSH;
    p->tcph = &tcph;
    p->dst.family = AF_INET;
    p->dst.address.address_un_data32[0] = addr.s_addr;
    p->ip4h = &ipv4h;

    StreamTcpCreateTestPacket(payload, 0x41, 3, sizeof(payload));  
    p->payload = payload;
    p->payload_len = 3;

    FLOWLOCK_WRLOCK(&f);
    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    p->tcph->th_seq = htonl(20);
    p->tcph->th_ack = htonl(13);
    p->tcph->th_flags = TH_ACK|TH_PUSH;
    p->flowflags = FLOW_PKT_TOCLIENT;

    StreamTcpCreateTestPacket(payload, 0x42, 3, sizeof(payload));  
    p->payload = payload;
    p->payload_len = 3;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    p->tcph->th_seq = htonl(15);
    p->tcph->th_ack = htonl(23);
    p->tcph->th_flags = TH_ACK|TH_PUSH;
    p->flowflags = FLOW_PKT_TOSERVER;

    StreamTcpCreateTestPacket(payload, 0x43, 3, sizeof(payload));  
    p->payload = payload;
    p->payload_len = 3;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    p->tcph->th_seq = htonl(14);
    p->tcph->th_ack = htonl(23);
    p->tcph->th_flags = TH_ACK|TH_PUSH;
    p->flowflags = FLOW_PKT_TOSERVER;

    StreamTcpCreateTestPacket(payload, 0x43, 3, sizeof(payload));  
    p->payload = payload;
    p->payload_len = 3;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    addr.s_addr = inet_addr("192.168.1.1");
    p->tcph->th_seq = htonl(25);
    p->tcph->th_ack = htonl(13);
    p->tcph->th_flags = TH_ACK|TH_PUSH;
    p->flowflags = FLOW_PKT_TOCLIENT;
    p->dst.address.address_un_data32[0] = addr.s_addr;

    StreamTcpCreateTestPacket(payload, 0x44, 3, sizeof(payload));  
    p->payload = payload;
    p->payload_len = 3;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    p->tcph->th_seq = htonl(24);
    p->tcph->th_ack = htonl(13);
    p->tcph->th_flags = TH_ACK|TH_PUSH;
    p->flowflags = FLOW_PKT_TOCLIENT;

    StreamTcpCreateTestPacket(payload, 0x44, 3, sizeof(payload));  
    p->payload = payload;
    p->payload_len = 3;

    if (StreamTcpPacket(&tv, p, &stt, &pq) == -1)
        goto end;

    if (stream_config.midstream != TRUE) {
        ret = 1;
        goto end;
    }
    if (((TcpSession *)(p->flow->protoctx))->state != TCP_ESTABLISHED)
        goto end;

    if (((TcpSession *)(p->flow->protoctx))->client.next_seq != 13 &&
            ((TcpSession *)(p->flow->protoctx))->server.next_seq != 23) {
        printf("failed in next_seq match client.next_seq %"PRIu32""
                " server.next_seq %"PRIu32"\n",
                ((TcpSession *)(p->flow->protoctx))->client.next_seq,
                ((TcpSession *)(p->flow->protoctx))->server.next_seq);
        goto end;
    }

    if (((TcpSession *)(p->flow->protoctx))->client.os_policy !=
            OS_POLICY_LINUX && ((TcpSession *)
            (p->flow->protoctx))->server.os_policy != OS_POLICY_WINDOWS)
    {
        printf("failed in setting up OS policy, client.os_policy: %"PRIu8""
                " should be %"PRIu8" and server.os_policy: %"PRIu8""
                " should be %"PRIu8"\n", ((TcpSession *)
                (p->flow->protoctx))->client.os_policy, (uint8_t)OS_POLICY_LINUX,
                ((TcpSession *)(p->flow->protoctx))->server.os_policy,
                (uint8_t)OS_POLICY_WINDOWS);
        goto end;
    }
    StreamTcpSessionPktFree(p);

    ret = 1;
end:
    ConfDeInit();
    ConfRestoreContextBackup();
    FLOWLOCK_UNLOCK(&f);
    SCFree(p);
    FLOW_DESTROY(&f);
    StreamTcpUTDeinit(stt.ra_ctx);
    return ret;
}