bool req_is_in_chain(struct smb_request *req)
{
	if (req->vwv != (uint16_t *)(req->inbuf+smb_vwv)) {
		 
		return true;
	}

	if (!is_andx_req(req->cmd)) {
		return false;
	}

	if (req->wct < 2) {
		 
		return false;
	}

	return (CVAL(req->vwv+0, 0) != 0xFF);
}
