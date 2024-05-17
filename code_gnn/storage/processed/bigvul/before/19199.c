int out_route(s2s_t s2s, char *route, int routelen, conn_t *out, int allow_bad) {
    dnscache_t dns;
    char ipport[INET6_ADDRSTRLEN + 16], *dkey, *c;
    time_t now;
    int reuse = 0;
    char ip[INET6_ADDRSTRLEN] = {0};
    int port, c_len, from_len;

    c = memchr(route, '/', routelen);
    from_len = c - route;
    c++;
    c_len = routelen - (c - route);
    dkey = strndup(c, c_len);

    log_debug(ZONE, "trying to find connection for '%s'", dkey);
    *out = (conn_t) xhash_get(s2s->out_dest, dkey);
    if(*out == NULL) {
        log_debug(ZONE, "connection for '%s' not found", dkey);

         
        dns = xhash_get(s2s->dnscache, dkey);
        if(dns == NULL) {
             
            log_debug(ZONE, "no dns for %s, preparing for resolution", dkey);

            dns = (dnscache_t) calloc(1, sizeof(struct dnscache_st));

            strcpy(dns->name, dkey);

            xhash_put(s2s->dnscache, dns->name, (void *) dns);

#if 0
             
            dns->pending = 0;
            strcpy(dns->ip, "127.0.0.1");
            dns->port = 3000;
            dns->expiry = time(NULL) + 99999999;
#endif
        }

         
        if(dns->pending) {
            log_debug(ZONE, "pending resolution");
            free(dkey);
            return 0;
        }

         
        now = time(NULL);  
        if(now > dns->expiry) {
             
            log_debug(ZONE, "requesting resolution for %s", dkey);

            dns->init_time = time(NULL);
            dns->pending = 1;

            dns_resolve_domain(s2s, dns);
            free(dkey);
            return 0;
        }

         
        if (dns_select(s2s, ip, &port, now, dns, allow_bad)) {
             
            free(dkey);
            return -1;
        }

         
        if (now > dns->expiry) {
             
            log_debug(ZONE, "requesting resolution for %s", dkey);

            dns->init_time = time(NULL);
            dns->pending = 1;

            dns_resolve_domain(s2s, dns);

            free(dkey);
            return 0;
        }

         
        snprintf(ipport, INET6_ADDRSTRLEN + 16, "%s/%d", ip, port);

         
        if (s2s->out_reuse)
            *out = (conn_t) xhash_get(s2s->out_host, ipport);

        if (*out != NULL) {
            log_write(s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] using connection for '%s'", (*out)->fd->fd, (*out)->ip, (*out)->port, dkey);

             
            xhash_put(s2s->out_dest, s2s->out_reuse ? pstrdup(xhash_pool((*out)->routes), dkey) : dkey, (void *) *out);

            reuse = 1;
        } else{
             
            *out = (conn_t) calloc(1, sizeof(struct conn_st));

            (*out)->s2s = s2s;

            (*out)->key = strdup(ipport);
            if (s2s->out_reuse)
                (*out)->dkey = NULL;
            else
                (*out)->dkey = dkey;

            strcpy((*out)->ip, ip);
            (*out)->port = port;

            (*out)->states = xhash_new(101);
            (*out)->states_time = xhash_new(101);

            (*out)->routes = xhash_new(101);

            (*out)->init_time = time(NULL);

            if (s2s->out_reuse)
                xhash_put(s2s->out_host, (*out)->key, (void *) *out);
            xhash_put(s2s->out_dest, s2s->out_reuse ? pstrdup(xhash_pool((*out)->routes), dkey) : dkey, (void *) *out);

            xhash_put((*out)->routes, pstrdupx(xhash_pool((*out)->routes), route, routelen), (void *) 1);

             
            log_debug(ZONE, "initiating connection to %s", ipport);

             
            int ip_is_v6 = 0;
            if (strchr(ip, ':') != NULL)
                ip_is_v6 = 1;
            int i;
            for (i = 0; i < s2s->origin_nips; i++) {
                if ((ip_is_v6 && (strchr(s2s->origin_ips[i], ':') != NULL)) ||           
                            (! ip_is_v6 && (strchr(s2s->origin_ips[i], ':') == NULL)))   
                    (*out)->fd = mio_connect(s2s->mio, port, ip, s2s->origin_ips[i], _out_mio_callback, (void *) *out);

                if ((*out)->fd != NULL) break;
            }

            if ((*out)->fd == NULL) {
                log_write(s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] mio_connect error: %s (%d)", -1, (*out)->ip, (*out)->port, MIO_STRERROR(MIO_ERROR), MIO_ERROR);

                _out_dns_mark_bad(*out);

                if (s2s->out_reuse)
                   xhash_zap(s2s->out_host, (*out)->key);
                xhash_zap(s2s->out_dest, dkey);

                xhash_free((*out)->states);
                xhash_free((*out)->states_time);

                xhash_free((*out)->routes);

                free((*out)->key);
                free((*out)->dkey);
                free(*out);
                *out = NULL;

                 
                return out_route(s2s, route, routelen, out, 0);
            } else {
                log_write(s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] outgoing connection for '%s'", (*out)->fd->fd, (*out)->ip, (*out)->port, dkey);

                (*out)->s = sx_new(s2s->sx_env, (*out)->fd->fd, _out_sx_callback, (void *) *out);

#ifdef HAVE_SSL
                 
                if(s2s->sx_ssl != NULL) {
                    sx_client_init((*out)->s, S2S_DB_HEADER, uri_SERVER, dkey, pstrdupx(xhash_pool((*out)->routes), route, from_len), "1.0");
                } else {
                    sx_client_init((*out)->s, S2S_DB_HEADER, uri_SERVER, NULL, NULL, NULL);
                }
#else
                sx_client_init((*out)->s, S2S_DB_HEADER, uri_SERVER, NULL, NULL, NULL);
#endif
                 
                return 0;
            }
        }
    } else {
        log_debug(ZONE, "connection for '%s' found (%d %s/%d)", dkey, (*out)->fd->fd, (*out)->ip, (*out)->port);
    }

     
    if (!(*out)->online || reuse) {
        if (xhash_getx((*out)->routes, route, routelen) == NULL)
            xhash_put((*out)->routes, pstrdupx(xhash_pool((*out)->routes), route, routelen), (void *) 1);
    }

    free(dkey);
    return 0;
}