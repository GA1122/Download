struct sctp_chunk *sctp_make_abort(const struct sctp_association *asoc,
			      const struct sctp_chunk *chunk,
			      const size_t hint)
{
	struct sctp_chunk *retval;
	__u8 flags = 0;

	 
	if (!asoc) {
		if (chunk && chunk->chunk_hdr &&
		    chunk->chunk_hdr->type == SCTP_CID_INIT)
			flags = 0;
		else
			flags = SCTP_CHUNK_FLAG_T;
	}

	retval = sctp_make_control(asoc, SCTP_CID_ABORT, flags, hint);

	 
	if (retval && chunk)
		retval->transport = chunk->transport;

	return retval;
}
