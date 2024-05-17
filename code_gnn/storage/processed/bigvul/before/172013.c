static l2cap_socket *btsock_l2cap_alloc_l(const char *name, const bt_bdaddr_t *addr,
 char is_server, int flags)
{
    l2cap_socket *sock;
 unsigned security = 0;
 int fds[2];

 if (flags & BTSOCK_FLAG_ENCRYPT)
        security |= is_server ? BTM_SEC_IN_ENCRYPT : BTM_SEC_OUT_ENCRYPT;
 if (flags & BTSOCK_FLAG_AUTH)
        security |= is_server ? BTM_SEC_IN_AUTHENTICATE : BTM_SEC_OUT_AUTHENTICATE;
 if (flags & BTSOCK_FLAG_AUTH_MITM)
        security |= is_server ? BTM_SEC_IN_MITM : BTM_SEC_OUT_MITM;
 if (flags & BTSOCK_FLAG_AUTH_16_DIGIT)
        security |= BTM_SEC_IN_MIN_16_DIGIT_PIN;

    sock = osi_calloc(sizeof(*sock));
 if (!sock) {
        APPL_TRACE_ERROR("alloc failed");
 goto fail_alloc;
 }

 if (socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, fds)) {
        APPL_TRACE_ERROR("socketpair failed, errno:%d", errno);
 goto fail_sockpair;
 }

    sock->our_fd = fds[0];
    sock->app_fd = fds[1];
    sock->security = security;
    sock->server = is_server;
    sock->connected = FALSE;
    sock->handle = 0;
    sock->server_psm_sent = FALSE;

 if (name)
        strncpy(sock->name, name, sizeof(sock->name) - 1);
 if (addr)
        sock->addr = *addr;

    sock->first_packet = NULL;
    sock->last_packet = NULL;

    sock->next = socks;
    sock->prev = NULL;
 if (socks)
        socks->prev = sock;
    sock->id = (socks ? socks->id : 0) + 1;
    socks = sock;
  
 while (1) {
        l2cap_socket *t;
        t = socks->next;
 while (t && t->id != sock->id) {
            t = t->next;
 }
 if (!t && sock->id)  
 break;
  
 if (!++sock->id)  
            sock->id++;
 }
    APPL_TRACE_DEBUG("SOCK_LIST: alloc(id = %d)", sock->id);
 return sock;

fail_sockpair:
    osi_free(sock);

fail_alloc:
 return NULL;
}
