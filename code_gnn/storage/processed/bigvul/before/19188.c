static int _out_sx_callback(sx_t s, sx_event_t e, void *data, void *arg) {
    conn_t out = (conn_t) arg;
    sx_buf_t buf = (sx_buf_t) data;
    int len, ns, elem, starttls = 0;
    sx_error_t *sxe;
    nad_t nad;

    switch(e) {
        case event_WANT_READ:
            log_debug(ZONE, "want read");
            mio_read(out->s2s->mio, out->fd);
            break;

        case event_WANT_WRITE:
            log_debug(ZONE, "want write");
            mio_write(out->s2s->mio, out->fd);
            break;

        case event_READ:
            log_debug(ZONE, "reading from %d", out->fd->fd);

             
            len = recv(out->fd->fd, buf->data, buf->len, 0);

            if(len < 0) {
                if(MIO_WOULDBLOCK) {
                    buf->len = 0;
                    return 0;
                }

                log_write(out->s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] read error: %s (%d)", out->fd->fd, out->ip, out->port, MIO_STRERROR(MIO_ERROR), MIO_ERROR);

                if (!out->online) {
                    _out_dns_mark_bad(out);
                }

                sx_kill(s);

                return -1;
            }

            else if(len == 0) {
                 
                sx_kill(s);

                return -1;
            }

            log_debug(ZONE, "read %d bytes", len);

            buf->len = len;

            return len;

        case event_WRITE:
            log_debug(ZONE, "writing to %d", out->fd->fd);

            len = send(out->fd->fd, buf->data, buf->len, 0);
            if(len >= 0) {
                log_debug(ZONE, "%d bytes written", len);
                return len;
            }

            if(MIO_WOULDBLOCK)
                return 0;

            log_write(out->s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] write error: %s (%d)", out->fd->fd, out->ip, out->port, MIO_STRERROR(MIO_ERROR), MIO_ERROR);

            if (!out->online) {
                _out_dns_mark_bad(out);
            }

            sx_kill(s);

            return -1;

        case event_ERROR:
            sxe = (sx_error_t *) data;
            log_write(out->s2s->log, LOG_NOTICE, "[%d] [%s, port=%d] error: %s (%s)", out->fd->fd, out->ip, out->port, sxe->generic, sxe->specific);

             
            if (!out->online ||
                    (sxe->code == SX_ERR_STREAM &&
                        (strstr(sxe->specific, "host-gone") ||         
                         strstr(sxe->specific, "host-unknown") ||      
                         strstr(sxe->specific, "not-authorized") ||    
                         strstr(sxe->specific, "see-other-host") ||    
                         strstr(sxe->specific, "system-shutdown") ||   
                         strstr(sxe->specific, "policy-violation") ||  
                         strstr(sxe->specific, "remote-connection-failed") ||   
                         strstr(sxe->specific, "unsupported-encoding") ||       
                         strstr(sxe->specific, "undefined-condition") ||        
                         strstr(sxe->specific, "internal-server-error") ||      
                         strstr(sxe->specific, "unsupported-version")           
                        ))) {
                _out_dns_mark_bad(out);
            }

            sx_kill(s);

            return -1;

        case event_OPEN:
            log_debug(ZONE, "OPEN event for %s", out->key);
            break;

        case event_STREAM:
             
            log_debug(ZONE, "STREAM event for %s stream version is %s", out->key, out->s->res_version);

             
            if(!out->online) {
                log_debug(ZONE, "outgoing conn to %s is online", out->key);

                 
                 
                if (((out->s->res_version==NULL) || (out->s2s->sx_ssl == NULL)) && out->s2s->require_tls == 0) {
                     log_debug(ZONE, "no stream version, sending dialbacks for %s immediately", out->key);
                     out->online = 1;
                     send_dialbacks(out);
                } else
                     log_debug(ZONE, "outgoing conn to %s - waiting for STREAM features", out->key);
            }

            break;

        case event_PACKET:
             
            out->packet_count++;
            out->s2s->packet_count++;

            nad = (nad_t) data;

             
            if ((out->s->res_version!=NULL)
                 && NAD_NURI_L(nad, NAD_ENS(nad, 0)) == strlen(uri_STREAMS)
                 && strncmp(uri_STREAMS, NAD_NURI(nad, NAD_ENS(nad, 0)), strlen(uri_STREAMS)) == 0
                 && NAD_ENAME_L(nad, 0) == 8 && strncmp("features", NAD_ENAME(nad, 0), 8) == 0) {
                log_debug(ZONE, "got the stream features packet");

#ifdef HAVE_SSL
                 
                if(out->s2s->sx_ssl != NULL && s->ssf == 0) {
                    ns = nad_find_scoped_namespace(nad, uri_TLS, NULL);
                    if(ns >= 0) {
                        elem = nad_find_elem(nad, 0, ns, "starttls", 1);
                        if(elem >= 0) {
                            log_debug(ZONE, "got STARTTLS in stream features");
                            if(sx_ssl_client_starttls(out->s2s->sx_ssl, s, out->s2s->local_pemfile) == 0) {
                                starttls = 1;
                                nad_free(nad);
                                return 0;
                            }
                            log_write(out->s2s->log, LOG_ERR, "unable to establish encrypted session with peer");
                        }
                    }
                }

                 
                if (!starttls) {
                    if (out->s2s->require_tls == 0 || s->ssf > 0) {
                     log_debug(ZONE, "No STARTTLS, sending dialbacks for %s", out->key);
                     out->online = 1;
                     send_dialbacks(out);
                    } else {
                        log_debug(ZONE, "No STARTTLS, dialbacks disabled for non-TLS connections, cannot complete negotiation");
                    }
                }
#else
                if (out->s2s->require_tls == 0) {
                out->online = 1;
                send_dialbacks(out);
                }
#endif
            }


             
            if(NAD_ENS(nad, 0) < 0 || NAD_NURI_L(nad, NAD_ENS(nad, 0)) != uri_DIALBACK_L || strncmp(uri_DIALBACK, NAD_NURI(nad, NAD_ENS(nad, 0)), uri_DIALBACK_L) != 0) {
                log_debug(ZONE, "got a non-dialback packet on an outgoing conn, dropping it");
                nad_free(nad);
                return 0;
            }

             
            if(NAD_ENAME_L(nad, 0) == 6) {
                if(strncmp("result", NAD_ENAME(nad, 0), 6) == 0) {
                    _out_result(out, nad);
                    return 0;
                }

                if(strncmp("verify", NAD_ENAME(nad, 0), 6) == 0) {
                    _out_verify(out, nad);
                    return 0;
                }
            }

            log_debug(ZONE, "unknown dialback packet, dropping it");

            nad_free(nad);
            return 0;

        case event_CLOSED:
            if (out->fd != NULL) {
            mio_close(out->s2s->mio, out->fd);
                out->fd = NULL;
            }
            return -1;
    }

    return 0;
}