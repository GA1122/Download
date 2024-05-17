sctp_disposition_t sctp_sf_do_8_5_1_E_sa(struct net *net,
				      const struct sctp_endpoint *ep,
				      const struct sctp_association *asoc,
				      const sctp_subtype_t type,
				      void *arg,
				      sctp_cmd_seq_t *commands)
{
	struct sctp_chunk *chunk = arg;

	 
	if (!sctp_chunk_length_valid(chunk, sizeof(sctp_chunkhdr_t)))
		return sctp_sf_violation_chunklen(net, ep, asoc, type, arg,
						  commands);

	 
	SCTP_INC_STATS(net, SCTP_MIB_OUTOFBLUES);

	return sctp_sf_shut_8_4_5(net, ep, NULL, type, arg, commands);
}
