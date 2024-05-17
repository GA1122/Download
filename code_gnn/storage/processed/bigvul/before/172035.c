static void on_srv_l2cap_listen_started(tBTA_JV_L2CAP_START *p_start, uint32_t id)
{
    l2cap_socket *sock;

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (sock) {
 if (p_start->status != BTA_JV_SUCCESS) {
            APPL_TRACE_ERROR("Error starting l2cap_listen - status: 0x%04x", p_start->status);
            btsock_l2cap_free_l(sock);
 }
 else {
            sock->handle = p_start->handle;
            APPL_TRACE_DEBUG("on_srv_l2cap_listen_started() sock->handle =%d id:%d",
                    sock->handle, sock->id);
 if(sock->server_psm_sent == FALSE) {
 if (!send_app_psm_or_chan_l(sock)) {
                    APPL_TRACE_DEBUG("send_app_psm() failed, close rs->id:%d", sock->id);
                    btsock_l2cap_free_l(sock);
 } else {
                    sock->server_psm_sent = TRUE;
 }
 }
 }
 }
    pthread_mutex_unlock(&state_lock);
}
