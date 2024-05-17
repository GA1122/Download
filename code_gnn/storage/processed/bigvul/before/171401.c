static int local_socket_enqueue(asocket* s, apacket* p) {
    D("LS(%d): enqueue %d", s->id, p->len);

    p->ptr = p->data;

  
 if (s->pkt_first) {
 goto enqueue;
 }

  
 while (p->len > 0) {
 int r = adb_write(s->fd, p->ptr, p->len);
 if (r > 0) {
            p->len -= r;
            p->ptr += r;
 continue;
 }
 if ((r == 0) || (errno != EAGAIN)) {
            D("LS(%d): not ready, errno=%d: %s", s->id, errno, strerror(errno));
            put_apacket(p);
            s->has_write_error = true;
            s->close(s);
 return 1;  
 } else {
 break;
 }
 }

 if (p->len == 0) {
        put_apacket(p);
 return 0;  
 }

enqueue:
    p->next = 0;
 if (s->pkt_first) {
        s->pkt_last->next = p;
 } else {
        s->pkt_first = p;
 }
    s->pkt_last = p;

  
    fdevent_add(&s->fde, FDE_WRITE);

 return 1;  
}
