void *sctp_addto_chunk(struct sctp_chunk *chunk, int len, const void *data)
{
	void *target;
	void *padding;
	int chunklen = ntohs(chunk->chunk_hdr->length);
	int padlen = WORD_ROUND(chunklen) - chunklen;

	padding = skb_put(chunk->skb, padlen);
	target = skb_put(chunk->skb, len);

	memset(padding, 0, padlen);
	memcpy(target, data, len);

	 
	chunk->chunk_hdr->length = htons(chunklen + padlen + len);
	chunk->chunk_end = skb_tail_pointer(chunk->skb);

	return target;
}