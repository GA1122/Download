void rds_ib_advertise_credits(struct rds_connection *conn, unsigned int posted)
{
	struct rds_ib_connection *ic = conn->c_transport_data;

	if (posted == 0)
		return;

	atomic_add(IB_SET_POST_CREDITS(posted), &ic->i_credits);

	 
	if (IB_GET_POST_CREDITS(atomic_read(&ic->i_credits)) >= 16)
		set_bit(IB_ACK_REQUESTED, &ic->i_ack_flags);
}
