sctp_disposition_t sctp_sf_autoclose_timer_expire(
	struct net *net,
	const struct sctp_endpoint *ep,
	const struct sctp_association *asoc,
	const sctp_subtype_t type,
	void *arg,
	sctp_cmd_seq_t *commands)
{
	int disposition;

	SCTP_INC_STATS(net, SCTP_MIB_AUTOCLOSE_EXPIREDS);

	 
	sctp_add_cmd_sf(commands, SCTP_CMD_NEW_STATE,
			SCTP_STATE(SCTP_STATE_SHUTDOWN_PENDING));

	disposition = SCTP_DISPOSITION_CONSUME;
	if (sctp_outq_is_empty(&asoc->outqueue)) {
		disposition = sctp_sf_do_9_2_start_shutdown(net, ep, asoc, type,
							    arg, commands);
	}
	return disposition;
}