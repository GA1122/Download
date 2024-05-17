static void *sctp_addto_param(struct sctp_chunk *chunk, int len,
			      const void *data)
{
	void *target;
	int chunklen = ntohs(chunk->chunk_hdr->length);

	target = skb_put(chunk->skb, len);

	if (data)
		memcpy(target, data, len);
	else
		memset(target, 0, len);

	 
	chunk->chunk_hdr->length = htons(chunklen + len);
	chunk->chunk_end = skb_tail_pointer(chunk->skb);

	return target;
}