static void sctp_cmd_init_failed(sctp_cmd_seq_t *commands,
				 struct sctp_association *asoc,
				 unsigned int error)
{
	struct sctp_ulpevent *event;

	event = sctp_ulpevent_make_assoc_change(asoc, 0, SCTP_CANT_STR_ASSOC,
						(__u16)error, 0, 0, NULL,
						GFP_ATOMIC);

	if (event)
		sctp_add_cmd_sf(commands, SCTP_CMD_EVENT_ULP,
				SCTP_ULPEVENT(event));

	sctp_add_cmd_sf(commands, SCTP_CMD_NEW_STATE,
			SCTP_STATE(SCTP_STATE_CLOSED));

	 
	asoc->outqueue.error = error;
	sctp_add_cmd_sf(commands, SCTP_CMD_DELETE_TCB, SCTP_NULL());
}