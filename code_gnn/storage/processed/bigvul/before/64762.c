int dns_packet_validate_query(DnsPacket *p) {
        int r;

        assert(p);

        r = dns_packet_validate(p);
        if (r < 0)
                return r;

        if (DNS_PACKET_QR(p) != 0)
                return 0;

        if (DNS_PACKET_OPCODE(p) != 0)
                return -EBADMSG;

        if (DNS_PACKET_TC(p))
                return -EBADMSG;

        switch (p->protocol) {

        case DNS_PROTOCOL_LLMNR:
        case DNS_PROTOCOL_DNS:
                 
                if (DNS_PACKET_QDCOUNT(p) != 1)
                        return -EBADMSG;

                 
                if (DNS_PACKET_ANCOUNT(p) > 0)
                        return -EBADMSG;

                 
                if (DNS_PACKET_NSCOUNT(p) > 0)
                        return -EBADMSG;

                break;

        case DNS_PROTOCOL_MDNS:
                 
                if (DNS_PACKET_AA(p)    != 0 ||
                    DNS_PACKET_RD(p)    != 0 ||
                    DNS_PACKET_RA(p)    != 0 ||
                    DNS_PACKET_AD(p)    != 0 ||
                    DNS_PACKET_CD(p)    != 0 ||
                    DNS_PACKET_RCODE(p) != 0)
                        return -EBADMSG;

                break;

        default:
                break;
        }

        return 1;
}
