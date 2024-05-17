static int sctp_eat_data(const struct sctp_association *asoc,
			 struct sctp_chunk *chunk,
			 sctp_cmd_seq_t *commands)
{
	sctp_datahdr_t *data_hdr;
	struct sctp_chunk *err;
	size_t datalen;
	sctp_verb_t deliver;
	int tmp;
	__u32 tsn;
	struct sctp_tsnmap *map = (struct sctp_tsnmap *)&asoc->peer.tsn_map;
	struct sock *sk = asoc->base.sk;
	struct net *net = sock_net(sk);
	u16 ssn;
	u16 sid;
	u8 ordered = 0;

	data_hdr = chunk->subh.data_hdr = (sctp_datahdr_t *)chunk->skb->data;
	skb_pull(chunk->skb, sizeof(sctp_datahdr_t));

	tsn = ntohl(data_hdr->tsn);
	pr_debug("%s: TSN 0x%x\n", __func__, tsn);

	 

	 

	if (asoc->peer.ecn_capable && !chunk->ecn_ce_done) {
		struct sctp_af *af = SCTP_INPUT_CB(chunk->skb)->af;
		chunk->ecn_ce_done = 1;

		if (af->is_ce(sctp_gso_headskb(chunk->skb))) {
			 
			sctp_add_cmd_sf(commands, SCTP_CMD_ECN_CE,
					SCTP_U32(tsn));
		}
	}

	tmp = sctp_tsnmap_check(&asoc->peer.tsn_map, tsn);
	if (tmp < 0) {
		 
		if (chunk->asoc)
			chunk->asoc->stats.outofseqtsns++;
		return SCTP_IERROR_HIGH_TSN;
	} else if (tmp > 0) {
		 
		sctp_add_cmd_sf(commands, SCTP_CMD_REPORT_DUP, SCTP_U32(tsn));
		return SCTP_IERROR_DUP_TSN;
	}

	 

	 
	datalen = ntohs(chunk->chunk_hdr->length);
	datalen -= sizeof(sctp_data_chunk_t);

	deliver = SCTP_CMD_CHUNK_ULP;

	 
	if ((datalen >= asoc->rwnd) && (!asoc->ulpq.pd_mode)) {

		 
		sctp_add_cmd_sf(commands, SCTP_CMD_PART_DELIVER, SCTP_NULL());
	}

	 
	if ((!chunk->data_accepted) && (!asoc->rwnd || asoc->rwnd_over ||
	    (datalen > asoc->rwnd + asoc->frag_point))) {

		 
		if (sctp_tsnmap_has_gap(map) &&
		    (sctp_tsnmap_get_ctsn(map) + 1) == tsn) {
			pr_debug("%s: reneging for tsn:%u\n", __func__, tsn);
			deliver = SCTP_CMD_RENEGE;
		} else {
			pr_debug("%s: discard tsn:%u len:%zu, rwnd:%d\n",
				 __func__, tsn, datalen, asoc->rwnd);

			return SCTP_IERROR_IGNORE_TSN;
		}
	}

	 
	if (*sk->sk_prot_creator->memory_pressure) {
		if (sctp_tsnmap_has_gap(map) &&
		    (sctp_tsnmap_get_ctsn(map) + 1) == tsn) {
			pr_debug("%s: under pressure, reneging for tsn:%u\n",
				 __func__, tsn);
			deliver = SCTP_CMD_RENEGE;
		 }
	}

	 
	if (unlikely(0 == datalen)) {
		err = sctp_make_abort_no_data(asoc, chunk, tsn);
		if (err) {
			sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
					SCTP_CHUNK(err));
		}
		 
		sctp_add_cmd_sf(commands, SCTP_CMD_DISCARD_PACKET, SCTP_NULL());
		sctp_add_cmd_sf(commands, SCTP_CMD_SET_SK_ERR,
				SCTP_ERROR(ECONNABORTED));
		sctp_add_cmd_sf(commands, SCTP_CMD_ASSOC_FAILED,
				SCTP_PERR(SCTP_ERROR_NO_DATA));
		SCTP_INC_STATS(net, SCTP_MIB_ABORTEDS);
		SCTP_DEC_STATS(net, SCTP_MIB_CURRESTAB);
		return SCTP_IERROR_NO_DATA;
	}

	chunk->data_accepted = 1;

	 
	if (chunk->chunk_hdr->flags & SCTP_DATA_UNORDERED) {
		SCTP_INC_STATS(net, SCTP_MIB_INUNORDERCHUNKS);
		if (chunk->asoc)
			chunk->asoc->stats.iuodchunks++;
	} else {
		SCTP_INC_STATS(net, SCTP_MIB_INORDERCHUNKS);
		if (chunk->asoc)
			chunk->asoc->stats.iodchunks++;
		ordered = 1;
	}

	 
	sid = ntohs(data_hdr->stream);
	if (sid >= asoc->c.sinit_max_instreams) {
		 
		sctp_add_cmd_sf(commands, SCTP_CMD_REPORT_TSN, SCTP_U32(tsn));

		err = sctp_make_op_error(asoc, chunk, SCTP_ERROR_INV_STRM,
					 &data_hdr->stream,
					 sizeof(data_hdr->stream),
					 sizeof(u16));
		if (err)
			sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
					SCTP_CHUNK(err));
		return SCTP_IERROR_BAD_STREAM;
	}

	 
	ssn = ntohs(data_hdr->ssn);
	if (ordered && SSN_lt(ssn, sctp_ssn_peek(&asoc->ssnmap->in, sid))) {
		return SCTP_IERROR_PROTO_VIOLATION;
	}

	 
	sctp_add_cmd_sf(commands, deliver, SCTP_CHUNK(chunk));

	return SCTP_IERROR_NO_ERROR;
}
