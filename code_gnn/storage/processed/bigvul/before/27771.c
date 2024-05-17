static inline int rds_ib_set_wr_signal_state(struct rds_ib_connection *ic,
					     struct rds_ib_send_work *send,
					     bool notify)
{
	 
	if (ic->i_unsignaled_wrs-- == 0 || notify) {
		ic->i_unsignaled_wrs = rds_ib_sysctl_max_unsig_wrs;
		send->s_wr.send_flags |= IB_SEND_SIGNALED;
		return 1;
	}
	return 0;
}
