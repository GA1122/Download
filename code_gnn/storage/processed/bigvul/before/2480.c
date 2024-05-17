static void smbXcli_negprot_invalid_done(struct tevent_req *subreq)
{
	struct tevent_req *req =
		tevent_req_callback_data(subreq,
		struct tevent_req);
	NTSTATUS status;

	 
	status = tevent_req_simple_recv_ntstatus(subreq);
	TALLOC_FREE(subreq);
	if (tevent_req_nterror(req, status)) {
		return;
	}

	 
	tevent_req_nterror(req, NT_STATUS_INTERNAL_ERROR);
}
