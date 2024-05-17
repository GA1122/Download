static char packet_put_head_l(l2cap_socket *sock, const void *data, uint32_t len)
{
 struct packet *p = packet_alloc((const uint8_t*)data, len);

  

 if (!p)
 return FALSE;

    p->prev = NULL;
    p->next = sock->first_packet;
    sock->first_packet = p;
 if (p->next)
        p->next->prev = p;
 else
        sock->last_packet = p;

    sock->bytes_buffered += len;

 return TRUE;
}
