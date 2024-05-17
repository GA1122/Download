static void _out_dialback(conn_t out, char *rkey, int rkeylen) {
    char *c, *dbkey, *tmp;
    nad_t nad;
    int elem, ns;
    int from_len, to_len;
    time_t now;

    now = time(NULL);

    c = memchr(rkey, '/', rkeylen);
    from_len = c - rkey;
    c++;
    to_len = rkeylen - (c - rkey);

     
    tmp = strndup(c, to_len);
    dbkey = s2s_db_key(NULL, out->s2s->local_secret, tmp, out->s->id);
    free(tmp);

    nad = nad_new();

     
    ns = nad_add_namespace(nad, uri_DIALBACK, "db");
    elem = nad_append_elem(nad, ns, "result", 0);
    nad_set_attr(nad, elem, -1, "from", rkey, from_len);
    nad_set_attr(nad, elem, -1, "to", c, to_len);
    nad_append_cdata(nad, dbkey, strlen(dbkey), 1);

    log_debug(ZONE, "sending auth request for %.*s (key %s)", rkeylen, rkey, dbkey);
    log_write(out->s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] sending dialback auth request for route '%.*s'", out->fd->fd, out->ip, out->port, rkeylen, rkey);

     
    sx_nad_write(out->s, nad);

    free(dbkey);

     
    xhash_put(out->states, pstrdupx(xhash_pool(out->states), rkey, rkeylen), (void *) conn_INPROGRESS);

     
    xhash_put(out->states_time, pstrdupx(xhash_pool(out->states_time), rkey, rkeylen), (void *) now);
}