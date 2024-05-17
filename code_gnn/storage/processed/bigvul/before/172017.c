static l2cap_socket *btsock_l2cap_find_by_id_l(uint32_t id)
{
    l2cap_socket *sock = socks;

 while (sock && sock->id != id)
        sock = sock->next;

 return sock;
}
