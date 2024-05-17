void dns_packet_set_flags(DnsPacket *p, bool dnssec_checking_disabled, bool truncated) {

        DnsPacketHeader *h;

        assert(p);

        h = DNS_PACKET_HEADER(p);

        switch(p->protocol) {
        case DNS_PROTOCOL_LLMNR:
                assert(!truncated);

                h->flags = htobe16(DNS_PACKET_MAKE_FLAGS(0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ));
                break;

        case DNS_PROTOCOL_MDNS:
                h->flags = htobe16(DNS_PACKET_MAKE_FLAGS(0          ,
                                                         0          ,
                                                         0          ,
                                                         truncated  ,
                                                         0          ,
                                                         0          ,
                                                         0          ,
                                                         0          ,
                                                         0          ));
                break;

        default:
                assert(!truncated);

                h->flags = htobe16(DNS_PACKET_MAKE_FLAGS(0  ,
                                                         0  ,
                                                         0  ,
                                                         0  ,
                                                         1  ,
                                                         0  ,
                                                         0  ,
                                                         dnssec_checking_disabled  ,
                                                         0  ));
        }
}
