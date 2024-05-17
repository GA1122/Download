static int StreamTcpTest18 (void)
{
    StreamTcpThread stt;
    struct in_addr addr;
    char os_policy_name[10] = "windows";
    const char *ip_addr;
    TcpStream stream;
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    if (unlikely(p == NULL))
    return 0;
    IPV4Hdr ipv4h;
    int ret = 0;

    memset(&addr, 0, sizeof(addr));
    memset(&stream, 0, sizeof(stream));
    memset(p, 0, SIZE_OF_PACKET);
    memset(&ipv4h, 0, sizeof(ipv4h));

    StreamTcpUTInit(&stt.ra_ctx);
    SCHInfoCleanResources();

     
    ConfCreateContextBackup();
    ConfInit();
    ConfYamlLoadString(dummy_conf_string1, strlen(dummy_conf_string1));

     
    ip_addr = StreamTcpParseOSPolicy(os_policy_name);
    SCHInfoAddHostOSInfo(os_policy_name, ip_addr, -1);

    p->dst.family = AF_INET;
    p->ip4h = &ipv4h;
    addr.s_addr = inet_addr("192.168.1.1");
    p->dst.address.address_un_data32[0] = addr.s_addr;
    StreamTcpSetOSPolicy(&stream, p);

    if (stream.os_policy != OS_POLICY_WINDOWS)
        goto end;

    ret = 1;
end:
    ConfDeInit();
    ConfRestoreContextBackup();
    SCFree(p);
    StreamTcpUTDeinit(stt.ra_ctx);
    return ret;
}