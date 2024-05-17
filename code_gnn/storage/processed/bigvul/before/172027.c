static void on_l2cap_close(tBTA_JV_L2CAP_CLOSE * p_close, uint32_t id)
{
    l2cap_socket *sock;

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (sock) {
        APPL_TRACE_DEBUG("on_l2cap_close, slot id:%d, fd:%d, %s:%d, server:%d",
                sock->id, sock->our_fd, sock->fixed_chan ? "fixed_chan" : "PSM",
                sock->channel, sock->server);
        sock->handle = 0;
 if(!sock->fixed_chan && (sock->server == TRUE)) {
            BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP);
 }
        btsock_l2cap_free_l(sock);
 }
    pthread_mutex_unlock(&state_lock);
}
