ssh_packet_need_rekeying(struct ssh *ssh, u_int outbound_packet_len)
{
	struct session_state *state = ssh->state;
	u_int32_t out_blocks;

	 
	if (!state->after_authentication)
		return 0;

	 
	if (ssh->kex == NULL || ssh_packet_is_rekeying(ssh))
		return 0;

	 
	if (ssh->compat & SSH_BUG_NOREKEY)
		return 0;

	 
	if (state->p_send.packets == 0 && state->p_read.packets == 0)
		return 0;

	 
	if (state->rekey_interval != 0 &&
	    state->rekey_time + state->rekey_interval <= monotime())
		return 1;

	 
	if (state->p_send.packets > MAX_PACKETS ||
	    state->p_read.packets > MAX_PACKETS)
		return 1;

	 
	out_blocks = ROUNDUP(outbound_packet_len,
	    state->newkeys[MODE_OUT]->enc.block_size);
	return (state->max_blocks_out &&
	    (state->p_send.blocks + out_blocks > state->max_blocks_out)) ||
	    (state->max_blocks_in &&
	    (state->p_read.blocks > state->max_blocks_in));
}
