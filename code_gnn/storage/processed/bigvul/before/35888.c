static int sctp_process_inv_mandatory(const struct sctp_association *asoc,
				      struct sctp_chunk *chunk,
				      struct sctp_chunk **errp)
{
	 

	if (!*errp)
		*errp = sctp_make_op_error_space(asoc, chunk, 0);

	if (*errp)
		sctp_init_cause(*errp, SCTP_ERROR_INV_PARAM, 0);

	 
	return 0;
}