sctp_disposition_t sctp_sf_do_5_2_2_dupinit(struct net *net,
					const struct sctp_endpoint *ep,
					const struct sctp_association *asoc,
					const sctp_subtype_t type,
					void *arg,
					sctp_cmd_seq_t *commands)
{
	 
	return sctp_sf_do_unexpected_init(net, ep, asoc, type, arg, commands);
}
