static bt_status_t btsock_l2cap_listen_or_connect(const char *name, const bt_bdaddr_t *addr,
 int channel, int* sock_fd, int flags, char listen)
{
 bt_status_t stat;
 int fixed_chan = 1;
    l2cap_socket *sock;
    tL2CAP_CFG_INFO cfg;

 if (!sock_fd)
 return BT_STATUS_PARM_INVALID;

 if(channel < 0) {
        fixed_chan = 0;
 } else {
        fixed_chan = (channel & L2CAP_MASK_FIXED_CHANNEL) != 0;
        channel &=~ L2CAP_MASK_FIXED_CHANNEL;
 }

 if (!is_inited())
 return BT_STATUS_NOT_READY;

    pthread_mutex_lock(&state_lock);

    sock = btsock_l2cap_alloc_l(name, addr, listen, flags);
 if (!sock)
 return BT_STATUS_NOMEM;

    sock->fixed_chan = fixed_chan;
    sock->channel = channel;

    stat = BT_STATUS_SUCCESS;

  
    memset(&cfg, 0, sizeof(tL2CAP_CFG_INFO));

    cfg.fcr_present = TRUE;
    cfg.fcr = obex_l2c_fcr_opts_def;

  
 if (listen) {
        stat = btSock_start_l2cap_server_l(sock);
 } else {
 if (fixed_chan) {
 if (BTA_JvL2capConnectLE(sock->security, 0, NULL, channel,
                    L2CAP_DEFAULT_MTU, NULL, sock->addr.address, btsock_l2cap_cbk,
 (void*)sock->id) != BTA_JV_SUCCESS)
                stat = BT_STATUS_FAIL;

 } else {
 if (BTA_JvL2capConnect(sock->security, 0, &obex_l2c_etm_opt,
                    channel, L2CAP_MAX_SDU_LENGTH, &cfg, sock->addr.address,
                    btsock_l2cap_cbk, (void*)sock->id) != BTA_JV_SUCCESS)
                stat = BT_STATUS_FAIL;
 }
 }

 if (stat == BT_STATUS_SUCCESS) {
 *sock_fd = sock->app_fd;
  
        sock->app_fd = -1;  
        btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_EXCEPTION,
                sock->id);
 } else {
       btsock_l2cap_free_l(sock);
 }
    pthread_mutex_unlock(&state_lock);

 return stat;
}
