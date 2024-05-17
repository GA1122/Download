void out_resolve(s2s_t s2s, char *domain, xht results, time_t expiry) {
    dnscache_t dns;

     
    if(xhash_count(results) == 0) {
        dns = xhash_get(s2s->dnscache, domain);
        if (dns != NULL) {
             
            xhash_free(dns->results);
            dns->query = NULL;
            dns->results = NULL;
            dns->expiry = expiry;
            dns->pending = 0;
        }

        log_write(s2s->log, LOG_NOTICE, "dns lookup for %s failed", domain);

         
        out_bounce_domain_queues(s2s, domain, stanza_err_REMOTE_SERVER_NOT_FOUND);

        xhash_free(results);
        return;
    }

    log_write(s2s->log, LOG_NOTICE, "dns lookup for %s returned %d result%s (ttl %d)",
        domain, xhash_count(results), xhash_count(results)!=1?"s":"", expiry - time(NULL));

     
    dns = xhash_get(s2s->dnscache, domain);

    if(dns == NULL) {
         
        char *punydomain;
        if (idna_to_ascii_8z(domain, &punydomain, 0) == IDNA_SUCCESS) {
            dns = xhash_get(s2s->dnscache, punydomain);
            free(punydomain);
        }
    }

    if(dns == NULL) {
        log_write(s2s->log, LOG_ERR, "weird, never requested %s resolution", domain);
        return;
    }

     
    xhash_free(dns->results);
    dns->query = NULL;
    dns->results = results;
    dns->expiry = expiry;
    dns->pending = 0;

    out_flush_domain_queues(s2s, domain);

     
    if (!s2s->dns_cache_enabled && !dns->pending) {
        xhash_free(dns->results);
        xhash_zap(s2s->dnscache, domain);
        free(dns);
    }
}
