int dns_packet_extract(DnsPacket *p) {
        _cleanup_(dns_question_unrefp) DnsQuestion *question = NULL;
        _cleanup_(dns_answer_unrefp) DnsAnswer *answer = NULL;
        _cleanup_(rewind_dns_packet) DnsPacketRewinder rewinder = {};
        unsigned n, i;
        int r;

        if (p->extracted)
                return 0;

        INIT_REWINDER(rewinder, p);
        dns_packet_rewind(p, DNS_PACKET_HEADER_SIZE);

        n = DNS_PACKET_QDCOUNT(p);
        if (n > 0) {
                question = dns_question_new(n);
                if (!question)
                        return -ENOMEM;

                for (i = 0; i < n; i++) {
                        _cleanup_(dns_resource_key_unrefp) DnsResourceKey *key = NULL;
                        bool cache_flush;

                        r = dns_packet_read_key(p, &key, &cache_flush, NULL);
                        if (r < 0)
                                return r;

                        if (cache_flush)
                                return -EBADMSG;

                        if (!dns_type_is_valid_query(key->type))
                                return -EBADMSG;

                        r = dns_question_add(question, key);
                        if (r < 0)
                                return r;
                }
        }

        n = DNS_PACKET_RRCOUNT(p);
        if (n > 0) {
                _cleanup_(dns_resource_record_unrefp) DnsResourceRecord *previous = NULL;
                bool bad_opt = false;

                answer = dns_answer_new(n);
                if (!answer)
                        return -ENOMEM;

                for (i = 0; i < n; i++) {
                        _cleanup_(dns_resource_record_unrefp) DnsResourceRecord *rr = NULL;
                        bool cache_flush = false;

                        r = dns_packet_read_rr(p, &rr, &cache_flush, NULL);
                        if (r < 0)
                                return r;

                         
                        if (previous)
                                dns_resource_key_reduce(&rr->key, &previous->key);

                        if (rr->key->type == DNS_TYPE_OPT) {
                                bool has_rfc6975;

                                if (p->opt || bad_opt) {
                                         
                                        log_debug("Multiple OPT RRs detected, ignoring all.");
                                        bad_opt = true;
                                        continue;
                                }

                                if (!dns_name_is_root(dns_resource_key_name(rr->key))) {
                                         
                                        log_debug("OPT RR is not owned by root domain, ignoring.");
                                        bad_opt = true;
                                        continue;
                                }

                                if (i < DNS_PACKET_ANCOUNT(p) + DNS_PACKET_NSCOUNT(p)) {
                                         
                                        log_debug("OPT RR in wrong section, ignoring.");
                                        bad_opt = true;
                                        continue;
                                }

                                if (!opt_is_good(rr, &has_rfc6975)) {
                                        log_debug("Malformed OPT RR, ignoring.");
                                        bad_opt = true;
                                        continue;
                                }

                                if (DNS_PACKET_QR(p)) {
                                         

                                        if (!DNS_RESOURCE_RECORD_OPT_VERSION_SUPPORTED(rr)) {
                                                 
                                                log_debug("EDNS version newer that our request, bad server.");
                                                return -EBADMSG;
                                        }

                                        if (has_rfc6975) {
                                                 
                                                log_debug("OPT RR contained RFC6975 data, ignoring.");
                                                bad_opt = true;
                                                continue;
                                        }
                                }

                                p->opt = dns_resource_record_ref(rr);
                        } else {

                                 

                                r = dns_answer_add(answer, rr, p->ifindex,
                                                   (i < DNS_PACKET_ANCOUNT(p) ? DNS_ANSWER_CACHEABLE : 0) |
                                                   (p->protocol == DNS_PROTOCOL_MDNS && !cache_flush ? DNS_ANSWER_SHARED_OWNER : 0));
                                if (r < 0)
                                        return r;
                        }

                         
                        dns_resource_record_unref(previous);
                        previous = dns_resource_record_ref(rr);
                }

                if (bad_opt)
                        p->opt = dns_resource_record_unref(p->opt);
        }

        p->question = question;
        question = NULL;

        p->answer = answer;
        answer = NULL;

        p->extracted = true;

         
        return 0;
}
