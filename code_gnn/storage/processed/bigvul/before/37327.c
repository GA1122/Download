void sctp_inq_push(struct sctp_inq *q, struct sctp_chunk *chunk)
{
	 
	if (chunk->rcvr->dead) {
		sctp_chunk_free(chunk);
		return;
	}

	 
	list_add_tail(&chunk->list, &q->in_chunk_list);
	if (chunk->asoc)
		chunk->asoc->stats.ipackets++;
	q->immediate.func(&q->immediate);
}
