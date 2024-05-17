static void release_in_xmit(struct rds_connection *conn)
{
	clear_bit(RDS_IN_XMIT, &conn->c_flags);
	smp_mb__after_atomic();
	 
	if (waitqueue_active(&conn->c_waitq))
		wake_up_all(&conn->c_waitq);
}
