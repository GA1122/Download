static void on_l2cap_connect(tBTA_JV *p_data, uint32_t id)
{
    l2cap_socket *sock;
    tBTA_JV_L2CAP_OPEN *psm_open = &p_data->l2c_open;
    tBTA_JV_L2CAP_LE_OPEN *le_open = &p_data->l2c_le_open;

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (!sock) {
        APPL_TRACE_ERROR("on_l2cap_connect on unknown socket");
 } else {
 if (sock->fixed_chan && le_open->status == BTA_JV_SUCCESS) {
 if (!sock->server)
                on_cl_l2cap_le_connect_l(le_open, sock);
 else
                on_srv_l2cap_le_connect_l(le_open, sock);
 } else if (!sock->fixed_chan && psm_open->status == BTA_JV_SUCCESS) {
 if (!sock->server)
                on_cl_l2cap_psm_connect_l(psm_open, sock);
 else
                on_srv_l2cap_psm_connect_l(psm_open, sock);
 }
 else
            btsock_l2cap_free_l(sock);
 }
    pthread_mutex_unlock(&state_lock);
}
