sctp_disposition_t sctp_sf_do_prm_requestheartbeat(
					struct net *net,
					const struct sctp_endpoint *ep,
					const struct sctp_association *asoc,
					const sctp_subtype_t type,
					void *arg,
					sctp_cmd_seq_t *commands)
{
	if (SCTP_DISPOSITION_NOMEM == sctp_sf_heartbeat(ep, asoc, type,
				      (struct sctp_transport *)arg, commands))
		return SCTP_DISPOSITION_NOMEM;

	 
	sctp_add_cmd_sf(commands, SCTP_CMD_TRANSPORT_HB_SENT,
			SCTP_TRANSPORT(arg));
	return SCTP_DISPOSITION_CONSUME;
}
