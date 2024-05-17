struct tevent_req *smbXcli_negprot_send(TALLOC_CTX *mem_ctx,
					struct tevent_context *ev,
					struct smbXcli_conn *conn,
					uint32_t timeout_msec,
					enum protocol_types min_protocol,
					enum protocol_types max_protocol)
{
	struct tevent_req *req, *subreq;
	struct smbXcli_negprot_state *state;

	req = tevent_req_create(mem_ctx, &state,
				struct smbXcli_negprot_state);
	if (req == NULL) {
		return NULL;
	}
	state->conn = conn;
	state->ev = ev;
	state->timeout_msec = timeout_msec;

	if (min_protocol == PROTOCOL_NONE) {
		tevent_req_nterror(req, NT_STATUS_INVALID_PARAMETER_MIX);
		return tevent_req_post(req, ev);
	}

	if (max_protocol == PROTOCOL_NONE) {
		tevent_req_nterror(req, NT_STATUS_INVALID_PARAMETER_MIX);
		return tevent_req_post(req, ev);
	}

	if (min_protocol > max_protocol) {
		tevent_req_nterror(req, NT_STATUS_INVALID_PARAMETER_MIX);
		return tevent_req_post(req, ev);
	}

	conn->min_protocol = min_protocol;
	conn->max_protocol = max_protocol;
	conn->protocol = PROTOCOL_NONE;

	if ((min_protocol < PROTOCOL_SMB2_02) &&
	    (max_protocol < PROTOCOL_SMB2_02)) {
		 
		conn->dispatch_incoming = smb1cli_conn_dispatch_incoming;

		subreq = smbXcli_negprot_smb1_subreq(state);
		if (tevent_req_nomem(subreq, req)) {
			return tevent_req_post(req, ev);
		}
		tevent_req_set_callback(subreq, smbXcli_negprot_smb1_done, req);
		return req;
	}

	if ((min_protocol >= PROTOCOL_SMB2_02) &&
	    (max_protocol >= PROTOCOL_SMB2_02)) {
		 
		conn->dispatch_incoming = smb2cli_conn_dispatch_incoming;

		 
		smb2cli_conn_set_max_credits(conn,
			WINDOWS_CLIENT_PURE_SMB2_NEGPROT_INITIAL_CREDIT_ASK);

		subreq = smbXcli_negprot_smb2_subreq(state);
		if (tevent_req_nomem(subreq, req)) {
			return tevent_req_post(req, ev);
		}
		tevent_req_set_callback(subreq, smbXcli_negprot_smb2_done, req);
		return req;
	}

	 
	conn->dispatch_incoming = smbXcli_negprot_dispatch_incoming;

	subreq = smbXcli_negprot_smb1_subreq(state);
	if (tevent_req_nomem(subreq, req)) {
		return tevent_req_post(req, ev);
	}
	tevent_req_set_callback(subreq, smbXcli_negprot_invalid_done, req);
	return req;
}
