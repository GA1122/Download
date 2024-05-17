static char packet_get_head_l(l2cap_socket *sock, uint8_t **data, uint32_t *len)
{
 struct packet *p = sock->first_packet;

 if (!p)
 return FALSE;

 if (data)
 *data = sock->first_packet->data;
 if (len)
 *len = sock->first_packet->len;
    sock->first_packet = p->next;
 if (sock->first_packet)
        sock->first_packet->prev = NULL;
 else
        sock->last_packet = NULL;

 if(len)
        sock->bytes_buffered -= *len;

    osi_free(p);

 return TRUE;
}
