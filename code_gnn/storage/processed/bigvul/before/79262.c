static int StreamTcpTest41(void)
{
     
    uint8_t raw_ip[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x28, 0x06, 0x40,
        0x20, 0x01, 0x06, 0x18, 0x04, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x51, 0x99, 0xcc, 0x70,
        0x20, 0x01, 0x06, 0x18, 0x00, 0x01, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
        0x8c, 0x9b, 0x00, 0x50, 0x6a, 0xe7, 0x07, 0x36,
        0x00, 0x00, 0x00, 0x00, 0xa0, 0x02, 0x16, 0x30,
        0x29, 0x9c, 0x00, 0x00, 0x02, 0x04, 0x05, 0x8c,
        0x04, 0x02, 0x08, 0x0a, 0x00, 0xdd, 0x1a, 0x39,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x02 };
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    if (unlikely(p == NULL))
        return 0;
    ThreadVars tv;
    DecodeThreadVars dtv;

    memset(&dtv, 0, sizeof(DecodeThreadVars));
    memset(&tv,  0, sizeof(ThreadVars));
    memset(p, 0, SIZE_OF_PACKET);
    PACKET_INITIALIZE(p);

    if (PacketCopyData(p, raw_ip, sizeof(raw_ip)) == -1) {
        PacketFree(p);
        return 1;
    }

    FlowInitConfig(FLOW_QUIET);

    DecodeRaw(&tv, &dtv, p, raw_ip, GET_PKT_LEN(p), NULL);

    if (p->ip6h == NULL) {
        printf("expected a valid ipv6 header but it was NULL: ");
        FlowShutdown();
        SCFree(p);
        return 1;
    }

    if(p->tcph == NULL) {
        SCFree(p);
        return 0;
    }

    Packet *np = StreamTcpPseudoSetup(p, GET_PKT_DATA(p), GET_PKT_LEN(p));
    if (np == NULL) {
        printf("the packet received from packet allocation is NULL: ");
        return 0;
    }

    StreamTcpPseudoPacketSetupHeader(np,p);

    if (((uint8_t *)p->tcph - (uint8_t *)p->ip6h) != ((uint8_t *)np->tcph - (uint8_t *)np->ip6h)) {
        return 0;
    }

    PACKET_RECYCLE(np);
    PACKET_RECYCLE(p);
    SCFree(p);
    FlowShutdown();

    return 1;
}