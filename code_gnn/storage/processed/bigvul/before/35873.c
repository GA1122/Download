static struct sctp_chunk *sctp_make_op_error_space(
	const struct sctp_association *asoc,
	const struct sctp_chunk *chunk,
	size_t size)
{
	struct sctp_chunk *retval;

	retval = sctp_make_control(asoc, SCTP_CID_ERROR, 0,
				   sizeof(sctp_errhdr_t) + size);
	if (!retval)
		goto nodata;

	 
	if (chunk)
		retval->transport = chunk->transport;

nodata:
	return retval;
}
