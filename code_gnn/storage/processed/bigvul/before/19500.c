static int tso_fill_packet_with_fragment(struct efx_tx_queue *tx_queue,
					 const struct sk_buff *skb,
					 struct tso_state *st)
{
	struct efx_tx_buffer *buffer;
	int n, end_of_packet, rc;

	if (st->in_len == 0)
		return 0;
	if (st->packet_space == 0)
		return 0;

	EFX_BUG_ON_PARANOID(st->in_len <= 0);
	EFX_BUG_ON_PARANOID(st->packet_space <= 0);

	n = min(st->in_len, st->packet_space);

	st->packet_space -= n;
	st->out_len -= n;
	st->in_len -= n;

	rc = efx_tx_queue_insert(tx_queue, st->dma_addr, n, &buffer);
	if (likely(rc == 0)) {
		if (st->out_len == 0)
			 
			buffer->skb = skb;

		end_of_packet = st->out_len == 0 || st->packet_space == 0;
		buffer->continuation = !end_of_packet;

		if (st->in_len == 0) {
			 
			buffer->unmap_len = st->unmap_len;
			buffer->unmap_single = st->unmap_single;
			st->unmap_len = 0;
		}
	}

	st->dma_addr += n;
	return rc;
}
