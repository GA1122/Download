static void conn_close(conn *c) {
    assert(c != NULL);

     
    event_del(&c->event);

    if (settings.verbose > 1)
        fprintf(stderr, "<%d connection closed.\n", c->sfd);

    MEMCACHED_CONN_RELEASE(c->sfd);
    close(c->sfd);
    accept_new_conns(true);
    conn_cleanup(c);

     
    if (c->rsize > READ_BUFFER_HIGHWAT || conn_add_to_freelist(c)) {
        conn_free(c);
    }

    STATS_LOCK();
    stats.curr_conns--;
    STATS_UNLOCK();

    return;
}
