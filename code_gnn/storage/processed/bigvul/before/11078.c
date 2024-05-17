static void smbd_deferred_open_timer(struct event_context *ev,
				     struct timed_event *te,
				     struct timeval _tval,
				     void *private_data)
{
	struct pending_message_list *msg = talloc_get_type(private_data,
					   struct pending_message_list);
	TALLOC_CTX *mem_ctx = talloc_tos();
	uint16_t mid = SVAL(msg->buf.data,smb_mid);
	uint8_t *inbuf;

	inbuf = (uint8_t *)talloc_memdup(mem_ctx, msg->buf.data,
					 msg->buf.length);
	if (inbuf == NULL) {
		exit_server("smbd_deferred_open_timer: talloc failed\n");
		return;
	}

	 
	DEBUG(5,("smbd_deferred_open_timer: trigger mid %u.\n",
		(unsigned int)mid ));

	 
	msg->processed = true;

	process_smb(smbd_server_conn, inbuf,
		    msg->buf.length, 0,
		    msg->seqnum, msg->encrypted, &msg->pcd);

	 
	msg = get_open_deferred_message(mid);
	if (msg && msg->processed) {
		remove_deferred_open_smb_message(mid);
	}
}