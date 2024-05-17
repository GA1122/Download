static int sctp_gen_sack(struct sctp_association *asoc, int force,
			 sctp_cmd_seq_t *commands)
{
	__u32 ctsn, max_tsn_seen;
	struct sctp_chunk *sack;
	struct sctp_transport *trans = asoc->peer.last_data_from;
	int error = 0;

	if (force ||
	    (!trans && (asoc->param_flags & SPP_SACKDELAY_DISABLE)) ||
	    (trans && (trans->param_flags & SPP_SACKDELAY_DISABLE)))
		asoc->peer.sack_needed = 1;

	ctsn = sctp_tsnmap_get_ctsn(&asoc->peer.tsn_map);
	max_tsn_seen = sctp_tsnmap_get_max_tsn_seen(&asoc->peer.tsn_map);

	 
	if (max_tsn_seen != ctsn)
		asoc->peer.sack_needed = 1;

	 
	if (!asoc->peer.sack_needed) {
		asoc->peer.sack_cnt++;

		 
		if (trans) {
			 
			if (asoc->peer.sack_cnt >= trans->sackfreq - 1)
				asoc->peer.sack_needed = 1;

			asoc->timeouts[SCTP_EVENT_TIMEOUT_SACK] =
				trans->sackdelay;
		} else {
			 
			if (asoc->peer.sack_cnt >= asoc->sackfreq - 1)
				asoc->peer.sack_needed = 1;

			asoc->timeouts[SCTP_EVENT_TIMEOUT_SACK] =
				asoc->sackdelay;
		}

		 
		sctp_add_cmd_sf(commands, SCTP_CMD_TIMER_RESTART,
				SCTP_TO(SCTP_EVENT_TIMEOUT_SACK));
	} else {
		asoc->a_rwnd = asoc->rwnd;
		sack = sctp_make_sack(asoc);
		if (!sack)
			goto nomem;

		asoc->peer.sack_needed = 0;
		asoc->peer.sack_cnt = 0;

		sctp_add_cmd_sf(commands, SCTP_CMD_REPLY, SCTP_CHUNK(sack));

		 
		sctp_add_cmd_sf(commands, SCTP_CMD_TIMER_STOP,
				SCTP_TO(SCTP_EVENT_TIMEOUT_SACK));
	}

	return error;
nomem:
	error = -ENOMEM;
	return error;
}
