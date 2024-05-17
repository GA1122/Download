static void on_cl_l2cap_psm_connect_l(tBTA_JV_L2CAP_OPEN *p_open, l2cap_socket *sock)
{
    bd_copy(sock->addr.address, p_open->rem_bda, 0);

 if (!send_app_psm_or_chan_l(sock)) {
        APPL_TRACE_ERROR("send_app_psm_or_chan_l failed");
 return;
 }

 if (send_app_connect_signal(sock->our_fd, &sock->addr, sock->channel, 0, -1, p_open->tx_mtu)) {
        APPL_TRACE_DEBUG("on_l2cap_connect_ind, connect signal sent, slot id:%d, psm:%d,"
 " server:%d", sock->id, sock->channel, sock->server);
        btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD, sock->id);
        sock->connected = TRUE;
 }
 else APPL_TRACE_ERROR("send_app_connect_signal failed");
}
