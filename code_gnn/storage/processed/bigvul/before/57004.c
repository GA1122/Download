static void sctp_cmd_process_operr(sctp_cmd_seq_t *cmds,
				   struct sctp_association *asoc,
				   struct sctp_chunk *chunk)
{
	struct sctp_errhdr *err_hdr;
	struct sctp_ulpevent *ev;

	while (chunk->chunk_end > chunk->skb->data) {
		err_hdr = (struct sctp_errhdr *)(chunk->skb->data);

		ev = sctp_ulpevent_make_remote_error(asoc, chunk, 0,
						     GFP_ATOMIC);
		if (!ev)
			return;

		sctp_ulpq_tail_event(&asoc->ulpq, ev);

		switch (err_hdr->cause) {
		case SCTP_ERROR_UNKNOWN_CHUNK:
		{
			sctp_chunkhdr_t *unk_chunk_hdr;

			unk_chunk_hdr = (sctp_chunkhdr_t *)err_hdr->variable;
			switch (unk_chunk_hdr->type) {
			 
			case SCTP_CID_ASCONF:
				if (asoc->peer.asconf_capable == 0)
					break;

				asoc->peer.asconf_capable = 0;
				sctp_add_cmd_sf(cmds, SCTP_CMD_TIMER_STOP,
					SCTP_TO(SCTP_EVENT_TIMEOUT_T4_RTO));
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}
}