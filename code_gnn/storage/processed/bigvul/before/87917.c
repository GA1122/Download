static bool fm10k_can_reuse_rx_page(struct fm10k_rx_buffer *rx_buffer,
				    struct page *page,
				    unsigned int __maybe_unused truesize)
{
	 
	if (unlikely(fm10k_page_is_reserved(page)))
		return false;

#if (PAGE_SIZE < 8192)
	 
	if (unlikely(page_count(page) != 1))
		return false;

	 
	rx_buffer->page_offset ^= FM10K_RX_BUFSZ;
#else
	 
	rx_buffer->page_offset += truesize;

	if (rx_buffer->page_offset > (PAGE_SIZE - FM10K_RX_BUFSZ))
		return false;
#endif

	 
	page_ref_inc(page);

	return true;
}
