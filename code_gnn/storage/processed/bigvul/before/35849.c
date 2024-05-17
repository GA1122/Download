static int sctp_init_cause_fixed(struct sctp_chunk *chunk, __be16 cause_code,
		      size_t paylen)
{
	sctp_errhdr_t err;
	__u16 len;

	 
	err.cause = cause_code;
	len = sizeof(sctp_errhdr_t) + paylen;
	err.length  = htons(len);

	if (skb_tailroom(chunk->skb) < len)
		return -ENOSPC;
	chunk->subh.err_hdr = sctp_addto_chunk_fixed(chunk,
						     sizeof(sctp_errhdr_t),
						     &err);
	return 0;
}