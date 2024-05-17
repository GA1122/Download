void smbXcli_req_unset_pending(struct tevent_req *req)
{
	struct smbXcli_req_state *state =
		tevent_req_data(req,
		struct smbXcli_req_state);
	struct smbXcli_conn *conn = state->conn;
	size_t num_pending = talloc_array_length(conn->pending);
	size_t i;

	TALLOC_FREE(state->write_req);

	if (state->smb1.mid != 0) {
		 
		return;
	}

	tevent_req_set_cleanup_fn(req, NULL);

	if (num_pending == 1) {
		 
		TALLOC_FREE(conn->pending);
		conn->read_smb_req = NULL;
		return;
	}

	for (i=0; i<num_pending; i++) {
		if (req == conn->pending[i]) {
			break;
		}
	}
	if (i == num_pending) {
		 
		return;
	}

	 
	for (; i < (num_pending - 1); i++) {
		conn->pending[i] = conn->pending[i+1];
	}

	 
	conn->pending = talloc_realloc(NULL, conn->pending, struct tevent_req *,
				       num_pending - 1);
	return;
}
