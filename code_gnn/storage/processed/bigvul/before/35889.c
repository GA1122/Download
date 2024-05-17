static int sctp_process_inv_paramlength(const struct sctp_association *asoc,
					struct sctp_paramhdr *param,
					const struct sctp_chunk *chunk,
					struct sctp_chunk **errp)
{
	 
	if (*errp)
		sctp_chunk_free(*errp);

	 
	*errp = sctp_make_violation_paramlen(asoc, chunk, param);

	return 0;
}
