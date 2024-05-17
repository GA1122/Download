static void sctp_cmd_transport_on(sctp_cmd_seq_t *cmds,
				  struct sctp_association *asoc,
				  struct sctp_transport *t,
				  struct sctp_chunk *chunk)
{
	sctp_sender_hb_info_t *hbinfo;
	int was_unconfirmed = 0;

	 
	t->error_count = 0;

	 
	if (t->asoc->state < SCTP_STATE_SHUTDOWN_PENDING)
		t->asoc->overall_error_count = 0;

	 
	t->hb_sent = 0;

	 
	if ((t->state == SCTP_INACTIVE) || (t->state == SCTP_UNCONFIRMED)) {
		was_unconfirmed = 1;
		sctp_assoc_control_transport(asoc, t, SCTP_TRANSPORT_UP,
					     SCTP_HEARTBEAT_SUCCESS);
	}

	if (t->state == SCTP_PF)
		sctp_assoc_control_transport(asoc, t, SCTP_TRANSPORT_UP,
					     SCTP_HEARTBEAT_SUCCESS);

	 
	if (t->dst)
		dst_confirm(t->dst);

	 
	if (t->rto_pending == 0)
		t->rto_pending = 1;

	hbinfo = (sctp_sender_hb_info_t *) chunk->skb->data;
	sctp_transport_update_rto(t, (jiffies - hbinfo->sent_at));

	 
	if (!mod_timer(&t->hb_timer, sctp_transport_timeout(t)))
		sctp_transport_hold(t);

	if (was_unconfirmed && asoc->peer.transport_count == 1)
		sctp_transport_immediate_rtx(t);
}
