perf_mmap_to_page(struct ring_buffer *rb, unsigned long pgoff)
{
	if (pgoff > (1UL << page_order(rb)))
		return NULL;

	return vmalloc_to_page((void *)rb->user_page + pgoff * PAGE_SIZE);
}
