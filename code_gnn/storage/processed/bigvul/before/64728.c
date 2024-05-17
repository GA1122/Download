int dns_packet_append_opt(DnsPacket *p, uint16_t max_udp_size, bool edns0_do, int rcode, size_t *start) {
        size_t saved_size;
        int r;

        assert(p);
         
        assert(max_udp_size >= DNS_PACKET_UNICAST_SIZE_MAX);
        assert(rcode >= 0);
        assert(rcode <= _DNS_RCODE_MAX);

        if (p->opt_start != (size_t) -1)
                return -EBUSY;

        assert(p->opt_size == (size_t) -1);

        saved_size = p->size;

         
        r = dns_packet_append_uint8(p, 0, NULL);
        if (r < 0)
                return r;

         
        r = dns_packet_append_uint16(p, DNS_TYPE_OPT, NULL);
        if (r < 0)
                goto fail;

         
        r = dns_packet_append_uint16(p, max_udp_size, NULL);
        if (r < 0)
                goto fail;

         
        r = dns_packet_append_uint16(p, ((uint16_t) rcode & 0x0FF0) << 4, NULL);
        if (r < 0)
                goto fail;

         
        r = dns_packet_append_uint16(p, edns0_do ? EDNS0_OPT_DO : 0, NULL);
        if (r < 0)
                goto fail;

         
        if (edns0_do && !DNS_PACKET_QR(p)) {
                 

                static const uint8_t rfc6975[] = {

                        0, 5,  
                        0, 6,  
                        DNSSEC_ALGORITHM_RSASHA1,
                        DNSSEC_ALGORITHM_RSASHA1_NSEC3_SHA1,
                        DNSSEC_ALGORITHM_RSASHA256,
                        DNSSEC_ALGORITHM_RSASHA512,
                        DNSSEC_ALGORITHM_ECDSAP256SHA256,
                        DNSSEC_ALGORITHM_ECDSAP384SHA384,

                        0, 6,  
                        0, 3,  
                        DNSSEC_DIGEST_SHA1,
                        DNSSEC_DIGEST_SHA256,
                        DNSSEC_DIGEST_SHA384,

                        0, 7,  
                        0, 1,  
                        NSEC3_ALGORITHM_SHA1,
                };

                r = dns_packet_append_uint16(p, sizeof(rfc6975), NULL);
                if (r < 0)
                        goto fail;

                r = dns_packet_append_blob(p, rfc6975, sizeof(rfc6975), NULL);
        } else
                r = dns_packet_append_uint16(p, 0, NULL);
        if (r < 0)
                goto fail;

        DNS_PACKET_HEADER(p)->arcount = htobe16(DNS_PACKET_ARCOUNT(p) + 1);

        p->opt_start = saved_size;
        p->opt_size = p->size - saved_size;

        if (start)
                *start = saved_size;

        return 0;

fail:
        dns_packet_truncate(p, saved_size);
        return r;
}
