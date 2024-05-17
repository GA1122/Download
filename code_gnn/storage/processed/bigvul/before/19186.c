static int _out_mio_callback(mio_t m, mio_action_t a, mio_fd_t fd, void *data, void *arg) {
    conn_t out = (conn_t) arg;
    char ipport[INET6_ADDRSTRLEN + 17];
    int nbytes;

    switch(a) {
        case action_READ:
            log_debug(ZONE, "read action on fd %d", fd->fd);

             
            out->last_activity = time(NULL);

            ioctl(fd->fd, FIONREAD, &nbytes);
            if(nbytes == 0) {
                sx_kill(out->s);
                return 0;
            }

            return sx_can_read(out->s);

        case action_WRITE:
            log_debug(ZONE, "write action on fd %d", fd->fd);

             
            out->last_activity = time(NULL);

            return sx_can_write(out->s);

        case action_CLOSE:
            log_debug(ZONE, "close action on fd %d", fd->fd);

            jqueue_push(out->s2s->dead, (void *) out->s, 0);

            log_write(out->s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] disconnect, packets: %i", fd->fd, out->ip, out->port, out->packet_count);


            if (out->s2s->out_reuse) {
                 
                snprintf(ipport, INET6_ADDRSTRLEN + 16, "%s/%d", out->ip, out->port);

                xhash_zap(out->s2s->out_host, ipport);
            }

            if (xhash_iter_first(out->routes)) {
                char *rkey;
                int rkeylen;
                char *c;
                int c_len;

                 
                do {
                    xhash_iter_get(out->routes, (const char **) &rkey, &rkeylen, NULL);
                    c = memchr(rkey, '/', rkeylen);
                    c++;
                    c_len = rkeylen - (c - rkey);

                    log_debug(ZONE, "route '%.*s'", rkeylen, rkey);
                    if (xhash_getx(out->s2s->out_dest, c, c_len) != NULL) {
                        log_debug(ZONE, "removing dest entry for '%.*s'", c_len, c);
                        xhash_zapx(out->s2s->out_dest, c, c_len);
                    }
                } while(xhash_iter_next(out->routes));
            }

            if (xhash_iter_first(out->routes)) {
                char *rkey;
                int rkeylen;
                jqueue_t q;
                int npkt;

                 
                do {
                    xhash_iter_get(out->routes, (const char **) &rkey, &rkeylen, NULL);

                    q = xhash_getx(out->s2s->outq, rkey, rkeylen);
                    if (out->s2s->retry_limit > 0 && q != NULL && jqueue_age(q) > out->s2s->retry_limit) {
                        log_write(out->s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] retry limit reached for '%.*s' queue", fd->fd, out->ip, out->port, rkeylen, rkey);
                        q = NULL;
                    }

                    if (q != NULL && (npkt = jqueue_size(q)) > 0 && xhash_get(out->states, rkey) != (void*) conn_INPROGRESS) {
                        conn_t retry;

                        log_debug(ZONE, "retrying connection for '%.*s' queue", rkeylen, rkey);
                        if (!out_route(out->s2s, rkey, rkeylen, &retry, 0)) {
                            log_debug(ZONE, "retry successful");

                            if (retry != NULL) {
                                 
                                out_flush_route_queue(out->s2s, rkey, rkeylen);
                            }
                        } else {
                            log_debug(ZONE, "retry failed");

                             
                            out_bounce_route_queue(out->s2s, rkey, rkeylen, stanza_err_SERVICE_UNAVAILABLE);
                            _out_dns_mark_bad(out);
                        }
                    } else {
                         
                        out_bounce_route_queue(out->s2s, rkey, rkeylen, stanza_err_REMOTE_SERVER_TIMEOUT);
                        _out_dns_mark_bad(out);
                    }
                } while(xhash_iter_next(out->routes));
            }

            jqueue_push(out->s2s->dead_conn, (void *) out, 0);

        case action_ACCEPT:
            break;
    }

    return 0;
}