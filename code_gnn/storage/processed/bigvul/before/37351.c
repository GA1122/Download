sctp_disposition_t sctp_sf_eat_sack_6_2(struct net *net,
					const struct sctp_endpoint *ep,
					const struct sctp_association *asoc,
					const sctp_subtype_t type,
					void *arg,
					sctp_cmd_seq_t *commands)
{
	struct sctp_chunk *chunk = arg;
	sctp_sackhdr_t *sackh;
	__u32 ctsn;

	if (!sctp_vtag_verify(chunk, asoc))
		return sctp_sf_pdiscard(net, ep, asoc, type, arg, commands);

	 
	if (!sctp_chunk_length_valid(chunk, sizeof(sctp_sack_chunk_t)))
		return sctp_sf_violation_chunklen(net, ep, asoc, type, arg,
						  commands);

	 
	sackh = sctp_sm_pull_sack(chunk);
	 
	if (!sackh)
		return sctp_sf_pdiscard(net, ep, asoc, type, arg, commands);
	chunk->subh.sack_hdr = sackh;
	ctsn = ntohl(sackh->cum_tsn_ack);

	 
	if (TSN_lt(ctsn, asoc->ctsn_ack_point)) {
		pr_debug("%s: ctsn:%x, ctsn_ack_point:%x\n", __func__, ctsn,
			 asoc->ctsn_ack_point);

		return SCTP_DISPOSITION_DISCARD;
	}

	 
	if (!TSN_lt(ctsn, asoc->next_tsn))
		return sctp_sf_violation_ctsn(net, ep, asoc, type, arg, commands);

	 
	sctp_add_cmd_sf(commands, SCTP_CMD_PROCESS_SACK, SCTP_CHUNK(chunk));

	 
	return SCTP_DISPOSITION_CONSUME;
}
