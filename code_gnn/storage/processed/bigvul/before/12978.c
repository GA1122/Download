ssh_packet_send2(struct ssh *ssh)
{
	struct session_state *state = ssh->state;
	struct packet *p;
	u_char type;
	int r;

	type = sshbuf_ptr(state->outgoing_packet)[5];

	 
	if (state->rekeying) {
		if ((type < SSH2_MSG_TRANSPORT_MIN) ||
		    (type > SSH2_MSG_TRANSPORT_MAX) ||
		    (type == SSH2_MSG_SERVICE_REQUEST) ||
		    (type == SSH2_MSG_SERVICE_ACCEPT)) {
			debug("enqueue packet: %u", type);
			p = calloc(1, sizeof(*p));
			if (p == NULL)
				return SSH_ERR_ALLOC_FAIL;
			p->type = type;
			p->payload = state->outgoing_packet;
			TAILQ_INSERT_TAIL(&state->outgoing, p, next);
			state->outgoing_packet = sshbuf_new();
			if (state->outgoing_packet == NULL)
				return SSH_ERR_ALLOC_FAIL;
			return 0;
		}
	}

	 
	if (type == SSH2_MSG_KEXINIT)
		state->rekeying = 1;

	if ((r = ssh_packet_send2_wrapped(ssh)) != 0)
		return r;

	 
	if (type == SSH2_MSG_NEWKEYS) {
		state->rekeying = 0;
		state->rekey_time = monotime();
		while ((p = TAILQ_FIRST(&state->outgoing))) {
			type = p->type;
			debug("dequeue packet: %u", type);
			sshbuf_free(state->outgoing_packet);
			state->outgoing_packet = p->payload;
			TAILQ_REMOVE(&state->outgoing, p, next);
			free(p);
			if ((r = ssh_packet_send2_wrapped(ssh)) != 0)
				return r;
		}
	}
	return 0;
}
