int dns_packet_append_label(DnsPacket *p, const char *d, size_t l, bool canonical_candidate, size_t *start) {
        uint8_t *w;
        int r;

         

        assert(p);
        assert(d);

        if (l > DNS_LABEL_MAX)
                return -E2BIG;

        r = dns_packet_extend(p, 1 + l, (void**) &w, start);
        if (r < 0)
                return r;

        *(w++) = (uint8_t) l;

        if (p->canonical_form && canonical_candidate) {
                size_t i;

                 

                for (i = 0; i < l; i++)
                        w[i] = (uint8_t) ascii_tolower(d[i]);
        } else
                 
                memcpy(w, d, l);

        return 0;
}
