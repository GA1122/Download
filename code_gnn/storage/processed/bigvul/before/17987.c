ssh_packet_send2(struct ssh *ssh)
{
	struct session_state *state = ssh->state;
	struct packet *p;
	u_char type;
	int r, need_rekey;

	if (sshbuf_len(state->outgoing_packet) < 6)
		return SSH_ERR_INTERNAL_ERROR;
	type = sshbuf_ptr(state->outgoing_packet)[5];
	need_rekey = !ssh_packet_type_is_kex(type) &&
	    ssh_packet_need_rekeying(ssh, sshbuf_len(state->outgoing_packet));

	 
	if ((need_rekey || state->rekeying) && !ssh_packet_type_is_kex(type)) {
		if (need_rekey)
			debug3("%s: rekex triggered", __func__);
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
		if (need_rekey) {
			 
			return kex_start_rekex(ssh);
		}
		return 0;
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
			 
			if (ssh_packet_need_rekeying(ssh,
			    sshbuf_len(p->payload))) {
				debug3("%s: queued packet triggered rekex",
				    __func__);
				return kex_start_rekex(ssh);
			}
			debug("dequeue packet: %u", type);
			sshbuf_free(state->outgoing_packet);
			state->outgoing_packet = p->payload;
			TAILQ_REMOVE(&state->outgoing, p, next);
			memset(p, 0, sizeof(*p));
			free(p);
			if ((r = ssh_packet_send2_wrapped(ssh)) != 0)
				return r;
		}
	}
	return 0;
}
