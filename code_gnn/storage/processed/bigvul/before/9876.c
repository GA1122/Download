static int si_idle_conn_wake_cb(struct connection *conn)
{
	struct stream_interface *si = conn->owner;

	if (!conn_ctrl_ready(conn))
		return 0;

	if (conn->flags & (CO_FL_ERROR | CO_FL_SOCK_RD_SH)) {
		 
		conn_force_close(conn);
		conn_free(conn);
		si->end = NULL;
		return -1;
	}
	return 0;
}
