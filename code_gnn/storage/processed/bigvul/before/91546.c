static bool hva_to_pfn_fast(unsigned long addr, bool write_fault,
			    bool *writable, kvm_pfn_t *pfn)
{
	struct page *page[1];
	int npages;

	 
	if (!(write_fault || writable))
		return false;

	npages = __get_user_pages_fast(addr, 1, 1, page);
	if (npages == 1) {
		*pfn = page_to_pfn(page[0]);

		if (writable)
			*writable = true;
		return true;
	}

	return false;
}