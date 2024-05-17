int sctp_user_addto_chunk(struct sctp_chunk *chunk, int off, int len,
			  struct iovec *data)
{
	__u8 *target;
	int err = 0;

	 
	target = skb_put(chunk->skb, len);

	 
	if ((err = memcpy_fromiovecend(target, data, off, len)))
		goto out;

	 
	chunk->chunk_hdr->length =
		htons(ntohs(chunk->chunk_hdr->length) + len);
	chunk->chunk_end = skb_tail_pointer(chunk->skb);

out:
	return err;
}
