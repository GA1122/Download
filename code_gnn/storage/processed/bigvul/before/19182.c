static void _dns_result_srv(struct dns_ctx *ctx, struct dns_rr_srv *result, void *data) {
    dnsquery_t query = data;
    assert(query != NULL);
    query->query = NULL;

    if (ctx != NULL && result == NULL) {
        log_debug(ZONE, "dns failure for %s@%p: SRV %s (%d)", query->name, query,
            query->s2s->lookup_srv[query->srv_i], dns_status(ctx));
    } else if (result != NULL) {
        int i;

        log_debug(ZONE, "dns response for %s@%p: SRV %s %d (%d)", query->name, query,
            result->dnssrv_qname, result->dnssrv_nrr, result->dnssrv_ttl);

        for (i = 0; i < result->dnssrv_nrr; i++) {
            if (strlen(result->dnssrv_srv[i].name) > 0
                    && result->dnssrv_srv[i].port > 0
                    && result->dnssrv_srv[i].port < 65536) {
                log_debug(ZONE, "dns response for %s@%p: SRV %s[%d] %s/%d (%d/%d)", query->name,
                    query, result->dnssrv_qname, i,
                    result->dnssrv_srv[i].name, result->dnssrv_srv[i].port,
                    result->dnssrv_srv[i].priority, result->dnssrv_srv[i].weight);

                _dns_add_host(query, result->dnssrv_srv[i].name,
                    result->dnssrv_srv[i].port, result->dnssrv_srv[i].priority,
                    result->dnssrv_srv[i].weight, result->dnssrv_ttl);
            }
        }

        free(result);
    }

     
    query->srv_i++;
    if (query->srv_i < query->s2s->lookup_nsrv) {
        log_debug(ZONE, "dns request for %s@%p: SRV %s", query->name, query,
            query->s2s->lookup_srv[query->srv_i]);

        query->query = dns_submit_srv(NULL, query->name, query->s2s->lookup_srv[query->srv_i], "tcp",
            DNS_NOSRCH, _dns_result_srv, query);

         
        if (query->query == NULL)
            _dns_result_srv(ctx, NULL, query);
    } else {
         
        if (xhash_count(query->hosts) > 0) {
            _dns_result_a(NULL, NULL, query);

         
        } else {
            query->cur_host = strdup(query->name);
            query->cur_port = 5269;
            query->cur_prio = 0;
            query->cur_weight = 0;
            query->cur_expiry = 0;
            if (query->s2s->resolve_aaaa) {
                log_debug(ZONE, "dns request for %s@%p: AAAA %s", query->name, query, query->name);

                query->query = dns_submit_a6(NULL, query->name,
                    DNS_NOSRCH, _dns_result_aaaa, query);

                 
                if (query->query == NULL)
                    _dns_result_aaaa(ctx, NULL, query);
            } else {
                log_debug(ZONE, "dns request for %s@%p: A %s", query->name, query, query->name);

                query->query = dns_submit_a4(NULL, query->name,
                    DNS_NOSRCH, _dns_result_a, query);

                 
                if (query->query == NULL)
                    _dns_result_a(ctx, NULL, query);
            }
        }
    }
}