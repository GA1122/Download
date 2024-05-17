static bt_status_t btSock_start_l2cap_server_l(l2cap_socket *sock) {
    tL2CAP_CFG_INFO cfg;
 bt_status_t stat = BT_STATUS_SUCCESS;
  
    memset(&cfg, 0, sizeof(tL2CAP_CFG_INFO));

    cfg.fcr_present = TRUE;
    cfg.fcr = obex_l2c_fcr_opts_def;

 if (sock->fixed_chan) {

 if (BTA_JvL2capStartServerLE(sock->security, 0, NULL, sock->channel,
                L2CAP_DEFAULT_MTU, NULL, btsock_l2cap_cbk, (void*)sock->id)
 != BTA_JV_SUCCESS)
            stat = BT_STATUS_FAIL;

 } else {
  
 if(sock->channel < 0) {
 if(BTA_JvGetChannelId(BTA_JV_CONN_TYPE_L2CAP, (void*)sock->id, 0)
 != BTA_JV_SUCCESS)
                stat = BT_STATUS_FAIL;
 } else {
 if (BTA_JvL2capStartServer(sock->security, 0, &obex_l2c_etm_opt,
                    sock->channel, L2CAP_MAX_SDU_LENGTH, &cfg, btsock_l2cap_cbk, (void*)sock->id)
 != BTA_JV_SUCCESS)
                stat = BT_STATUS_FAIL;
 }
 }
 return stat;
}
