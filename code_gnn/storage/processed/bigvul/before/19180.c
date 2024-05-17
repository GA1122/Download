static void _dns_result_a(struct dns_ctx *ctx, struct dns_rr_a4 *result, void *data) {
    dnsquery_t query = data;
    assert(query != NULL);
    query->query = NULL;

    if (ctx != NULL && result == NULL) {
#define DRA_IP_LEN 16
        char szIP[DRA_IP_LEN];
        if (_etc_hosts_lookup (query->name, szIP, DRA_IP_LEN)) {
            log_debug(ZONE, "/etc/lookup for %s@%p: %s (%d)", query->name,
                query, szIP, query->s2s->etc_hosts_ttl);

            _dns_add_result (query, szIP, query->cur_port,
                query->cur_prio, query->cur_weight, query->s2s->etc_hosts_ttl);
        } else {
            log_debug(ZONE, "dns failure for %s@%p: A %s (%d)", query->name, query,
                query->cur_host, dns_status(ctx));
        }
    } else if (result != NULL) {
        char ip[INET_ADDRSTRLEN];
        int i;

        log_debug(ZONE, "dns response for %s@%p: A %s %d (%d)", query->name,
            query, result->dnsa4_qname, result->dnsa4_nrr, result->dnsa4_ttl);

        if (query->cur_expiry > 0 && result->dnsa4_ttl > query->cur_expiry)
            result->dnsa4_ttl = query->cur_expiry;

        for (i = 0; i < result->dnsa4_nrr; i++) {
            if (inet_ntop(AF_INET, &result->dnsa4_addr[i], ip, INET_ADDRSTRLEN) != NULL) {
                log_debug(ZONE, "dns response for %s@%p: A %s[%d] %s/%d", query->name,
                    query, result->dnsa4_qname, i, ip, query->cur_port);

                _dns_add_result(query, ip, query->cur_port,
                    query->cur_prio, query->cur_weight, result->dnsa4_ttl);
            }
        }

        free(result);
    }

     
    if (xhash_iter_first(query->hosts)) {
        char *ipport, *c, *tmp;
        int ipport_len, ip_len, port_len;
        dnsres_t res;
        union xhashv xhv;

        xhv.dnsres_val = &res;

         
        xhash_iter_get(query->hosts, (const char **) &ipport, &ipport_len, xhv.val);

         
        xhash_iter_zap(query->hosts);

        c = memchr(ipport, '/', ipport_len);
        ip_len = c - ipport;
        c++;
        port_len = ipport_len - (c - ipport);

         
        free(query->cur_host);
        query->cur_host = strndup(ipport, ip_len);
        tmp = strndup(c, port_len);
        query->cur_port = atoi(tmp);
        free(tmp);
        query->cur_prio = res->prio;
        query->cur_weight = res->weight;
        query->cur_expiry = res->expiry;
        log_debug(ZONE, "dns ttl for %s@%p limited to %d", query->name, query, query->cur_expiry);

        if (query->s2s->resolve_aaaa) {
            log_debug(ZONE, "dns request for %s@%p: AAAA %s", query->name, query, query->cur_host);

            query->query = dns_submit_a6(NULL, query->cur_host, DNS_NOSRCH, _dns_result_aaaa, query);

             
            if (query->query == NULL)
                _dns_result_aaaa(ctx, NULL, query);
        } else {
            log_debug(ZONE, "dns request for %s@%p: A %s", query->name, query, query->cur_host);

            query->query = dns_submit_a4(NULL, query->cur_host, DNS_NOSRCH, _dns_result_a, query);

             
            if (query->query == NULL)
                _dns_result_a(ctx, NULL, query);
        }

     
    } else {
        time_t now = time(NULL);
        char *domain;

        free(query->cur_host);
        query->cur_host = NULL;

        log_debug(ZONE, "dns requests for %s@%p complete: %d (%d)", query->name,
            query, xhash_count(query->results), query->expiry);

         
        if (query->expiry > query->s2s->dns_max_ttl)
            query->expiry = query->s2s->dns_max_ttl;

        if (query->expiry < query->s2s->dns_min_ttl)
            query->expiry = query->s2s->dns_min_ttl;

        query->expiry += now;

         
        if (xhash_iter_first(query->results)) {
            union xhashv xhv;
            dnsres_t res;

            xhv.dnsres_val = &res;

            do {
                xhash_iter_get(query->results, NULL, NULL, xhv.val);

                if (res->expiry > query->s2s->dns_max_ttl)
                    res->expiry = query->s2s->dns_max_ttl;

                if (res->expiry < query->s2s->dns_min_ttl)
                    res->expiry = query->s2s->dns_min_ttl;

                res->expiry += now;
            } while(xhash_iter_next(query->results));
        }

        xhash_free(query->hosts);
        query->hosts = NULL;
        if (idna_to_unicode_8z8z(query->name, &domain, 0) != IDNA_SUCCESS) {
            log_write(query->s2s->log, LOG_ERR, "idna dns decode for %s failed", query->name);
             
            xhash_free(query->results);
            query->results = xhash_new(71);
            query->expiry = time(NULL) + 99999999;
            domain = strdup(query->name);
        }
        out_resolve(query->s2s, domain, query->results, query->expiry);
        free(domain);
        free(query->name);
        free(query);
    }
}