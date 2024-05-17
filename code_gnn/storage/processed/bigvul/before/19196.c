int out_packet(s2s_t s2s, pkt_t pkt) {
    char *rkey;
    int rkeylen;
    conn_t out;
    conn_state_t state;
    int ret;

     
    if (s2s->enable_whitelist > 0 &&
            (pkt->to->domain != NULL) &&
            (s2s_domain_in_whitelist(s2s, pkt->to->domain) == 0)) {
        log_write(s2s->log, LOG_NOTICE, "sending a packet to domain not in the whitelist, dropping it");
        if (pkt->to != NULL)
            jid_free(pkt->to);
        if (pkt->from != NULL)
            jid_free(pkt->from);
        if (pkt->nad != NULL)
            nad_free(pkt->nad);
        free(pkt);

        return;
    }

     
    rkey = s2s_route_key(NULL, pkt->from->domain, pkt->to->domain);
    rkeylen = strlen(rkey);

     
    ret = out_route(s2s, rkey, rkeylen, &out, 1);

    if (out == NULL) {
         
        _out_packet_queue(s2s, pkt);

         
        if (ret) {
             
            out_bounce_route_queue(s2s, rkey, rkeylen, stanza_err_SERVICE_UNAVAILABLE);

            free(rkey);
            return -1;
        }

        free(rkey);
        return 0;
    }

     
    if(!out->online) {
        log_debug(ZONE, "connection in progress, queueing packet");

        _out_packet_queue(s2s, pkt);

        free(rkey);
        return 0;
    }

     
    state = (conn_state_t) xhash_get(out->states, rkey);

     
    if(state == conn_VALID || pkt->db) {
        log_debug(ZONE, "writing packet for %s to outgoing conn %d", rkey, out->fd->fd);

         
        if(pkt->db) {
             
            if(NAD_ENAME_L(pkt->nad, 0) == 6 && strncmp("verify", NAD_ENAME(pkt->nad, 0), 6) == 0) {
                out->verify++;
                out->last_verify = time(NULL);
            }

             
            sx_nad_write(out->s, pkt->nad);
        } else {
             
            int ns = nad_find_namespace(pkt->nad, 1, uri_CLIENT, NULL);
            if(ns >= 0) {
                
               pkt->nad->elems[0].ns = -1;
               pkt->nad->elems[0].my_ns = -1;
               pkt->nad->elems[1].ns = -1;
               pkt->nad->elems[1].my_ns = -1;
            }

             
            sx_nad_write_elem(out->s, pkt->nad, 1);
        }

         
        out->last_packet = time(NULL);

        jid_free(pkt->from);
        jid_free(pkt->to);
        free(pkt);

        free(rkey);
        return 0;
    }

     
    _out_packet_queue(s2s, pkt);

     
    if(state == conn_INPROGRESS) {
        free(rkey);
        return 0;
    }

     
    if (out->s2s->require_tls == 0 || out->s->ssf > 0) {
    _out_dialback(out, rkey, rkeylen);
    }
    free(rkey);
    return 0;
}
