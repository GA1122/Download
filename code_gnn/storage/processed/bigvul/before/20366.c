inline int kvm_is_mmio_pfn(pfn_t pfn)
{
	if (pfn_valid(pfn)) {
		int reserved;
		struct page *tail = pfn_to_page(pfn);
		struct page *head = compound_trans_head(tail);
		reserved = PageReserved(head);
		if (head != tail) {
			 
			smp_rmb();
			if (PageTail(tail))
				return reserved;
		}
		return PageReserved(tail);
	}

	return true;
}
