struct sctp_chunk *sctp_make_cookie_echo(const struct sctp_association *asoc,
				    const struct sctp_chunk *chunk)
{
	struct sctp_chunk *retval;
	void *cookie;
	int cookie_len;

	cookie = asoc->peer.cookie;
	cookie_len = asoc->peer.cookie_len;

	 
	retval = sctp_make_control(asoc, SCTP_CID_COOKIE_ECHO, 0, cookie_len);
	if (!retval)
		goto nodata;
	retval->subh.cookie_hdr =
		sctp_addto_chunk(retval, cookie_len, cookie);

	 
	if (chunk)
		retval->transport = chunk->transport;

nodata:
	return retval;
}
