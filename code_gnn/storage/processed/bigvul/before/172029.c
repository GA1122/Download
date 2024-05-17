static void on_l2cap_data_ind(tBTA_JV *evt, uint32_t id)
{
    l2cap_socket *sock;

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (sock) {
 if (sock->fixed_chan) {  

            tBTA_JV_LE_DATA_IND *p_le_data_ind = &evt->le_data_ind;
            BT_HDR *p_buf = p_le_data_ind->p_buf;
 uint8_t *data = (uint8_t*)(p_buf + 1) + p_buf->offset;

 if (packet_put_tail_l(sock, data, p_buf->len))
                btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_WR, sock->id);
 else { 
                APPL_TRACE_DEBUG("on_l2cap_data_ind() unable to push data to socket - closing"
 " fixed channel");
                BTA_JvL2capCloseLE(sock->handle);
                btsock_l2cap_free_l(sock);
 }

 } else {

            tBTA_JV_DATA_IND *p_data_ind = &evt->data_ind;
            UINT8 buffer[L2CAP_MAX_SDU_LENGTH];
            UINT32  count;

 if (BTA_JvL2capReady(sock->handle, &count) == BTA_JV_SUCCESS) {
 if (BTA_JvL2capRead(sock->handle, sock->id, buffer, count) == BTA_JV_SUCCESS) {
 if (packet_put_tail_l(sock, buffer, count))
                        btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_WR,
                                sock->id);
 else { 
                        APPL_TRACE_DEBUG("on_l2cap_data_ind() unable to push data to socket"
 " - closing channel");
                        BTA_JvL2capClose(sock->handle);
                        btsock_l2cap_free_l(sock);
 }
 }
 }
 }
 }
    pthread_mutex_unlock(&state_lock);
}
