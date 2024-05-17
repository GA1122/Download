static struct tevent_req *smb2cli_conn_find_pending(struct smbXcli_conn *conn,
						    uint64_t mid)
{
	size_t num_pending = talloc_array_length(conn->pending);
	size_t i;

	for (i=0; i<num_pending; i++) {
		struct tevent_req *req = conn->pending[i];
		struct smbXcli_req_state *state =
			tevent_req_data(req,
			struct smbXcli_req_state);

		if (mid == BVAL(state->smb2.hdr, SMB2_HDR_MESSAGE_ID)) {
			return req;
		}
	}
	return NULL;
}
