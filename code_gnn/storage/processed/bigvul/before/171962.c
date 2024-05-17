static void btpan_open_conn(btpan_conn_t* conn, tBTA_PAN *p_data)
{
    BTIF_TRACE_API("btpan_open_conn: local_role:%d, peer_role: %d,  handle:%d, conn: %p",
            p_data->open.local_role, p_data->open.peer_role, p_data->open.handle, conn);

 if (conn == NULL)
        conn = btpan_new_conn(p_data->open.handle, p_data->open.bd_addr, p_data->open.local_role,
                p_data->open.peer_role);
 if (conn)
 {
        BTIF_TRACE_DEBUG("btpan_open_conn:tap_fd:%d, open_count:%d, "
 "conn->handle:%d should = handle:%d, local_role:%d, remote_role:%d",
                btpan_cb.tap_fd, btpan_cb.open_count, conn->handle, p_data->open.handle,
                conn->local_role, conn->remote_role);

        btpan_cb.open_count++;
        conn->handle = p_data->open.handle;
 if (btpan_cb.tap_fd < 0)
 {
            btpan_cb.tap_fd = btpan_tap_open();
 if(btpan_cb.tap_fd >= 0)
                create_tap_read_thread(btpan_cb.tap_fd);
 }

 if (btpan_cb.tap_fd >= 0)
 {
            btpan_cb.flow = 1;
            conn->state = PAN_STATE_OPEN;
 }
 }
}
