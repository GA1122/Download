struct sctp_chunk *sctp_make_abort_no_data(
	const struct sctp_association *asoc,
	const struct sctp_chunk *chunk, __u32 tsn)
{
	struct sctp_chunk *retval;
	__be32 payload;

	retval = sctp_make_abort(asoc, chunk, sizeof(sctp_errhdr_t)
				 + sizeof(tsn));

	if (!retval)
		goto no_mem;

	 
	payload = htonl(tsn);
	sctp_init_cause(retval, SCTP_ERROR_NO_DATA, sizeof(payload));
	sctp_addto_chunk(retval, sizeof(payload), (const void *)&payload);

	 
	if (chunk)
		retval->transport = chunk->transport;

no_mem:
	return retval;
}
