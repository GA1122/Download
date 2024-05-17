static char packet_put_tail_l(l2cap_socket *sock, const void *data, uint32_t len)
{
 struct packet *p = packet_alloc((const uint8_t*)data, len);

 if (sock->bytes_buffered >= L2CAP_MAX_RX_BUFFER) {
        ALOGE("packet_put_tail_l: buffer overflow");
 return FALSE;
 }

 if (!p) {
        ALOGE("packet_put_tail_l: unable to allocate packet...");
 return FALSE;
 }

    p->next = NULL;
    p->prev = sock->last_packet;
    sock->last_packet = p;
 if (p->prev)
        p->prev->next = p;
 else
        sock->first_packet = p;

    sock->bytes_buffered += len;

 return TRUE;
}
