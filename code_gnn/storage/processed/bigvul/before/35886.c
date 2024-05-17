static int sctp_process_hn_param(const struct sctp_association *asoc,
				 union sctp_params param,
				 struct sctp_chunk *chunk,
				 struct sctp_chunk **errp)
{
	__u16 len = ntohs(param.p->length);

	 
	if (*errp)
		sctp_chunk_free(*errp);

	*errp = sctp_make_op_error_space(asoc, chunk, len);

	if (*errp) {
		sctp_init_cause(*errp, SCTP_ERROR_DNS_FAILED, len);
		sctp_addto_chunk(*errp, len, param.v);
	}

	 
	return 0;
}
