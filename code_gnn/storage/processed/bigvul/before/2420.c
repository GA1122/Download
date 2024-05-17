void smb1cli_req_set_seqnum(struct tevent_req *req, uint32_t seqnum)
{
	struct smbXcli_req_state *state =
		tevent_req_data(req,
		struct smbXcli_req_state);

	state->smb1.seqnum = seqnum;
}
