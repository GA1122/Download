void out_flush_route_queue(s2s_t s2s, char *rkey, int rkeylen) {
    jqueue_t q;
    pkt_t pkt;
    int npkt, i, ret;

    q = xhash_getx(s2s->outq, rkey, rkeylen);
    if(q == NULL)
        return;

    npkt = jqueue_size(q);
    log_debug(ZONE, "flushing %d packets for '%.*s' to out_packet", npkt, rkeylen, rkey);

    for(i = 0; i < npkt; i++) {
        pkt = jqueue_pull(q);
        if(pkt) {
            ret = out_packet(s2s, pkt);
            if (ret) {
                 
                return;
            }
        }
    }

     
    if (jqueue_size(q) == 0) {
        log_debug(ZONE, "deleting out packet queue for '%.*s'", rkeylen, rkey);
        rkey = q->key;
        jqueue_free(q);
        xhash_zap(s2s->outq, rkey);
        free(rkey);
    } else {
        log_debug(ZONE, "emptied queue gained more packets...");
    }
}