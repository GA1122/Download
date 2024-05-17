struct tevent_req *smb2cli_req_create(TALLOC_CTX *mem_ctx,
				      struct tevent_context *ev,
				      struct smbXcli_conn *conn,
				      uint16_t cmd,
				      uint32_t additional_flags,
				      uint32_t clear_flags,
				      uint32_t timeout_msec,
				      struct smbXcli_tcon *tcon,
				      struct smbXcli_session *session,
				      const uint8_t *fixed,
				      uint16_t fixed_len,
				      const uint8_t *dyn,
				      uint32_t dyn_len,
				      uint32_t max_dyn_len)
{
	struct tevent_req *req;
	struct smbXcli_req_state *state;
	uint32_t flags = 0;
	uint32_t tid = 0;
	uint64_t uid = 0;
	bool use_channel_sequence = false;
	uint16_t channel_sequence = 0;
	bool use_replay_flag = false;

	req = tevent_req_create(mem_ctx, &state,
				struct smbXcli_req_state);
	if (req == NULL) {
		return NULL;
	}

	state->ev = ev;
	state->conn = conn;
	state->session = session;
	state->tcon = tcon;

	if (conn->smb2.server.capabilities & SMB2_CAP_PERSISTENT_HANDLES) {
		use_channel_sequence = true;
	} else if (conn->smb2.server.capabilities & SMB2_CAP_MULTI_CHANNEL) {
		use_channel_sequence = true;
	}

	if (smbXcli_conn_protocol(conn) >= PROTOCOL_SMB3_00) {
		use_replay_flag = true;
	}

	if (smbXcli_conn_protocol(conn) >= PROTOCOL_SMB3_11) {
		flags |= SMB2_PRIORITY_VALUE_TO_MASK(conn->smb2.io_priority);
	}

	if (session) {
		uid = session->smb2->session_id;

		if (use_channel_sequence) {
			channel_sequence = session->smb2->channel_sequence;
		}

		if (use_replay_flag && session->smb2->replay_active) {
			additional_flags |= SMB2_HDR_FLAG_REPLAY_OPERATION;
		}

		state->smb2.should_sign = session->smb2->should_sign;
		state->smb2.should_encrypt = session->smb2->should_encrypt;

		if (cmd == SMB2_OP_SESSSETUP &&
		    session->smb2_channel.signing_key.length == 0 &&
		    session->smb2->signing_key.length != 0)
		{
			 
			state->smb2.should_sign = true;
		}

		if (cmd == SMB2_OP_SESSSETUP &&
		    session->smb2_channel.signing_key.length == 0) {
			state->smb2.should_encrypt = false;
		}

		if (additional_flags & SMB2_HDR_FLAG_SIGNED) {
			if (session->smb2_channel.signing_key.length == 0) {
				tevent_req_nterror(req, NT_STATUS_NO_USER_SESSION_KEY);
				return req;
			}

			additional_flags &= ~SMB2_HDR_FLAG_SIGNED;
			state->smb2.should_sign = true;
		}
	}

	if (tcon) {
		tid = tcon->smb2.tcon_id;

		if (tcon->smb2.should_sign) {
			state->smb2.should_sign = true;
		}
		if (tcon->smb2.should_encrypt) {
			state->smb2.should_encrypt = true;
		}
	}

	if (state->smb2.should_encrypt) {
		state->smb2.should_sign = false;
	}

	state->smb2.recv_iov = talloc_zero_array(state, struct iovec, 3);
	if (state->smb2.recv_iov == NULL) {
		TALLOC_FREE(req);
		return NULL;
	}

	flags |= additional_flags;
	flags &= ~clear_flags;

	state->smb2.fixed = fixed;
	state->smb2.fixed_len = fixed_len;
	state->smb2.dyn = dyn;
	state->smb2.dyn_len = dyn_len;
	state->smb2.max_dyn_len = max_dyn_len;

	if (state->smb2.should_encrypt) {
		SIVAL(state->smb2.transform, SMB2_TF_PROTOCOL_ID, SMB2_TF_MAGIC);
		SBVAL(state->smb2.transform, SMB2_TF_SESSION_ID, uid);
	}

	SIVAL(state->smb2.hdr, SMB2_HDR_PROTOCOL_ID,	SMB2_MAGIC);
	SSVAL(state->smb2.hdr, SMB2_HDR_LENGTH,		SMB2_HDR_BODY);
	SSVAL(state->smb2.hdr, SMB2_HDR_OPCODE,		cmd);
	SSVAL(state->smb2.hdr, SMB2_HDR_CHANNEL_SEQUENCE, channel_sequence);
	SIVAL(state->smb2.hdr, SMB2_HDR_FLAGS,		flags);
	SIVAL(state->smb2.hdr, SMB2_HDR_PID,		0);  
	SIVAL(state->smb2.hdr, SMB2_HDR_TID,		tid);
	SBVAL(state->smb2.hdr, SMB2_HDR_SESSION_ID,	uid);

	switch (cmd) {
	case SMB2_OP_CANCEL:
		state->one_way = true;
		break;
	case SMB2_OP_BREAK:
		 
		SBVAL(state->smb2.hdr, SMB2_HDR_MESSAGE_ID, UINT64_MAX);
		break;
	}

	if (timeout_msec > 0) {
		struct timeval endtime;

		endtime = timeval_current_ofs_msec(timeout_msec);
		if (!tevent_req_set_endtime(req, ev, endtime)) {
			return req;
		}
	}

	return req;
}
