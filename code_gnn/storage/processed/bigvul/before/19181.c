static void _dns_result_aaaa(struct dns_ctx *ctx, struct dns_rr_a6 *result, void *data) {
    dnsquery_t query = data;
    char ip[INET6_ADDRSTRLEN];
    int i;
    assert(query != NULL);
    query->query = NULL;

    if (ctx != NULL && result == NULL) {
        log_debug(ZONE, "dns failure for %s@%p: AAAA %s (%d)", query->name, query,
            query->cur_host, dns_status(ctx));
    } else if (result != NULL) {
        log_debug(ZONE, "dns response for %s@%p: AAAA %s %d (%d)", query->name, query,
            result->dnsa6_qname, result->dnsa6_nrr, result->dnsa6_ttl);

        if (query->cur_expiry > 0 && result->dnsa6_ttl > query->cur_expiry)
            result->dnsa6_ttl = query->cur_expiry;

        for (i = 0; i < result->dnsa6_nrr; i++) {
            if (inet_ntop(AF_INET6, &result->dnsa6_addr[i], ip, INET6_ADDRSTRLEN) != NULL) {
                log_debug(ZONE, "dns response for %s@%p: AAAA %s[%d] %s/%d", query->name,
                    query, result->dnsa6_qname, i, ip, query->cur_port);

                _dns_add_result(query, ip, query->cur_port,
                    query->cur_prio, query->cur_weight, result->dnsa6_ttl);
            }
        }
    }

    if (query->cur_host != NULL) {
         
        log_debug(ZONE, "dns request for %s@%p: A %s", query->name, query, query->cur_host);

        query->query = dns_submit_a4(NULL, query->cur_host,
            DNS_NOSRCH, _dns_result_a, query);

         
        if (query->query == NULL)
            _dns_result_a(ctx, NULL, query);
    } else {
         
        log_debug(ZONE, "dns result for %s@%p: AAAA host vanished...", query->name, query);
        _dns_result_a(NULL, NULL, query);
    }

    free(result);
}