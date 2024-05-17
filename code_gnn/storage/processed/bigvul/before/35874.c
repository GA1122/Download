struct sctp_chunk *sctp_make_sack(const struct sctp_association *asoc)
{
	struct sctp_chunk *retval;
	struct sctp_sackhdr sack;
	int len;
	__u32 ctsn;
	__u16 num_gabs, num_dup_tsns;
	struct sctp_association *aptr = (struct sctp_association *)asoc;
	struct sctp_tsnmap *map = (struct sctp_tsnmap *)&asoc->peer.tsn_map;
	struct sctp_gap_ack_block gabs[SCTP_MAX_GABS];
	struct sctp_transport *trans;

	memset(gabs, 0, sizeof(gabs));
	ctsn = sctp_tsnmap_get_ctsn(map);

	pr_debug("%s: sackCTSNAck sent:0x%x\n", __func__, ctsn);

	 
	num_gabs = sctp_tsnmap_num_gabs(map, gabs);
	num_dup_tsns = sctp_tsnmap_num_dups(map);

	 
	sack.cum_tsn_ack	    = htonl(ctsn);
	sack.a_rwnd 		    = htonl(asoc->a_rwnd);
	sack.num_gap_ack_blocks     = htons(num_gabs);
	sack.num_dup_tsns           = htons(num_dup_tsns);

	len = sizeof(sack)
		+ sizeof(struct sctp_gap_ack_block) * num_gabs
		+ sizeof(__u32) * num_dup_tsns;

	 
	retval = sctp_make_control(asoc, SCTP_CID_SACK, 0, len);
	if (!retval)
		goto nodata;

	 
	retval->transport = asoc->peer.last_data_from;

	retval->subh.sack_hdr =
		sctp_addto_chunk(retval, sizeof(sack), &sack);

	 
	if (num_gabs)
		sctp_addto_chunk(retval, sizeof(__u32) * num_gabs,
				 gabs);

	 
	if (num_dup_tsns) {
		aptr->stats.idupchunks += num_dup_tsns;
		sctp_addto_chunk(retval, sizeof(__u32) * num_dup_tsns,
				 sctp_tsnmap_get_dups(map));
	}
	 
	if (++aptr->peer.sack_generation == 0) {
		list_for_each_entry(trans, &asoc->peer.transport_addr_list,
				    transports)
			trans->sack_generation = 0;
		aptr->peer.sack_generation = 1;
	}
nodata:
	return retval;
}
