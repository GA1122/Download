static void btpan_close_conn(btpan_conn_t* conn)
{
    BTIF_TRACE_API("btpan_close_conn: %p",conn);

 if (conn && conn->state == PAN_STATE_OPEN)
 {
        BTIF_TRACE_DEBUG("btpan_close_conn: PAN_STATE_OPEN");

        conn->state = PAN_STATE_CLOSE;
        btpan_cb.open_count--;

 if (btpan_cb.open_count == 0)
 {
            destroy_tap_read_thread();
 if (btpan_cb.tap_fd != INVALID_FD)
 {
                btpan_tap_close(btpan_cb.tap_fd);
                btpan_cb.tap_fd = INVALID_FD;
 }
 }
 }
}
