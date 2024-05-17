static void on_srv_l2cap_le_connect_l(tBTA_JV_L2CAP_LE_OPEN *p_open, l2cap_socket *sock)
{
    l2cap_socket *accept_rs;
 uint32_t new_listen_id;

    accept_rs = btsock_l2cap_alloc_l(sock->name, (const bt_bdaddr_t*)p_open->rem_bda, FALSE, 0);
 if (accept_rs) {

        new_listen_id = accept_rs->id;
        accept_rs->id = sock->id;
        sock->id = new_listen_id;

        accept_rs->handle = p_open->handle;
        accept_rs->connected = TRUE;
        accept_rs->security = sock->security;
        accept_rs->fixed_chan = sock->fixed_chan;
        accept_rs->channel = sock->channel;

 *(p_open->p_p_cback) = (void*)btsock_l2cap_cbk;
 *(p_open->p_user_data) = (void*)accept_rs->id;

        btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_EXCEPTION, sock->id);
        btsock_thread_add_fd(pth, accept_rs->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                accept_rs->id);
        APPL_TRACE_DEBUG("sending connect signal & app fd:%dto app server to accept() the"
 " connection", accept_rs->app_fd);
        APPL_TRACE_DEBUG("server fd:%d, scn:%d", sock->our_fd, sock->channel);
        send_app_connect_signal(sock->our_fd, &accept_rs->addr, sock->channel, 0,
                accept_rs->app_fd, p_open->tx_mtu);
        accept_rs->app_fd = -1;  
 }
}
