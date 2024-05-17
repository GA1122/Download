int dns_select(s2s_t s2s, char *ip, int *port, time_t now, dnscache_t dns, int allow_bad) {
     
    dnsres_t l_reuse[DNS_MAX_RESULTS];
    dnsres_t l_aaaa[DNS_MAX_RESULTS];
    dnsres_t l_a[DNS_MAX_RESULTS];
    dnsres_t l_bad[DNS_MAX_RESULTS];
     
    int rw_reuse[DNS_MAX_RESULTS];
    int rw_aaaa[DNS_MAX_RESULTS];
    int rw_a[DNS_MAX_RESULTS];
    int s_reuse = 0, s_aaaa = 0, s_a = 0, s_bad = 0;  
    int p_reuse = 0, p_aaaa = 0, p_a = 0;  
    int wt_reuse = 0, wt_aaaa = 0, wt_a = 0;  
    int c_expired_good = 0;
    union xhashv xhv;
    dnsres_t res;
    char *ipport;
    int ipport_len;
    char *c;
    int c_len;
    char *tmp;

     

    if (dns->results == NULL) {
        log_debug(ZONE, "negative cache entry for '%s'", dns->name);
        return -1;
    }
    log_debug(ZONE, "selecting DNS result for '%s'", dns->name);

    xhv.dnsres_val = &res;
    if (xhash_iter_first(dns->results)) {
        dnsres_t bad = NULL;
        do {
            xhash_iter_get(dns->results, (const char **) &ipport, &ipport_len, xhv.val);

            if (s2s->dns_bad_timeout > 0)
                bad = xhash_getx(s2s->dns_bad, ipport, ipport_len);

            if (now > res->expiry) {
                 
                if (bad == NULL)
                    c_expired_good++;

                log_debug(ZONE, "host '%s' expired", res->key);
                continue;
            } else if (bad != NULL && !(now > bad->expiry)) {
                 
                l_bad[s_bad++] = res;

                log_debug(ZONE, "host '%s' bad", res->key);
            } else if (s2s->out_reuse && xhash_getx(s2s->out_host, ipport, ipport_len) != NULL) {
                 
                log_debug(ZONE, "host '%s' exists", res->key);
                if (s_reuse == 0 || p_reuse > res->prio) {
                    p_reuse = res->prio;
                    s_reuse = 0;
                    wt_reuse = 0;

                    log_debug(ZONE, "reset prio list, using prio %d", res->prio);
                }
                if (res->prio <= p_reuse) {
                    l_reuse[s_reuse] = res;
                    wt_reuse += res->weight;
                    rw_reuse[s_reuse] = wt_reuse;
                    s_reuse++;

                    log_debug(ZONE, "added host with weight %d (%d), running weight %d",
                        (res->weight >> 8), res->weight, wt_reuse);
                } else {
                    log_debug(ZONE, "ignored host with prio %d", res->prio);
                }
            } else if (memchr(ipport, ':', ipport_len) != NULL) {
                 
                log_debug(ZONE, "host '%s' IPv6", res->key);
                if (s_aaaa == 0 || p_aaaa > res->prio) {
                    p_aaaa = res->prio;
                    s_aaaa = 0;
                    wt_aaaa = 0;

                    log_debug(ZONE, "reset prio list, using prio %d", res->prio);
                }
                if (res->prio <= p_aaaa) {
                    l_aaaa[s_aaaa] = res;
                    wt_aaaa += res->weight;
                    rw_aaaa[s_aaaa] = wt_aaaa;
                    s_aaaa++;

                    log_debug(ZONE, "added host with weight %d (%d), running weight %d",
                        (res->weight >> 8), res->weight, wt_aaaa);
                } else {
                    log_debug(ZONE, "ignored host with prio %d", res->prio);
                }
            } else {
                 
                log_debug(ZONE, "host '%s' IPv4", res->key);
                if (s_a == 0 || p_a > res->prio) {
                    p_a = res->prio;
                    s_a = 0;
                    wt_a = 0;

                    log_debug(ZONE, "reset prio list, using prio %d", res->prio);
                }
                if (res->prio <= p_a) {
                    l_a[s_a] = res;
                    wt_a += res->weight;
                    rw_a[s_a] = wt_a;
                    s_a++;

                    log_debug(ZONE, "added host with weight %d (%d), running weight %d",
                        (res->weight >> 8), res->weight, wt_a);
                } else {
                    log_debug(ZONE, "ignored host with prio %d", res->prio);
                }
            }
        } while(xhash_iter_next(dns->results));
    }

     
    ipport = NULL;
    if (s_reuse > 0) {
        int i, r;

        log_debug(ZONE, "using existing hosts, total weight %d", wt_reuse);
        assert((wt_reuse + 1) > 0);

        r = rand() % (wt_reuse + 1);
        log_debug(ZONE, "random number %d", r);

        for (i = 0; i < s_reuse; i++)
            if (rw_reuse[i] >= r) {
                log_debug(ZONE, "selected host '%s', running weight %d",
                    l_reuse[i]->key, rw_reuse[i]);

                ipport = l_reuse[i]->key;
                break;
            }
    } else if (s_aaaa > 0 && (s_a == 0 || p_aaaa <= p_a)) {
        int i, r;

        log_debug(ZONE, "using IPv6 hosts, total weight %d", wt_aaaa);
        assert((wt_aaaa + 1) > 0);

        r = rand() % (wt_aaaa + 1);
        log_debug(ZONE, "random number %d", r);

        for (i = 0; i < s_aaaa; i++)
            if (rw_aaaa[i] >= r) {
                log_debug(ZONE, "selected host '%s', running weight %d",
                    l_aaaa[i]->key, rw_aaaa[i]);

                ipport = l_aaaa[i]->key;
                break;
            }
    } else if (s_a > 0) {
        int i, r;

        log_debug(ZONE, "using IPv4 hosts, total weight %d", wt_a);
        assert((wt_a + 1) > 0);

        r = rand() % (wt_a + 1);
        log_debug(ZONE, "random number %d", r);

        for (i = 0; i < s_a; i++)
            if (rw_a[i] >= r) {
                log_debug(ZONE, "selected host '%s', running weight %d",
                    l_a[i]->key, rw_a[i]);

                ipport = l_a[i]->key;
                break;
            }
    } else if (s_bad > 0) {
        ipport = l_bad[rand() % s_bad]->key;

        log_debug(ZONE, "using bad hosts, allow_bad=%d", allow_bad);

         
        if (c_expired_good > 0) {
            log_debug(ZONE, "expiring this DNS cache entry, %d expired hosts",
                c_expired_good);

            dns->expiry = 0;
        }

        if (!allow_bad)
            return -1;
    }

     
    assert(ipport != NULL);

     
    ipport_len = strlen(ipport);
    c = strchr(ipport, '/');
    strncpy(ip, ipport, c-ipport);
    ip[c-ipport] = '\0';
    c++;
    c_len = ipport_len - (c - ipport);
    tmp = strndup(c, c_len);
    *port = atoi(tmp);
    free(tmp);

    return 0;
}