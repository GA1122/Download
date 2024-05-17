sctp_disposition_t sctp_sf_shutdown_ack_sent_prm_abort(
	struct net *net,
	const struct sctp_endpoint *ep,
	const struct sctp_association *asoc,
	const sctp_subtype_t type,
	void *arg,
	sctp_cmd_seq_t *commands)
{
	 
	return sctp_sf_shutdown_sent_prm_abort(net, ep, asoc, type, arg, commands);
}
