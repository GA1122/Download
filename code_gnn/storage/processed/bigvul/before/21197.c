int __tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	struct mmu_gather_batch *batch;

	VM_BUG_ON(!tlb->need_flush);

	if (tlb_fast_mode(tlb)) {
		free_page_and_swap_cache(page);
		return 1;  
	}

	batch = tlb->active;
	batch->pages[batch->nr++] = page;
	if (batch->nr == batch->max) {
		if (!tlb_next_batch(tlb))
			return 0;
		batch = tlb->active;
	}
	VM_BUG_ON(batch->nr > batch->max);

	return batch->max - batch->nr;
}