static void _dns_add_result(dnsquery_t query, char *ip, int port, int prio, int weight, unsigned int ttl) {
    char *ipport = dns_make_ipport(ip, port);
    dnsres_t res = xhash_get(query->results, ipport);

    if (res != NULL) {
        if (prio < res->prio)
            res->prio = prio;

        if (prio < res->prio) {
             
            res->weight = weight;
        } else if (prio == res->prio) {
             
            res->weight += weight;
            if (res->weight > (65535 << 8))
                res->weight = (65535 << 8);
        }

        if (ttl > res->expiry)
            res->expiry = ttl;

        if (ttl > query->expiry)
            query->expiry = ttl;

        log_debug(ZONE, "dns result updated for %s@%p: %s (%d/%d/%d)", query->name, query, ipport,
            res->prio, (res->weight >> 8), res->expiry);
    } else if (xhash_count(query->results) < DNS_MAX_RESULTS) {
        res = pmalloc(xhash_pool(query->results), sizeof(struct dnsres_st));
        res->key = pstrdup(xhash_pool(query->results), ipport);
        res->prio = prio;
        res->weight = weight;
        res->expiry = ttl;

        if (ttl > query->expiry)
            query->expiry = ttl;

        xhash_put(query->results, res->key, res);

        log_debug(ZONE, "dns result added for %s@%p: %s (%d/%d/%d)", query->name, query, ipport,
            res->prio, (res->weight >> 8), res->expiry);
    } else {
        log_debug(ZONE, "dns result ignored for %s@%p: %s (%d/%d/%d)", query->name, query, ipport,
            prio, (weight >> 8), ttl);
    }

    free(ipport);
}