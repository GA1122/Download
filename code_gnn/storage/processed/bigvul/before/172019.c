static void btsock_l2cap_free_l(l2cap_socket *sock)
{
 uint8_t *buf;
    l2cap_socket *t = socks;

 while(t && t != sock)
        t = t->next;

 if (!t)  
 return;

 if (sock->next)
        sock->next->prev = sock->prev;

 if (sock->prev)
        sock->prev->next = sock->next;
 else
        socks = sock->next;

    shutdown(sock->our_fd, SHUT_RDWR);
    close(sock->our_fd);
 if (sock->app_fd != -1) {
        close(sock->app_fd);
 } else {
        APPL_TRACE_ERROR("SOCK_LIST: free(id = %d) - NO app_fd!", sock->id);
 }

 while (packet_get_head_l(sock, &buf, NULL))
        osi_free(buf);

 if (sock->handle && (sock->server == FALSE)) {
 if (sock->fixed_chan)
            BTA_JvL2capCloseLE(sock->handle);
 else
            BTA_JvL2capClose(sock->handle);
 }
 if ((sock->channel >= 0) && (sock->server == TRUE)) {
 if (sock->fixed_chan) {
            BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP_LE);
 } else {
            BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP);
 }
 }

    APPL_TRACE_DEBUG("SOCK_LIST: free(id = %d)", sock->id);
    osi_free(sock);
}
