 static void local_socket_destroy(asocket* s) {
     apacket *p, *n;
 int exit_on_close = s->exit_on_close;

    D("LS(%d): destroying fde.fd=%d", s->id, s->fde.fd);

  
    fdevent_remove(&s->fde);

  
 for (p = s->pkt_first; p; p = n) {
        D("LS(%d): discarding %d bytes", s->id, p->len);
        n = p->next;
        put_apacket(p);
 }
    remove_socket(s);
    free(s);

 if (exit_on_close) {
        D("local_socket_destroy: exiting");
        exit(1);

     }
 }
