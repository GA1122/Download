static void sctp_cmd_hb_timer_update(sctp_cmd_seq_t *cmds,
				     struct sctp_transport *t)
{
	 
	if (!mod_timer(&t->hb_timer, sctp_transport_timeout(t)))
		sctp_transport_hold(t);
}