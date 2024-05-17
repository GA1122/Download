void  sctp_init_cause(struct sctp_chunk *chunk, __be16 cause_code,
		      size_t paylen)
{
	sctp_errhdr_t err;
	__u16 len;

	 
	err.cause = cause_code;
	len = sizeof(sctp_errhdr_t) + paylen;
	err.length  = htons(len);
	chunk->subh.err_hdr = sctp_addto_chunk(chunk, sizeof(sctp_errhdr_t), &err);
}