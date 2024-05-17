static void _dns_add_host(dnsquery_t query, char *ip, int port, int prio, int weight, unsigned int ttl) {
    char *ipport = dns_make_ipport(ip, port);
    dnsres_t res = xhash_get(query->hosts, ipport);

     
    if (weight == 0)
        weight = 1 << 4;
    else
        weight <<= 8;

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

        log_debug(ZONE, "dns host updated for %s@%p: %s (%d/%d/%d)", query->name, query, ipport,
            res->prio, (res->weight >> 8), res->expiry);
    } else if (xhash_count(query->hosts) < DNS_MAX_RESULTS) {
        res = pmalloc(xhash_pool(query->hosts), sizeof(struct dnsres_st));
        res->key = pstrdup(xhash_pool(query->hosts), ipport);
        res->prio = prio;
        res->weight = weight;
        res->expiry = ttl;

        xhash_put(query->hosts, res->key, res);

        log_debug(ZONE, "dns host added for %s@%p: %s (%d/%d/%d)", query->name, query, ipport,
            res->prio, (res->weight >> 8), res->expiry);
    } else {
        log_debug(ZONE, "dns host ignored for %s@%p: %s (%d/%d/%d)", query->name, query, ipport,
            prio, (weight >> 8), ttl);
    }

    free(ipport);
}