static void sctp_cmd_assoc_failed(sctp_cmd_seq_t *commands,
				  struct sctp_association *asoc,
				  sctp_event_t event_type,
				  sctp_subtype_t subtype,
				  struct sctp_chunk *chunk,
				  unsigned int error)
{
	struct sctp_ulpevent *event;
	struct sctp_chunk *abort;
	 
	sctp_ulpq_abort_pd(&asoc->ulpq, GFP_ATOMIC);

	if (event_type == SCTP_EVENT_T_CHUNK && subtype.chunk == SCTP_CID_ABORT)
		event = sctp_ulpevent_make_assoc_change(asoc, 0, SCTP_COMM_LOST,
						(__u16)error, 0, 0, chunk,
						GFP_ATOMIC);
	else
		event = sctp_ulpevent_make_assoc_change(asoc, 0, SCTP_COMM_LOST,
						(__u16)error, 0, 0, NULL,
						GFP_ATOMIC);
	if (event)
		sctp_add_cmd_sf(commands, SCTP_CMD_EVENT_ULP,
				SCTP_ULPEVENT(event));

	if (asoc->overall_error_count >= asoc->max_retrans) {
		abort = sctp_make_violation_max_retrans(asoc, chunk);
		if (abort)
			sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
					SCTP_CHUNK(abort));
	}

	sctp_add_cmd_sf(commands, SCTP_CMD_NEW_STATE,
			SCTP_STATE(SCTP_STATE_CLOSED));

	 
	asoc->outqueue.error = error;
	sctp_add_cmd_sf(commands, SCTP_CMD_DELETE_TCB, SCTP_NULL());
}