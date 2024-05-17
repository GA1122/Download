static void si_conn_send_cb(struct connection *conn)
{
	struct stream_interface *si = conn->owner;
	struct channel *chn = si->ob;

	if (conn->flags & CO_FL_ERROR)
		return;

	if (conn->flags & CO_FL_HANDSHAKE)
		 
		return;

	 
	if (chn->flags & CF_SHUTW)
		return;

	 
	si_conn_send(conn);

	 
	return;
}
