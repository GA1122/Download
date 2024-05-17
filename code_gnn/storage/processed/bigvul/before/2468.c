static void smbXcli_conn_samba_suicide_cleanup(struct tevent_req *req,
					       enum tevent_req_state req_state)
{
	struct smbXcli_conn_samba_suicide_state *state = tevent_req_data(
		req, struct smbXcli_conn_samba_suicide_state);

	TALLOC_FREE(state->write_req);

	if (state->conn == NULL) {
		return;
	}

	if (state->conn->suicide_req == req) {
		state->conn->suicide_req = NULL;
	}
	state->conn = NULL;
}
