static void drive_machine(conn *c) {
    bool stop = false;
    int sfd;
    socklen_t addrlen;
    struct sockaddr_storage addr;
    int nreqs = settings.reqs_per_event;
    int res;
    const char *str;
#ifdef HAVE_ACCEPT4
    static int  use_accept4 = 1;
#else
    static int  use_accept4 = 0;
#endif

    assert(c != NULL);

    while (!stop) {

        switch(c->state) {
        case conn_listening:
            addrlen = sizeof(addr);
#ifdef HAVE_ACCEPT4
            if (use_accept4) {
                sfd = accept4(c->sfd, (struct sockaddr *)&addr, &addrlen, SOCK_NONBLOCK);
            } else {
                sfd = accept(c->sfd, (struct sockaddr *)&addr, &addrlen);
            }
#else
            sfd = accept(c->sfd, (struct sockaddr *)&addr, &addrlen);
#endif
            if (sfd == -1) {
                if (use_accept4 && errno == ENOSYS) {
                    use_accept4 = 0;
                    continue;
                }
                perror(use_accept4 ? "accept4()" : "accept()");
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                     
                    stop = true;
                } else if (errno == EMFILE) {
                    if (settings.verbose > 0)
                        fprintf(stderr, "Too many open connections\n");
                    accept_new_conns(false);
                    stop = true;
                } else {
                    perror("accept()");
                    stop = true;
                }
                break;
            }
            if (!use_accept4) {
                if (fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL) | O_NONBLOCK) < 0) {
                    perror("setting O_NONBLOCK");
                    close(sfd);
                    break;
                }
            }

            if (settings.maxconns_fast &&
                stats_state.curr_conns + stats_state.reserved_fds >= settings.maxconns - 1) {
                str = "ERROR Too many open connections\r\n";
                res = write(sfd, str, strlen(str));
                close(sfd);
                STATS_LOCK();
                stats.rejected_conns++;
                STATS_UNLOCK();
            } else {
                dispatch_conn_new(sfd, conn_new_cmd, EV_READ | EV_PERSIST,
                                     DATA_BUFFER_SIZE, c->transport);
            }

            stop = true;
            break;

        case conn_waiting:
            if (!update_event(c, EV_READ | EV_PERSIST)) {
                if (settings.verbose > 0)
                    fprintf(stderr, "Couldn't update event\n");
                conn_set_state(c, conn_closing);
                break;
            }

            conn_set_state(c, conn_read);
            stop = true;
            break;

        case conn_read:
            res = IS_UDP(c->transport) ? try_read_udp(c) : try_read_network(c);

            switch (res) {
            case READ_NO_DATA_RECEIVED:
                conn_set_state(c, conn_waiting);
                break;
            case READ_DATA_RECEIVED:
                conn_set_state(c, conn_parse_cmd);
                break;
            case READ_ERROR:
                conn_set_state(c, conn_closing);
                break;
            case READ_MEMORY_ERROR:  
                 
                break;
            }
            break;

        case conn_parse_cmd :
            if (try_read_command(c) == 0) {
                 
                conn_set_state(c, conn_waiting);
            }

            break;

        case conn_new_cmd:
             

            --nreqs;
            if (nreqs >= 0) {
                reset_cmd_handler(c);
            } else {
                pthread_mutex_lock(&c->thread->stats.mutex);
                c->thread->stats.conn_yields++;
                pthread_mutex_unlock(&c->thread->stats.mutex);
                if (c->rbytes > 0) {
                     
                    if (!update_event(c, EV_WRITE | EV_PERSIST)) {
                        if (settings.verbose > 0)
                            fprintf(stderr, "Couldn't update event\n");
                        conn_set_state(c, conn_closing);
                        break;
                    }
                }
                stop = true;
            }
            break;

        case conn_nread:
            if (c->rlbytes == 0) {
                complete_nread(c);
                break;
            }

             
            if (c->rlbytes < 0) {
                if (settings.verbose) {
                    fprintf(stderr, "Invalid rlbytes to read: len %d\n", c->rlbytes);
                }
                conn_set_state(c, conn_closing);
                break;
            }

            if (!c->item || (((item *)c->item)->it_flags & ITEM_CHUNKED) == 0) {
                 
                if (c->rbytes > 0) {
                    int tocopy = c->rbytes > c->rlbytes ? c->rlbytes : c->rbytes;
                    if (c->ritem != c->rcurr) {
                        memmove(c->ritem, c->rcurr, tocopy);
                    }
                    c->ritem += tocopy;
                    c->rlbytes -= tocopy;
                    c->rcurr += tocopy;
                    c->rbytes -= tocopy;
                    if (c->rlbytes == 0) {
                        break;
                    }
                }

                 
                res = read(c->sfd, c->ritem, c->rlbytes);
                if (res > 0) {
                    pthread_mutex_lock(&c->thread->stats.mutex);
                    c->thread->stats.bytes_read += res;
                    pthread_mutex_unlock(&c->thread->stats.mutex);
                    if (c->rcurr == c->ritem) {
                        c->rcurr += res;
                    }
                    c->ritem += res;
                    c->rlbytes -= res;
                    break;
                }
            } else {
                res = read_into_chunked_item(c);
                if (res > 0)
                    break;
            }

            if (res == 0) {  
                conn_set_state(c, conn_closing);
                break;
            }

            if (res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                if (!update_event(c, EV_READ | EV_PERSIST)) {
                    if (settings.verbose > 0)
                        fprintf(stderr, "Couldn't update event\n");
                    conn_set_state(c, conn_closing);
                    break;
                }
                stop = true;
                break;
            }

             
            if (res == -2) {
                out_of_memory(c, "SERVER_ERROR Out of memory during read");
                c->sbytes = c->rlbytes;
                c->write_and_go = conn_swallow;
                break;
            }
             
            if (settings.verbose > 0) {
                fprintf(stderr, "Failed to read, and not due to blocking:\n"
                        "errno: %d %s \n"
                        "rcurr=%lx ritem=%lx rbuf=%lx rlbytes=%d rsize=%d\n",
                        errno, strerror(errno),
                        (long)c->rcurr, (long)c->ritem, (long)c->rbuf,
                        (int)c->rlbytes, (int)c->rsize);
            }
            conn_set_state(c, conn_closing);
            break;

        case conn_swallow:
             
            if (c->sbytes == 0) {
                conn_set_state(c, conn_new_cmd);
                break;
            }

             
            if (c->rbytes > 0) {
                int tocopy = c->rbytes > c->sbytes ? c->sbytes : c->rbytes;
                c->sbytes -= tocopy;
                c->rcurr += tocopy;
                c->rbytes -= tocopy;
                break;
            }

             
            res = read(c->sfd, c->rbuf, c->rsize > c->sbytes ? c->sbytes : c->rsize);
            if (res > 0) {
                pthread_mutex_lock(&c->thread->stats.mutex);
                c->thread->stats.bytes_read += res;
                pthread_mutex_unlock(&c->thread->stats.mutex);
                c->sbytes -= res;
                break;
            }
            if (res == 0) {  
                conn_set_state(c, conn_closing);
                break;
            }
            if (res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                if (!update_event(c, EV_READ | EV_PERSIST)) {
                    if (settings.verbose > 0)
                        fprintf(stderr, "Couldn't update event\n");
                    conn_set_state(c, conn_closing);
                    break;
                }
                stop = true;
                break;
            }
             
            if (settings.verbose > 0)
                fprintf(stderr, "Failed to read, and not due to blocking\n");
            conn_set_state(c, conn_closing);
            break;

        case conn_write:
             
            if (c->iovused == 0 || (IS_UDP(c->transport) && c->iovused == 1)) {
                if (add_iov(c, c->wcurr, c->wbytes) != 0) {
                    if (settings.verbose > 0)
                        fprintf(stderr, "Couldn't build response\n");
                    conn_set_state(c, conn_closing);
                    break;
                }
            }

             

        case conn_mwrite:
          if (IS_UDP(c->transport) && c->msgcurr == 0 && build_udp_headers(c) != 0) {
            if (settings.verbose > 0)
              fprintf(stderr, "Failed to build UDP headers\n");
            conn_set_state(c, conn_closing);
            break;
          }
            switch (transmit(c)) {
            case TRANSMIT_COMPLETE:
                if (c->state == conn_mwrite) {
                    conn_release_items(c);
                     
                    if(c->protocol == binary_prot) {
                        conn_set_state(c, c->write_and_go);
                    } else {
                        conn_set_state(c, conn_new_cmd);
                    }
                } else if (c->state == conn_write) {
                    if (c->write_and_free) {
                        free(c->write_and_free);
                        c->write_and_free = 0;
                    }
                    conn_set_state(c, c->write_and_go);
                } else {
                    if (settings.verbose > 0)
                        fprintf(stderr, "Unexpected state %d\n", c->state);
                    conn_set_state(c, conn_closing);
                }
                break;

            case TRANSMIT_INCOMPLETE:
            case TRANSMIT_HARD_ERROR:
                break;                    

            case TRANSMIT_SOFT_ERROR:
                stop = true;
                break;
            }
            break;

        case conn_closing:
            if (IS_UDP(c->transport))
                conn_cleanup(c);
            else
                conn_close(c);
            stop = true;
            break;

        case conn_closed:
             
            abort();
            break;

        case conn_watch:
             
            stop = true;
            break;
        case conn_max_state:
            assert(false);
            break;
        }
    }

    return;
}